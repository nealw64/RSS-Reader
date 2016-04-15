#include "newsview.h"

NewsView::NewsView() {}

void NewsView::setContent(QTextBrowser *browser, QSqlQuery *query)
{
    Feed feed;
    QSqlRecord rec = query->record();
    while (query->next()) {
        feed.setTitle(query->value(rec.indexOf("title")).toString());
        feed.setDate(query->value(rec.indexOf("date")).toString());
        feed.setContent(query->value(rec.indexOf("content")).toString());
        feed.setLink(query->value(rec.indexOf("link")).toString());
    }

    browser->append("<html><head><meta http-equiv='Content-Type' content='text/html; "
                    "charset=windows-1251'><link rel='stylesheet' type='text/css' "
                    "href=':/styles/styles.css'></head><body><p><titleStyle>"
                    + feed.getTitle() + "</titleStyle></p><p><dateStyle>" + feed.getDate()
                    + "</dateStyle></p><p><contentStyle>" + feed.getContent()
                    + "</contentStyle></p><readMoreStyle><a href='" + feed.getLink() + "'>"
                    + QObject::tr("Read more here") + "</a></readMoreStyle></body></html>");
}
