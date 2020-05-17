#include "computingstatus.h"
#include "ui_computingstatus.h"

ComputingStatus::ComputingStatus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComputingStatus)
{
    ui->setupUi(this);
}

ComputingStatus::~ComputingStatus()
{
    delete ui;
}

void ComputingStatus::on_cancelPushButton_clicked()
{
    //Остановить расчёт

    //Закрыть виджет
    QWidget::close();
}
