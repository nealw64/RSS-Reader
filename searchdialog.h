#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>
#include "filedownloader.h"

class SearchDialog : public QDialog
{
Q_OBJECT
public:
    SearchDialog(QWidget *parent = 0);
    QUrl* getFeedUrl();
    QByteArray getDownloadedData();

signals:
    void feedFound();

private slots:
    void searchFeed();

private:
    QString query;
    QLineEdit* searchInput;
    QPushButton* searchButton;
    QUrl* feedUrl;
    QGridLayout* layout;
    FileDownloader* fileDownloader;
};

#endif // SEARCHDIALOG_H
