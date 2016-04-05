#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Search feeds"));
    searchInput = new QLineEdit;
    searchButton = new QPushButton;
    searchButton->setText(tr("Search"));
    messageLabel = new QLabel;

    gridLayout = new QGridLayout;
    gridLayout->addWidget(searchInput, 0, 0);
    gridLayout->addWidget(searchButton, 0, 1);
    gridLayout->addWidget(messageLabel);
    setLayout(gridLayout);
    setMinimumSize(500, 60);

    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(isValidUrl()));
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
    QRegExp validUrlRegex("^(http|https|ftp):\\/\\/[a-z0-9]+([-.]{1}[a-z0-9]+)*.[a-z]{2,5}(([0-9]{1,5})?\\/?.*)$");

    if (validUrlRegex.exactMatch(searchInput->text())){
        messageLabel->setText(tr("All right. URL is valid."));
        emit validUrl();
    }
    else {
        messageLabel->setText(tr("Invalid URL"));
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
    progressDialog->setWindowModality(Qt::WindowModal);

    feedUrl.setUrl(searchInput->text());
    QUrl url(searchInput->text());
    progressDialog->show();
    fileDownloader.downloadData(url);

    connect(&fileDownloader, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDownloadProgress(qint64, qint64)));
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
