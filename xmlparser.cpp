#include "xmlparser.h"

XMLparser::XMLparser()
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

bool XMLparser::parseXML(QXmlStreamReader* _xml, QUrl* _url, QSqlQuery* _query)
{
    QString title;
    QString content;
    QString date;
    QString link;

    while (!_xml->atEnd())
    {
        _xml->readNext();
        if (_xml->isStartElement())
        {
            currentTag = _xml->name().toString();
        }
        else if (_xml->isEndElement())
        {
            if (_xml->name() == "item")
            {
                Feed feed(title, content, date, link);
                feeds.append(feed);
            }
        }
        else if (_xml->isCharacters() && !_xml->isWhitespace())
        {
            if (currentTag == "title")
            {
                title = _xml->text().toString();
            }
            else if (currentTag == "description")
            {
                content = this->extractAndParseDescription(_xml->text().toString());
            }
            else if (currentTag == "link")
            {
                link = _xml->text().toString();
            }
            else if (currentTag == "pubDate" || currentTag == "date")
            {
                date = this->extractAndParseDate(_xml->text().toString());
            }
        }
    }

    if (feeds.isEmpty())
    {
        return false;
    }

    foreach (Feed feed, feeds)
    {
        _query->prepare("INSERT INTO Feed (url, title, content, date, link) VALUES (:stringUrl, :stringTitle, :stringContent, DATETIME(:stringDate), :stringLink)");
        _query->bindValue(":stringUrl", _url->toString());
        _query->bindValue(":stringTitle", feed.getTitle().trimmed());
        _query->bindValue(":stringContent", feed.getContent().trimmed());
        _query->bindValue(":stringDate", feed.getDate().trimmed());
        _query->bindValue(":stringLink", feed.getLink().trimmed());
        _query->exec();
    }

    return true;
}

QString XMLparser::extractAndParseDate(QString pubDate)
{
    QStringList pubDateList = pubDate.split(QRegExp("\\W+"));
    QString formatedDate = pubDateList.value(3) + "-" + months.value(pubDateList.value(2)) + "-" +
            pubDateList.value(1) + " " + pubDateList.value(4) + ":" +
            pubDateList.value(5) + ":" + pubDateList.value(6);
    return formatedDate;
}

QString XMLparser::extractAndParseDescription(QString description)
{
    int indexFirst = 0, indexLast = 0;
    QString currentExp;
    while (true)
    {
        currentExp = "<";
        indexFirst = description.indexOf(currentExp);
        currentExp = ">";
        indexLast = description.indexOf(currentExp);
        if (indexFirst != -1 && indexLast != -1)
        {
            int length = indexLast + currentExp.length() - indexFirst;
            description.remove(indexFirst, length);
        }
        else
        {
            break;
        }
    }
    return description;
}
