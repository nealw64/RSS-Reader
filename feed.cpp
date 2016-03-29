#include "feed.h"

Feed::Feed()
{

}

Feed::Feed(QString _title, QString _content, QString _date, QString _link)
{
    title = _title;
    content = _content;
    date = _date;
    link = _link;
}

QString Feed::getTitle()
{
    return title;
}

QString Feed::getContent()
{
    return content;
}

QString Feed::getDate()
{
    return date;
}

QString Feed::getLink()
{
    return link;
}

void Feed::setTitle(QString _title)
{
    title = _title;
}

void Feed::setContent(QString _content)
{
    content = _content;
}

void Feed::setDate(QString _date)
{
    date = _date;
}

void Feed::setLink(QString _link)
{
    link = _link;
}
