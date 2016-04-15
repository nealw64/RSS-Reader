#ifndef NEWSVIEW_H
#define NEWSVIEW_H

#include <QTextBrowser>
#include <QObject>
#include <QtSql>
#include "feed.h"

class NewsView
{
public:
    NewsView();
    void static setContent(QTextBrowser *browser, QSqlQuery *query);
};

#endif // NEWSVIEW_H
