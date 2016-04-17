#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T18:35:10
#
#-------------------------------------------------

QT       += core\
            gui\
            sql\
            network\
            xml\

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RSS-Reader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        xmlparser.cpp \
        feed.cpp \
        filedownloader.cpp \
        newsview.cpp \
        addfeedsdialog.cpp

HEADERS  += mainwindow.h \
            xmlparser.h \
            feed.h \
            filedownloader.h \
            newsview.h \
            addfeedsdialog.h

FORMS    += mainwindow.ui

RESOURCES += images.qrc\
             styles.qrc
