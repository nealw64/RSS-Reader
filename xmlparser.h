#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlStreamReader>
#include <QtSql>
#include <QUrl>
#include <feed.h>

class XMLparser
{
public:
    XMLparser();
    bool parseXML(QXmlStreamReader *_xml, QUrl* _url, QSqlQuery* _query);
    QString extractAndParseDate(QString pubDate);
    QString extractAndParseDescription(QString description);

private:
    QString currentTag;
    QLinkedList<Feed> feeds;
    QMap<QString, QString> months;
};

#endif // XMLPARSER_H
