#include "xmlparser.h"

XmlParser::XmlParser()
{
    months.insert("Jan", "01");
    months.insert("Feb", "02");
    months.insert("Mar", "03");
    months.insert("Apr", "04");
    months.insert("May", "05");
    months.insert("Jun", "06");
    months.insert("Jul", "07");
    months.insert("Aug", "08");
    months.insert("Sep", "09");
    months.insert("Okt", "10");
    months.insert("Nov", "11");
    months.insert("Des", "12");
}

bool XmlParser::parseXml(QXmlStreamReader *xml, QUrl *url, QSqlQuery *query)
{
    QString title;
    QString category;
    QString content;
    QString date;
    QString link;
    QString channelTitle;
    QString previousTag;

    while (!xml->atEnd()) {
        xml->readNext();
        if (xml->isStartElement()) {
            currentTag = xml->name().toString();
            if (currentTag == "channel") {
                previousTag = currentTag;
                continue;
            }
        } else if (xml->isEndElement()) {
            if (xml->name() == "item") {
                Feed feed(title, category, content, date, link);
                feeds.append(feed);
            }
        } else if (xml->isCharacters() && !xml->isWhitespace()) {
            if (currentTag == "title" && previousTag == "channel") {
                channelTitle = xml->text().toString();
                previousTag = "";
            } else if (currentTag == "title") {
                title = xml->text().toString();
            } else if (currentTag == "category") {
                category = xml->text().toString();
            } else if (currentTag == "description") {
                content = this->extractAndParseDescription(xml->text().toString());
            } else if (currentTag == "link") {
                link = xml->text().toString();
            } else if (currentTag == "pubDate" || currentTag == "date") {
                date = this->extractAndParseDate(xml->text().toString());
            }
        }
    }

    if (feeds.isEmpty()) {
        return false;
    }

    QString request;
    foreach (Feed feed, feeds) {
        request = "INSERT INTO Feed (name, url, title, category, content, date, link)"
                  "VALUES (:stringName, :stringUrl, :stringTitle, :stringCategory, :stringContent,"
                  "DATETIME(:stringDate), :stringLink)";
        query->prepare(request);
        query->bindValue(":stringName", channelTitle.trimmed());
        query->bindValue(":stringUrl", url->toString());
        query->bindValue(":stringTitle", feed.getTitle().trimmed());
        query->bindValue(":stringCategory", feed.getCategory().trimmed());
        query->bindValue(":stringContent", feed.getContent().trimmed());
        query->bindValue(":stringDate", feed.getDate().trimmed());
        query->bindValue(":stringLink", feed.getLink().trimmed());
        query->exec();
    }

    return true;
}

QString XmlParser::extractAndParseDate(QString pubDate)
{
    QStringList pubDateList = pubDate.split(QRegExp("\\W+"));
    QString formatedDate = pubDateList.value(3) + "-" + months.value(pubDateList.value(2)) + "-"
            + pubDateList.value(1) + " " + pubDateList.value(4) + ":" + pubDateList.value(5)
            + ":" + pubDateList.value(6);
    return formatedDate;
}

QString XmlParser::extractAndParseDescription(QString description)
{
    QStringList currentExp;
    currentExp.append("<p><a");
    currentExp.append("</a></p>");
    currentExp.append("<br");
    currentExp.append("/>");
    currentExp.append("<div");
    currentExp.append("/>");
    while (!currentExp.isEmpty()) {
        if (removeCurrentExp(&description, currentExp)) {
            continue;
        } else {
            currentExp.removeFirst();
            currentExp.removeFirst();
        }
    }
    return description;
}

bool XmlParser::removeCurrentExp(QString *description, QStringList currentExp)
{
    int indexFirst = 0, indexLast = 0;
    indexFirst = description->indexOf(currentExp[0]);
    if (currentExp[1] != "/>") {
      indexLast = description->indexOf(currentExp[1]);
    } else {
      indexLast = description->lastIndexOf(currentExp[1]);
    }
    if (indexFirst != -1 && indexLast != -1) {
        int length = indexLast + currentExp[1].length() - indexFirst;
        description->remove(indexFirst, length);
        return true;
    }
    return false;
}
