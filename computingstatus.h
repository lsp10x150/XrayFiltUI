#ifndef COMPUTINGSTATUS_H
#define COMPUTINGSTATUS_H

#include <QDialog>

namespace Ui {
class ComputingStatus;
}

class ComputingStatus : public QDialog
{
    Q_OBJECT

public:
    explicit ComputingStatus(QWidget *parent = nullptr);
    ~ComputingStatus();

private slots:

    void on_cancelPushButton_clicked();

private:
    Ui::ComputingStatus *ui;
};

#endif // COMPUTINGSTATUS_H
