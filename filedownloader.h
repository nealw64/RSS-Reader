#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QtNetwork>

class FileDownloader: public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject* parent = 0);
    QByteArray getDownloadedData() const;
    void downloadData(QUrl url);
signals:
    void downloaded();
private slots:
    void fileDownloaded(QNetworkReply* reply);
private:
    QNetworkAccessManager netManager;
    QByteArray downloadedData;
};

#endif // FILEDOWNLOADER_H
