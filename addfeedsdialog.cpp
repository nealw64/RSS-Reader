#include "addfeedsdialog.h"

AddFeedsDialog::AddFeedsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Add feeds"));
    linkInput = new QLineEdit;
    addButton = new QPushButton;
    addButton->setText(tr("Add"));
    messageLabel = new QLabel("Enter the link on RSS-Channel");

    gridLayout = new QGridLayout;
    gridLayout->addWidget(linkInput, 0, 0);
    gridLayout->addWidget(addButton, 0, 1);
    gridLayout->addWidget(messageLabel, 1, 0);
    setLayout(gridLayout);
    setMinimumSize(400, 60);

    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(isValidUrl()));
    connect(linkInput, SIGNAL(returnPressed()), this, SLOT(isValidUrl()));
    connect(this, SIGNAL(validUrl()), this, SLOT(addFeeds()));
    connect(&fileDownloader, SIGNAL(downloaded()), this, SLOT(accepted()));
    connect(&fileDownloader, SIGNAL(replyError(QString)), this, SLOT(addFeedsFailed(QString)));
}

AddFeedsDialog::~AddFeedsDialog()
{
    delete linkInput;
    delete addButton;
    delete messageLabel;
    delete gridLayout;
}

void AddFeedsDialog::isValidUrl()
{
    QString regex;
    regex = "^(http|https|ftp):\\/\\/[a-z0-9]+([-.]{1}[a-z0-9]+)*.[a-z]{2,5}(([0-9]{1,5})?\\/?.*)$";
    QRegExp validUrlRegex(regex);
    if (validUrlRegex.exactMatch(linkInput->text())) {
        emit validUrl();
    } else {
        messageLabel->setText(tr("Invalid feed-adress. Please, enter the valid feed-adress."));
    }
}

QUrl AddFeedsDialog::getFeedsUrl()
{
    return feedUrl;
}

QByteArray AddFeedsDialog::getDownloadedData()
{
    return fileDownloader.getDownloadedData();
}

void AddFeedsDialog::addFeeds()
{
    progressDialog = new QProgressDialog(tr("Downloading feeds..."), 0, 0, 100, this);
    progressDialog->setWindowTitle(tr("Downloading"));

    feedUrl.setUrl(linkInput->text());
    fileDownloader.addUrlForDownload(feedUrl);
    fileDownloader.downloadData();
    progressDialog->show();

    connect(&fileDownloader, SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(updateDownloadProgress(qint64, qint64)));
}

void AddFeedsDialog::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void AddFeedsDialog::addFeedsFailed(QString error)
{
    progressDialog->close();
    QMessageBox::warning(this, tr("Error"), error, QMessageBox::Ok);
}

void AddFeedsDialog::accepted()
{
    progressDialog->close();
    accept();
}
