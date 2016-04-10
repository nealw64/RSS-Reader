#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QtSql>
#include <QtWidgets>
#include "searchdialog.h"
#include "xmlparser.h"
#include "filedownloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupDatabase();
    void parseXml();

signals:
    void feedFound();
    void getFeed();
    void readyForUpdate();
    void checkUrlForDownload();

private slots:
    void checkUrlForDownloading();
    void downloadFeed();
    void feedIsDownloaded();
    void addFeedsToDatabase();
    void updateChannelInfo();
    void updateFeedsInfo();
    void on_actionAdd_triggered();  
    void on_actionDelete_triggered();
    void on_actionExit_triggered();
    void on_actionUpdate_triggered();
    void onChannelItem_clicked(QTreeWidgetItem *item);
    void onFeedItem_clicked(QTreeWidgetItem *item);
    void onRSSLink_clicked(QUrl url);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQuery *query;
    XmlParser *xmlParser;
    QXmlStreamReader *xmlReader;
    QUrl *url;
    QVBoxLayout *vBoxLayout;
    QHBoxLayout *hBoxLayout;
    QTreeWidget *feedTreeWidget;
    QTreeWidget *channelTreeWidget;
    QTextBrowser *feedBrowser;
    QWidget *feedPanel;
    QWidget *mainPanel;
    QString request;
    QString currentChannelName;
    FileDownloader *fileDownloader;
    QTimer *timer;
    const int UPDATE_FREQUENCY;
};

#endif // MAINWINDOW_H
