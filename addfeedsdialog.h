#ifndef ADDFEEDSDIALOG_H
#define ADDFEEDSDIALOG_H

#include <QDialog>
#include <QProgressDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>
#include "filedownloader.h"

class AddFeedsDialog : public QDialog
{
    Q_OBJECT
public:
    AddFeedsDialog(QWidget *parent = 0);
    ~AddFeedsDialog();
    QUrl getFeedsUrl();
    QByteArray getDownloadedData();

signals:
    void validUrl();

private slots:
    void addFeeds();
    void updateDownloadProgress(qint64, qint64);
    void accepted();
    void isValidUrl();

private:
    QLineEdit *linkInput;
    QPushButton *addButton;
    QLabel *messageLabel;
    QGridLayout *gridLayout;
    QProgressDialog *progressDialog;
    QString query;
    QUrl feedUrl;
    FileDownloader fileDownloader;
};

#endif // ADDFEEDSDIALOG_H
