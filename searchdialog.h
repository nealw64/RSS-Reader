#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QProgressDialog>
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
    ~SearchDialog();
    QUrl getFeedUrl();
    QByteArray getDownloadedData();

signals:
    void validUrl();

private slots:
    void searchFeed();
    void updateDownloadProgress(qint64, qint64);
    void accepted();
    void isValidUrl();

private:
    QLineEdit* searchInput;
    QPushButton* searchButton;
    QLabel* messageLabel;
    QGridLayout* gridLayout;
    QProgressDialog* progressDialog;
    QString query;
    QUrl feedUrl;
    FileDownloader fileDownloader;
};

#endif // SEARCHDIALOG_H
