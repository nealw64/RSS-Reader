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

void FileDownloader::downloadData()
{
    downloadedData.clear();
    QUrl url(urlForDownload.first());
    QNetworkRequest request(url);
    QNetworkReply *reply = netManager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            SIGNAL(downloadProgress(qint64, qint64)));
}

void FileDownloader::addUrlForDownload(QString url)
{
    urlForDownload.append(url);
}

bool FileDownloader::isUrlForDownloadEmpty()
{
    return urlForDownload.isEmpty();
}

QString FileDownloader::getCurrentUrl()
{
    return urlForDownload.takeFirst();
}

void FileDownloader::fileDownloaded(QNetworkReply *reply)
{
    if (reply->error()) {
        emit networkError(reply->errorString());
        urlForDownload.clear();
    } else {
        downloadedData = reply->readAll();
        emit downloaded();
    }
    reply->deleteLater();
}

QByteArray FileDownloader::getDownloadedData()
{
    return downloadedData;
}
