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

    feedTreeWidget->setColumnCount(4);
    QTreeWidgetItem *headerItem = new QTreeWidgetItem;
    headerItem->setIcon(0, QIcon(":/img/readHeader.png"));
    headerItem->setText(1, "Title");
    headerItem->setText(2, "Category");
    headerItem->setText(3, "Date");
    feedTreeWidget->setHeaderItem(headerItem);
    feedTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    feedTreeWidget->setIndentation(0);
    feedTreeWidget->setSortingEnabled(true);
    feedTreeWidget->sortItems(3, Qt::DescendingOrder);

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

    connect(this, SIGNAL(feedsFound()), this, SLOT(addFeedsToDatabase()));
    connect(this, SIGNAL(readyForUpdateChannels()), this, SLOT(updateChannelsInfo()));
    connect(channelTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
            SLOT(onChannelItem_clicked(QTreeWidgetItem*)));
    connect(feedTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
            SLOT(onFeedItem_clicked(QTreeWidgetItem*)));
    connect(feedTreeWidget, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this,
            SLOT(onFeedItem_clicked(QTreeWidgetItem*)));
    connect(feedBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(onRssLink_clicked(QUrl)));
    connect(this, SIGNAL(getFeeds()), this, SLOT(downloadFeeds()));
    connect(fileDownloader, SIGNAL(downloaded()), this, SLOT(feedsIsDownloaded()));
    connect(fileDownloader, SIGNAL(replyError(QString)), this, SLOT(updateFeedsFailed(QString)));
    connect(this, SIGNAL(checkUrlListForUpdatingChannels()), this, SLOT(checkUrlListForGetFeeds()));
    connect(timer, SIGNAL(timeout()), this, SLOT(on_actionUpdate_triggered()));
    connect(channelTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this,
            SLOT(onChannelItem_clicked(QTreeWidgetItem*)));
    connect(feedTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this,
            SLOT(onFeedItem_clicked(QTreeWidgetItem*)));
    timer->start(UPDATE_FREQUENCY);
    setupDatabase();
    updateChannelsInfo();
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
    request = "CREATE TABLE IF NOT EXISTS Feed (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
              "name VARCHAR, url VARCHAR, title VARCHAR, category VARCHAR, content VARCHAR,"
              "date DATETIME, link VARCHAR, unread INTEGER, UNIQUE(title, url, name))";
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
    emit readyForUpdateChannels();
}

void MainWindow::checkUrlListForGetFeeds()
{
    if (fileDownloader->isUrlForDownloadEmpty()) {
        request = "SELECT title, category, date, unread FROM Feed WHERE name = '"
                + currentChannelName + "'";
        updateFeedsInfo();
        return;
    }
    emit getFeeds();
}

void MainWindow::downloadFeeds()
{
    fileDownloader->downloadData();
}

void MainWindow::feedsIsDownloaded()
{
    xmlReader->addData(fileDownloader->getDownloadedData());
    url->setUrl(fileDownloader->getCurrentUrl());
    query->clear();
    parseXml();
    emit checkUrlListForUpdatingChannels();
}


void MainWindow::updateChannelsInfo()
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
        if (query->value(rec.indexOf("unread")).toInt() == 1) {
            feedTreeWidgetItem->setIcon(0, QIcon(":/img/unread.png"));
        } else {
            feedTreeWidgetItem->setIcon(0, QIcon(":/img/read.png"));
        }
        feedTreeWidgetItem->setText(1, query->value(rec.indexOf("title")).toString());
        feedTreeWidgetItem->setText(2, query->value(rec.indexOf("category")).toString());
        feedTreeWidgetItem->setText(3, query->value(rec.indexOf("date")).toString());
        feedTreeWidget->addTopLevelItem(feedTreeWidgetItem);
    }
    for (int i = 0; i < feedTreeWidget->columnCount(); i++) {
        feedTreeWidget->resizeColumnToContents(i);
    }
}

void MainWindow::updateFeedsFailed(QString error)
{
    QMessageBox::warning(this, tr("Error"), error, QMessageBox::Ok);
}

void MainWindow::feedIsRead(QTreeWidgetItem *item)
{
    QString title = item->text(1);
    request = "UPDATE Feed SET unread = NULL WHERE title = :stringTitle AND name = :stringName";
    query->prepare(request);
    query->bindValue(":stringTitle", title);
    query->bindValue(":stringName", currentChannelName);
    query->exec();
    item->setIcon(0, QIcon(":/img/read.png"));
}

void MainWindow::onChannelItem_clicked(QTreeWidgetItem *item)
{
    currentChannelName = item->text(0);
    request = "SELECT title, category, date, unread FROM Feed WHERE name = '"
            + currentChannelName + "'";
    updateFeedsInfo();
    feedTreeWidget->sortItems(3, Qt::DescendingOrder);
}

void MainWindow::onFeedItem_clicked(QTreeWidgetItem *item)
{
    query->clear();
    feedBrowser->clear();
    QString title = item->text(1).replace("'", "''");
    request = "SELECT title, date, content, link FROM Feed WHERE title = '" + title + "'";
    query->exec(request);
    NewsView::setContent(feedBrowser, query);
    feedIsRead(item);
}

void MainWindow::onRssLink_clicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionAdd_triggered()
{
    AddFeedsDialog searchDialog(this);
    if (searchDialog.exec() == QDialog::Accepted) {
        *url = searchDialog.getFeedsUrl();
        xmlReader->addData(searchDialog.getDownloadedData());
        emit feedsFound();
    }
}

void MainWindow::on_actionDelete_triggered()
{
    if (QApplication::focusWidget() == feedTreeWidget) {
        int index = feedTreeWidget->currentIndex().row();
        QString title = feedTreeWidget->takeTopLevelItem(index)->text(1);
        request = "DELETE FROM Feed WHERE title = '" + title + "' AND name = '"
                + currentChannelName + "'";
        query->exec(request);
    } else if (QApplication::focusWidget() == channelTreeWidget) {
        int index = channelTreeWidget->currentIndex().row();
        QString name = channelTreeWidget->takeTopLevelItem(index)->text(0);
        request = "DELETE FROM Feed WHERE name = '" + name + "'";
        query->exec(request);
        if (name == currentChannelName) {
            feedTreeWidget->clear();
        }
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    if (!fileDownloader->isUrlForDownloadEmpty()) {
        return;
    }
    query->clear();
    request = "SELECT DISTINCT url FROM Feed";
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next()) {
        fileDownloader->addUrlForDownload(query->value(rec.indexOf("url")).toString());
    }
    emit checkUrlListForUpdatingChannels();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionShow_all_feeds_triggered()
{
    request = "SELECT title, category, date, unread FROM Feed WHERE name = '"
            + currentChannelName + "'";
    updateFeedsInfo();
}


void MainWindow::on_actionShow_only_unread_feeds_triggered()
{
    request = "SELECT title, category, date, unread FROM Feed WHERE name = '"
            + currentChannelName + "' AND unread = '1'";
    updateFeedsInfo();
}

void MainWindow::on_actionFull_screen_triggered()
{
    showMaximized();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, tr("About RSS-Reader"),
                             tr("RSS-Reader was written by George Kovalenko as a course project.\n"
                                                              "BSUIR, 2016."),
                                 QMessageBox::Ok);
}
