#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Search feeds"));
    searchInput = new QLineEdit;
    searchButton = new QPushButton;
    searchButton->setText(tr("Search"));

    layout = new QGridLayout;
    layout->addWidget(searchInput, 0, 0);
    layout->addWidget(searchButton, 0, 1);
    setLayout(layout);
    setMinimumSize(500, 60);

    feedUrl = new QUrl;
    fileDownloader = new FileDownloader(this);
    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(searchFeed()));
    connect(fileDownloader, SIGNAL(downloaded()), this, SLOT(accept()));
}

QUrl* SearchDialog::getFeedUrl()
{
    return feedUrl;
}

QByteArray SearchDialog::getDownloadedData()
{
    return fileDownloader->getDownloadedData();
}

void SearchDialog::searchFeed()
{
    feedUrl->setUrl(searchInput->text());
    QUrl url(searchInput->text());
    fileDownloader->downloadData(url);
}
