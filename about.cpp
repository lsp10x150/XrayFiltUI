#include "about.h"
#include "ui_about.h"
#include <QPixmap>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    QPixmap logo;
    //logo.
}

about::~about()
{
    delete ui;
}
