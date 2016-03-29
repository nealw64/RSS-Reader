#include "filedownloader.h"

FileDownloader::FileDownloader(QObject* parent): QObject(parent)
{
    connect(&netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
}

void FileDownloader::downloadData(QUrl url)
{
    QNetworkRequest request(url);
    netManager.get(request);
}

void FileDownloader::fileDownloaded(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << "ERROR";
        qDebug() << reply->errorString();
    }
    else
    {
        downloadedData = reply->readAll();
        emit downloaded();
    }
}

QByteArray FileDownloader::getDownloadedData() const
{
    return downloadedData;
}
