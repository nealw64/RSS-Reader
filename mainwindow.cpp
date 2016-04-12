#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    UPDATE_FREQUENCY(600000)
{
    ui->setupUi(this);
    setWindowTitle(tr("RSS-Reader"));
    setWindowIcon(QIcon(":/img/windowIcon.png"));
    setMinimumSize(1200, 900);

    url = new QUrl;
    xmlParser = new XmlParser;
    xmlReader = new QXmlStreamReader;
    feedTreeWidget = new QTreeWidget;
    channelTreeWidget = new QTreeWidget;
    feedBrowser = new QTextBrowser;
    feedPanel = new QWidget;
    mainPanel = new QWidget;
    fileDownloader = new FileDownloader;
    timer = new QTimer(this);

    feedBrowser->setOpenLinks(false);

    feedTreeWidget->setColumnCount(3);
    QStringList headerLabelsList;
    headerLabelsList.push_back("Title");
    headerLabelsList.push_back("Category");
    headerLabelsList.push_back("Date");
    feedTreeWidget->setHeaderLabels(headerLabelsList);
    feedTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    feedTreeWidget->setSortingEnabled(true);
    feedTreeWidget->sortItems(2, Qt::DescendingOrder);

    channelTreeWidget->setColumnCount(1);
    channelTreeWidget->setHeaderLabel("Channel");
    channelTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    channelTreeWidget->setSortingEnabled(true);
    channelTreeWidget->sortItems(0, Qt::AscendingOrder);

    vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(feedTreeWidget);
    vBoxLayout->addWidget(feedBrowser);
    feedPanel->setLayout(vBoxLayout);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(channelTreeWidget, 1);
    hBoxLayout->addWidget(feedPanel, 4);
    mainPanel->setLayout(hBoxLayout);
    setCentralWidget(mainPanel);

    connect(this, SIGNAL(feedFound()), this, SLOT(addFeedsToDatabase()));
    connect(this, SIGNAL(readyForUpdate()), this, SLOT(updateChannelInfo()));
    connect(channelTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
            SLOT(onChannelItem_clicked(QTreeWidgetItem*)));
    connect(feedTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
            SLOT(onFeedItem_clicked(QTreeWidgetItem*)));
    connect(feedBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(onRSSLink_clicked(QUrl)));
    connect(this, SIGNAL(getFeed()), this, SLOT(downloadFeed()));
    connect(fileDownloader, SIGNAL(downloaded()), this, SLOT(feedIsDownloaded()));
    connect(this, SIGNAL(checkUrlForDownload()), this, SLOT(checkUrlForDownloading()));
    connect(timer, SIGNAL(timeout()), this, SLOT(on_actionUpdate_triggered()));
    timer->start(UPDATE_FREQUENCY);
    setupDatabase();
    updateChannelInfo();
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
    delete query;
    delete xmlParser;
    delete xmlReader;
    delete url;
    delete vBoxLayout;
    delete hBoxLayout;
    delete feedTreeWidget;
    delete channelTreeWidget;
    delete feedBrowser;
    delete feedPanel;
    delete mainPanel;
}

void MainWindow::setupDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("RSS-Reader");
    database.open();
    if (!database.isOpen()) {
        qDebug() << database.lastError().text();
        exit(0);
    }
    query = new QSqlQuery;
    request = "CREATE TABLE IF NOT EXISTS Feed (name VARCHAR, url VARCHAR,"
              "title VARCHAR UNIQUE NOT NULL, category VARCHAR, content VARCHAR, date DATETIME,"
              "link VARCHAR, CONSTRAINT Feed PRIMARY KEY (title))";
    query->exec(request);
}

void MainWindow::parseXml()
{
    if (!xmlParser->parseXml(xmlReader, url, query)) {
        QMessageBox::warning(this, tr("Error"), tr("Was not able to download the feed. "
                                                   "Please make sure you have entered"
                                                   " a valid feed-adress."), QMessageBox::Ok);
    }
    xmlReader->clear();
}

void MainWindow::addFeedsToDatabase()
{
    parseXml();
    emit readyForUpdate();
}

void MainWindow::checkUrlForDownloading()
{
    if (fileDownloader->isUrlForDownloadEmpty()) {
        feedTreeWidget->clear();
        request = "SELECT title, category, date FROM Feed WHERE name LIKE '%"
                + currentChannelName + "%'";
        updateFeedsInfo();
        return;
    }
    emit getFeed();
}

void MainWindow::downloadFeed()
{
    fileDownloader->downloadData();
}

void MainWindow::feedIsDownloaded()
{
    xmlReader->addData(fileDownloader->getDownloadedData());
    url->setUrl(fileDownloader->getCurrentUrl());
    parseXml();
    emit checkUrlForDownload();
}


void MainWindow::updateChannelInfo()
{
    query->clear();
    channelTreeWidget->clear();
    request = "SELECT DISTINCT name FROM Feed";
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next())
    {
        QTreeWidgetItem *channelTreeWidgetItem = new QTreeWidgetItem;
        channelTreeWidgetItem->setText(0, query->value(rec.indexOf("name")).toString());
        channelTreeWidget->addTopLevelItem(channelTreeWidgetItem);
    }
}

void MainWindow::updateFeedsInfo()
{
    query->clear();
    feedTreeWidget->clear();
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next()) {
        QTreeWidgetItem* feedTreeWidgetItem = new QTreeWidgetItem;
        feedTreeWidgetItem->setText(0, query->value(rec.indexOf("title")).toString());
        feedTreeWidgetItem->setText(1, query->value(rec.indexOf("category")).toString());
        feedTreeWidgetItem->setText(2, query->value(rec.indexOf("date")).toString());
        feedTreeWidget->addTopLevelItem(feedTreeWidgetItem);
    }
    for (int i = 0; i < feedTreeWidget->columnCount(); i++) {
        feedTreeWidget->resizeColumnToContents(i);
    }
}

void MainWindow::onChannelItem_clicked(QTreeWidgetItem *item)
{
    feedTreeWidget->clear();
    currentChannelName = item->text(0);
    request = "SELECT title, category, date FROM Feed WHERE name LIKE '%"
            + currentChannelName + "%'";
    updateFeedsInfo();
    feedTreeWidget->sortItems(2, Qt::DescendingOrder);
}

void MainWindow::onFeedItem_clicked(QTreeWidgetItem *item)
{
    query->clear();
    feedBrowser->clear();
    QTextEdit output;
    request = "SELECT title, date, content, link FROM Feed WHERE title LIKE '%"
            + item->text(0).replace("'", "''") + "%'";
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next()) {
        output.append(query->value(rec.indexOf("title")).toString());
        output.append(query->value(rec.indexOf("date")).toString());
        output.append(query->value(rec.indexOf("content")).toString());
        output.append("<a href='" + (query->value(rec.indexOf("link")).toString() + "'>"
                                     + tr("Read more here") + "</a>"));
    }
    feedBrowser->setHtml(output.toHtml());
}

void MainWindow::onRSSLink_clicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionAdd_triggered()
{
    SearchDialog searchDialog(this);
    if (searchDialog.exec() == QDialog::Accepted) {
        *url = searchDialog.getFeedUrl();
        xmlReader->addData(searchDialog.getDownloadedData());
        emit feedFound();
    }
}

void MainWindow::on_actionDelete_triggered()
{
    if (QApplication::focusWidget() == feedTreeWidget) {
        int index = feedTreeWidget->currentIndex().row();
        QString title = feedTreeWidget->takeTopLevelItem(index)->text(0);
        request = "DELETE FROM Feed WHERE title LIKE '%" + title + "%'";
        query->exec(request);
    } else if (QApplication::focusWidget() == channelTreeWidget) {
        int index = channelTreeWidget->currentIndex().row();
        QString name = channelTreeWidget->takeTopLevelItem(index)->text(0);
        request = "DELETE FROM Feed WHERE name LIKE '%" + name + "%'";
        query->exec(request);
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    query->clear();
    request = "SELECT DISTINCT url FROM Feed";
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next()) {
        fileDownloader->addUrlForDownload(query->value(rec.indexOf("url")).toString());
    }
    emit checkUrlForDownload();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
