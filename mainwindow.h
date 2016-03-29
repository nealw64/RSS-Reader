#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QtSql>
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
    void setUpDatabase();

signals:
    void feedFound();

private slots:
    void addFeedsToDatabase();
    void updateInfo();
    void on_actionAdd_triggered();
    void on_actionExit_triggered();
    void onTreeWidgetItem_clicked(QTreeWidgetItem* item);
    void onRSSLink_clicked(QUrl url);

private:
    Ui::MainWindow* ui;
    QSqlDatabase database;
    QSqlQuery* query;
    XMLparser* xmlParser;
    QXmlStreamReader* xmlReader;
    QUrl* url;
    QVBoxLayout* vBoxLayout;
    QTreeWidget* treeWidget;
    QTextBrowser* feedBrowser;
    QWidget* window;
};

#endif // MAINWINDOW_H
