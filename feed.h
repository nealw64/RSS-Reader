#ifndef FEED_H
#define FEED_H

#include <QString>

class Feed
{
public:
    Feed();
    Feed(QString _title, QString _category, QString _content, QString _date, QString _link);

    QString getTitle();
    QString getCategory();
    QString getContent();
    QString getDate();
    QString getLink();

    void setTitle(QString title);
    void setContent(QString content);
    void setCategory(QString category);
    void setDate(QString date);
    void setLink(QString link);

private:
    QString title;
    QString category;
    QString content;
    QString date;
    QString link;
};

#endif // FEED_H
