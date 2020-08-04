#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QLabel>

#include "control.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   void onReplyFinished();
   void onUpdateFinished();
   void onReplyError(QNetworkReply::NetworkError err);

    void onConnect();
    void onDisconnect();
    void onRefresh();
    void onExit();
    void onAbout();
    void onControl(QString name, QString state);
    void authRequired(QNetworkReply *reply, QAuthenticator *auth);

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QLabel *labHost;
    QLabel *labPort;

    Credential current;

    QNetworkReply *reply;
    QNetworkAccessManager *http;

    void quit();
    void clear();
    void createPanel(Instance curr);
    void toggleActions(bool connected = true);

    void retrieve();

    void readSettings();
    void writeSettings();
};
#endif // MAINWINDOW_H
