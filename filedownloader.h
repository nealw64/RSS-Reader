#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QtNetwork>

class FileDownloader: public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    ~FileDownloader();
    QByteArray getDownloadedData() const;
    void downloadData();
    void addUrlForDownload(QUrl url);
    QString getCurrentUrl();
    bool isUrlForDownloadEmpty();

signals:
    void downloaded();
    void downloadProgress(qint64, qint64);
    void replyError(QString);

private slots:
    void fileDownloaded(QNetworkReply *reply);

private:
    QNetworkAccessManager *netManager;
    QByteArray downloadedData;
    QStringList urlForDownload;
};

#endif // FILEDOWNLOADER_H
