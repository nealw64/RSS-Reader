#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QtSql>
#include <QMessageBox>
#include <QtWidgets>
#include "searchdialog.h"
#include "xmlparser.h"
#include "filedownloader.h"
#include "newsview.h"

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
    void feedsFound();
    void getFeeds();
    void readyForUpdateChannels();
    void checkUrlListForUpdatingChannels();

private slots:
    void checkUrlListForGetFeeds();
    void downloadFeeds();
    void feedsIsDownloaded();
    void addFeedsToDatabase();
    void updateChannelsInfo();
    void updateFeedsInfo();
    void feedIsRead(QTreeWidgetItem *item);
    void on_actionAdd_triggered();
    void on_actionDelete_triggered();
    void on_actionExit_triggered();
    void on_actionUpdate_triggered();
    void onChannelItem_clicked(QTreeWidgetItem *item);
    void onFeedItem_clicked(QTreeWidgetItem *item);
    void onRssLink_clicked(QUrl url);
    void on_actionShow_all_feeds_triggered();
    void on_actionShow_only_unread_feeds_triggered();

    void on_actionFull_screen_triggered();

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
