#include "filedownloader.h"

FileDownloader::FileDownloader(QObject *parent): QObject(parent)
{
    netManager = new QNetworkAccessManager;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(fileDownloaded(QNetworkReply*)));
}

FileDownloader::~FileDownloader()
{
    delete netManager;
}

void FileDownloader::downloadData(QUrl url)
{
    QNetworkRequest request(url);
    connect(netManager->get(request), SIGNAL(downloadProgress(qint64, qint64)),
            SIGNAL(downloadProgress(qint64, qint64)));
}

void FileDownloader::fileDownloaded(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << "ERROR";
        qDebug() << reply->errorString();
    } else {
        downloadedData = reply->readAll();
        emit downloaded();
    }
}

QByteArray FileDownloader::getDownloadedData() const
{
    return downloadedData;
}
