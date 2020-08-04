#include "instanceform.h"
#include "ui_instanceform.h"

InstanceForm::InstanceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstanceForm)
{
    ui->setupUi(this);
}

InstanceForm::InstanceForm(Instance i, QWidget *parent):
    InstanceForm(parent)
{
    curr = i;
    ui->label->setText(curr.name);
    ui->status->setText(curr.status);

    ui->stop->setEnabled(curr.status==RUNNING);
    ui->start->setEnabled(curr.status==OFFLINE);
    ui->restart->setEnabled(curr.status==RUNNING);

    connect(ui->stop, &QPushButton::clicked, this, &InstanceForm::onStop);
    connect(ui->start, &QPushButton::clicked, this, &InstanceForm::onStart);
    connect(ui->restart, &QPushButton::clicked, this, &InstanceForm::onRestart);
}

InstanceForm::~InstanceForm()
{
    delete ui;
}

void InstanceForm::onStart()
{
    emit stateChanged(curr.name, RUNNING);
}

void InstanceForm::onStop()
{
    emit stateChanged(curr.name, OFFLINE);
}

void InstanceForm::onRestart()
{
    emit stateChanged(curr.name, RESTART);
}
