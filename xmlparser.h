#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlStreamReader>
#include <QtSql>
#include <QUrl>
#include <feed.h>

class XmlParser
{
public:
    XmlParser();
    bool parseXml(QXmlStreamReader *xml, QUrl* url, QSqlQuery* query);
    QString extractAndParseDate(QString pubDate);
    QString extractAndParseDescription(QString description);

private:
    QString currentTag;
    QLinkedList<Feed> feeds;
    QMap<QString, QString> months;
};

#endif // XMLPARSER_H
