#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QString host, int port, QString user, QString passwd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->username->setText(user);
    ui->password->setText(passwd);
    ui->hostname->setText(host);
    ui->port->setValue(port);

    connect(ui->reveal, &QCheckBox::clicked, this, &ConnectDialog::onReveal);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::onReveal(bool checked)
{
    if (checked) {
        ui->password->setEchoMode(QLineEdit::EchoMode::Normal);
    } else {
        ui->password->setEchoMode(QLineEdit::EchoMode::Password);
    }
}

QString ConnectDialog::user()
{
    return ui->username->text();
}

QString ConnectDialog::password()
{
    return ui->password->text();
}

QString ConnectDialog::host()
{
    return ui->hostname->text();
}

int ConnectDialog::port()
{
    return ui->port->value();
}
