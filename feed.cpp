#include "feed.h"

Feed::Feed() {}

Feed::Feed(QString title, QString category, QString content, QString date, QString link)
{
    this->title = title;
    this->category = category;
    this->content = content;
    this->date = date;
    this->link = link;
}

QString Feed::getTitle()
{
    return title;
}

QString Feed::getCategory()
{
    return category;
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

void Feed::setTitle(QString title)
{
    this->title = title;
}

void Feed::setCategory(QString category)
{
    this->category = category;
}

void Feed::setContent(QString content)
{
    this->content = content;
}

void Feed::setDate(QString date)
{
    this->date = date;
}

void Feed::setLink(QString link)
{
    this->link = link;
}
