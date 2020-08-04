#ifndef INSTANCEFORM_H
#define INSTANCEFORM_H

#include <QWidget>

#include "control.h"

namespace Ui {
class InstanceForm;
}

class InstanceForm : public QWidget
{
    Q_OBJECT

public:
    explicit InstanceForm(Instance i, QWidget *parent = nullptr);
    explicit InstanceForm(QWidget *parent = nullptr);
    ~InstanceForm();

signals:
    void stateChanged(QString name, QString state);

private slots:
    void onStart();
    void onStop();
    void onRestart();

private:
    Ui::InstanceForm *ui;
    Instance curr;
};

#endif // INSTANCEFORM_H
