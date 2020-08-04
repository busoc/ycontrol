#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QUrlQuery>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QSettings>

#include "connectdialog.h"
#include "instanceform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onConnect);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::onDisconnect);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::onRefresh);

    setFixedSize(QSize(960, 540));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    ui->content->setLayout(layout);

    labPort = new QLabel;
    labHost = new QLabel;
    statusBar()->addWidget(labHost);
    statusBar()->addWidget(labPort);

    readSettings();
    toggleActions(false);

    http = new QNetworkAccessManager();
    connect(http, &QNetworkAccessManager::authenticationRequired, this, &MainWindow::authRequired);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::retrieve()
{
    QUrl url;
    url.setScheme(SCHEME);
    url.setHost(current.host);
    url.setPort(current.port);
    url.setPath(API);

    reply = http->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &MainWindow::onReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &MainWindow::onReplyError);
}

void MainWindow::onReplyFinished()
{
    reply->deleteLater();
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isNull() || !doc.isObject()) {
        return;
    }
    QJsonObject root = doc.object();
    QJsonArray array = root["instance"].toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        auto curr = Instance{
            .name = obj["name"].toString(),
            .status = obj["state"].toString()
        };
        createPanel(curr);
    }
}

void MainWindow::onUpdateFinished()
{
    reply->deleteLater();
    reply->close();
    clear();
    retrieve();
//    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
//    if (doc.isNull() || !doc.isObject()) {
//        return ;
//    }
//    QJsonObject root = doc.object();
//    Q_UNUSED(root)
}

void MainWindow::onReplyError(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err)
    QMessageBox::warning(this, "request error", "request to remote server failed");
}

void MainWindow::authRequired(QNetworkReply *resp, QAuthenticator *auth)
{
    Q_UNUSED(resp)
    auth->setUser(current.user);
    auth->setPassword(current.passwd);
}

void MainWindow::onControl(QString name, QString state)
{
    QUrlQuery query;
    query.addQueryItem("state", state);

    QUrl url;
    url.setScheme(SCHEME);
    url.setHost(current.host);
    url.setPort(current.port);
    url.setPath(QString("%1%2").arg(API, name));
    url.setQuery(query);

    QString question = QString("You are about to modify the state of instance %1 to %2. Do you want to continue?");
    int code = QMessageBox::information(this, "confirm", question.arg(name, state), QMessageBox::Yes|QMessageBox::No);
    if (code == QMessageBox::No) {
        return;
    }

    ProgressDialog *dialog = new ProgressDialog(url, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    QByteArray body;
    reply = http->post(req, body);
    connect(reply, &QNetworkReply::errorOccurred, this, &MainWindow::onReplyError);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::onUpdateFinished);

    connect(reply, &QNetworkReply::downloadProgress, dialog, &ProgressDialog::replyProgress);
    connect(reply, &QNetworkReply::finished, dialog, &ProgressDialog::hide);
    dialog->show();
}

void MainWindow::onConnect()
{
    auto dialog = new ConnectDialog(current.host, current.port, current.user, current.passwd, this);
    int code = dialog->exec();
    if (code == QDialog::DialogCode::Rejected) {
        return;
    }
    http->clearAccessCache();

    current.host = dialog->host();
    current.port = dialog->port();
    current.user = dialog->user();
    current.passwd = dialog->password();

    labHost->setText(current.host);
    labPort->setNum(current.port);

    toggleActions(true);
    retrieve();
}

void MainWindow::onDisconnect()
{
    clear();
    current.host = "";
    current.port = 0;
    current.user = "";
    current.passwd = "";

    labPort->setText("N/A");
    labHost->setText("N/A");

    toggleActions(false);
}

void MainWindow::onRefresh()
{
    clear();
    retrieve();
}

void MainWindow::onExit()
{
    quit();
}

void MainWindow::onAbout()
{

}

void MainWindow::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    writeSettings();
}

void MainWindow::toggleActions(bool connected)
{
    ui->actionDisconnect->setEnabled(connected);
    ui->actionRefresh->setEnabled(connected);
    ui->actionConnect->setEnabled(!connected);
}

void MainWindow::createPanel(Instance curr)
{
    auto panel = new InstanceForm(curr);
    ui->content->layout()->addWidget(panel);

    connect(panel, &InstanceForm::stateChanged, this, &MainWindow::onControl);
}

void MainWindow::clear()
{
    auto layout = ui->content->layout();
    while (QLayoutItem *item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
}

void MainWindow::quit()
{
    writeSettings();
    QApplication::quit();
}

void MainWindow::readSettings()
{
    QSettings settings;
    current.host = settings.value("host").toString();
    current.port = settings.value("port").toInt();
    current.user = settings.value("user").toString();
    current.passwd = settings.value("passwd").toString();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("host", current.host);
    settings.setValue("port", current.port);
    settings.setValue("user", current.user);
    settings.setValue("passwd", current.passwd);
}

ProgressDialog::ProgressDialog(const QUrl &url, QWidget *parent)
  : QProgressDialog(parent)
{
    setWindowTitle("Request in progress");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(QString("Waiting from %1.").arg(url.toDisplayString()));
    setMinimum(0);
    setMaximum(0);
    setValue(0);
    setMinimumDuration(0);
    setFixedSize(QSize(400, 75));
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::replyProgress(qint64 read, qint64 total)
{
    setMaximum(total);
    setValue(read);
}
