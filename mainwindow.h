#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
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

signals:
    void feedFound();
    void readyForUpdate();

private slots:
    void addFeedsToDatabase();
    void updateChannelInfo();
    void on_actionAdd_triggered();
    void on_actionExit_triggered();
    void onChannelItem_clicked(QTreeWidgetItem* item);
    void onFeedItem_clicked(QTreeWidgetItem* item);
    void onRSSLink_clicked(QUrl url);

private:
    Ui::MainWindow* ui;
    QSqlDatabase database;
    QSqlQuery* query;
    XMLparser* xmlParser;
    QXmlStreamReader* xmlReader;
    QUrl* url;
    QVBoxLayout* vBoxLayout;
    QHBoxLayout* hBoxLayout;
    QTreeWidget* feedTreeWidget;
    QTreeWidget* channelTreeWidget;
    QTextBrowser* feedBrowser;
    QWidget* feedPanel;
    QWidget* mainPanel;
};

#endif // MAINWINDOW_H
