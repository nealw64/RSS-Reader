#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("RSS-Reader"));
    this->setMinimumSize(1200, 900);

    url = new QUrl;
    xmlParser = new XMLparser;
    xmlReader = new QXmlStreamReader;
    treeWidget = new QTreeWidget;
    feedBrowser = new QTextBrowser;
    window = new QWidget;

    feedBrowser->setOpenLinks(false);

    treeWidget->setColumnCount(3);
    QStringList stringList;
    stringList.push_back("url");
    stringList.push_back("title");
    stringList.push_back("date");
    treeWidget->setHeaderLabels(stringList);
    treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(treeWidget);
    vBoxLayout->addWidget(feedBrowser);
    window->setLayout(vBoxLayout);
    this->setCentralWidget(window);

    connect(this, SIGNAL(feedFound()), this, SLOT(addFeedsToDatabase()));
    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onTreeWidgetItem_clicked(QTreeWidgetItem*)));
    connect(feedBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(onRSSLink_clicked(QUrl)));
    setUpDatabase();
    updateInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("RSS-Reader");
    database.open();
    if (!database.isOpen())
    {
        qDebug() << database.lastError().text();
        exit(0);
    }
    query = new QSqlQuery;
    query->exec("CREATE TABLE IF NOT EXISTS Feed (url VARCHAR, title VARCHAR UNIQUE NOT NULL, content VARCHAR, date DATETIME, link VARCHAR, CONSTRAINT Feed PRIMARY KEY (title))");
}

void MainWindow::addFeedsToDatabase()
{
    if (!xmlParser->parseXML(xmlReader, url, query))
    {
        exit(0);
    }
    xmlReader->clear();
    updateInfo();
}

void MainWindow::updateInfo()
{
    query->exec("SELECT * FROM Feed");
    QSqlRecord rec = query->record();
    while (query->next())
    {
        QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
        treeWidgetItem->setText(0, query->value(rec.indexOf("url")).toString());
        treeWidgetItem->setText(1, query->value(rec.indexOf("title")).toString());
        treeWidgetItem->setText(2, query->value(rec.indexOf("date")).toString());
        treeWidget->addTopLevelItem(treeWidgetItem);
    }
    for (int i = 0; i < treeWidget->columnCount(); i++)
    {
        treeWidget->resizeColumnToContents(i);
    }
}

void MainWindow::onTreeWidgetItem_clicked(QTreeWidgetItem* item)
{
    feedBrowser->clear();
    QTextEdit output;
    QString request = "SELECT * FROM Feed WHERE title LIKE '%" + item->text(1).replace("'", "''")+ "%'";
    query->exec(request);
    QSqlRecord rec = query->record();
    while (query->next())
    {
        output.append(query->value(rec.indexOf("title")).toString());
        output.append(query->value(rec.indexOf("date")).toString());
        output.append(query->value(rec.indexOf("content")).toString());
        output.append("<a href='" + (query->value(rec.indexOf("link")).toString() + "'>" + tr("Read more here") + "</a>"));
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
    if (searchDialog.exec() == QDialog::Accepted)
    {
       url = searchDialog.getFeedUrl();
       xmlReader->addData(searchDialog.getDownloadedData());
       emit feedFound();
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
