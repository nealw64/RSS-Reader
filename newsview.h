#ifndef NEWSVIEW_H
#define NEWSVIEW_H

#include <QTextEdit>
#include <QObject>
#include <QtSql>
#include "feed.h"
#include "filedownloader.h"

class NewsView: public QObject
{
    Q_OBJECT
public:
    explicit NewsView(QObject *parent = 0);
    void getContent(QSqlQuery *query);
    void downloadImage();

signals:
    void noImage();
    void imageSaved();
    void pageIsReady(QString);

private slots:
    void imageDownloaded();
    void setContent();
    void downloadFailed(QString error);

private:
    FileDownloader fileDownloader;
    Feed feed;
};

#endif // NEWSVIEW_H
