#include "newsview.h"

NewsView::NewsView(QObject *parent): QObject(parent)
{
    connect(&fileDownloader, SIGNAL(downloaded()), this, SLOT(imageDownloaded()));
    connect(this, SIGNAL(imageSaved()), this, SLOT(setContent()));
    connect(this, SIGNAL(noImage()), this, SLOT(setContent()));
}

void NewsView::getContent(QSqlQuery *query)
{
    QSqlRecord rec = query->record();
    while (query->next()) {
        feed.setTitle(query->value(rec.indexOf("title")).toString());
        feed.setDate(query->value(rec.indexOf("date")).toString());
        feed.setContent(query->value(rec.indexOf("content")).toString());
        feed.setLink(query->value(rec.indexOf("link")).toString());
    }
    downloadImage();
}

void NewsView::setContent()
{
    QString page = "<html><head><meta http-equiv='Content-Type' content='text/html; "
                   "charset=windows-1251'><link rel='stylesheet' type='text/css' "
                   "href=':/styles/styles.css'></head><body><p><titleStyle>"
                    + feed.getTitle() + "</titleStyle></p><p><dateStyle>" + feed.getDate()
                    + "</dateStyle></p><contentStyle>" + feed.getContent() + "</contentStyle>";
    if (feed.getContent().indexOf(feed.getLink()) == -1) {
        page += "<br><br><readMoreStyle><a href='" + feed.getLink() + "'>"
                + QObject::tr("Read more here") + "</a></readMoreStyle></body></html>";
    }
    emit pageIsReady(page);
}

void NewsView::downloadImage()
{
    QString link = feed.getContent();
    QString currentExp = "<img src=";
    int indexFirst = link.indexOf(currentExp);
    if (indexFirst == -1) {
        emit noImage();
        return;
    }
    link.remove(0, ++indexFirst + currentExp.length());
    currentExp = " alt=";
    int indexLast = link.indexOf(currentExp);
    indexLast--;
    link.remove(indexLast, link.size() - indexLast);
    fileDownloader.addUrlForDownload(link);
    fileDownloader.downloadData();
}

void NewsView::imageDownloaded()
{
    QString path = qApp->applicationDirPath() + "/tmp";
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(fileDownloader.getDownloadedData());
    file.close();
    QString content = feed.getContent();
    content.replace(fileDownloader.getCurrentUrl(), path);
    feed.setContent(content);
    emit imageSaved();
}
