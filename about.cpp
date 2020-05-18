#include "about.h"
#include "ui_about.h"
#include <QPixmap>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    QPixmap vniiimtLogo(":/VNIIIMTLogo.png",".png");
    ui->VNIIIMTLogoLabel->setPixmap(vniiimtLogo);
    ui->VNIIIMTLogoLabel->setScaledContents(true);
}

about::~about()
{
    delete ui;
}

void about::on_closeAboutPushButton_clicked()
{
    QWidget::close();
}
