#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Search feeds"));
    searchInput = new QLineEdit;
    searchButton = new QPushButton;
    searchButton->setText(tr("Search"));
    messageLabel = new QLabel("Enter the link on RSS-Channel");

    gridLayout = new QGridLayout;
    gridLayout->addWidget(searchInput, 0, 0);
    gridLayout->addWidget(searchButton, 0, 1);
    gridLayout->addWidget(messageLabel, 1, 0);
    setLayout(gridLayout);
    setMinimumSize(350, 60);

    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(isValidUrl()));
    connect(searchInput, SIGNAL(returnPressed()), this, SLOT(isValidUrl()));
    connect(this, SIGNAL(validUrl()), this, SLOT(searchFeed()));
    connect(&fileDownloader, SIGNAL(downloaded()), this, SLOT(accepted()));
}

SearchDialog::~SearchDialog()
{
    delete searchInput;
    delete searchButton;
    delete messageLabel;
    delete gridLayout;
}

void SearchDialog::isValidUrl()
{
    QString regex;
    regex = "^(http|https|ftp):\\/\\/[a-z0-9]+([-.]{1}[a-z0-9]+)*.[a-z]{2,5}(([0-9]{1,5})?\\/?.*)$";
    QRegExp validUrlRegex(regex);
    if (validUrlRegex.exactMatch(searchInput->text())) {
        messageLabel->setText(tr("All right. URL is valid."));
        emit validUrl();
    } else {
        messageLabel->setText(tr("Invalid URL. Please, enter the valid URL."));
    }
}

QUrl SearchDialog::getFeedUrl()
{
    return feedUrl;
}

QByteArray SearchDialog::getDownloadedData()
{
    return fileDownloader.getDownloadedData();
}

void SearchDialog::searchFeed()
{
    progressDialog = new QProgressDialog(tr("Downloading feeds..."), 0, 0, 100, this);
    progressDialog->setWindowTitle(tr("Downloading"));

    feedUrl.setUrl(searchInput->text());
    fileDownloader.addUrlForDownload(feedUrl);
    progressDialog->show();
    fileDownloader.downloadData();

    connect(&fileDownloader, SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(updateDownloadProgress(qint64, qint64)));
}

void SearchDialog::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void SearchDialog::accepted()
{
    progressDialog->close();
    delete progressDialog;
    accept();
}
