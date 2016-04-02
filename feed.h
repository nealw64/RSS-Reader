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

    void setTitle(QString _title);
    void setContent(QString _content);
    void setCategory(QString _category);
    void setDate(QString _date);
    void setLink(QString _link);

private:
    QString title;
    QString category;
    QString content;
    QString date;
    QString link;
};

#endif // FEED_H
