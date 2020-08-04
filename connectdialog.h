#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QString host, int port, QString user, QString passwd, QWidget *parent = nullptr);
    ~ConnectDialog();

    QString user();
    QString password();
    QString host();
    int port();

private slots:
    void onReveal(bool checked = false);

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
