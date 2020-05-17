#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "computingstatus.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitPushButton_clicked()
{
    QApplication::quit(); // Выход из приложения
}

void MainWindow::on_startPushButton_clicked()
{
    ComputingStatus loading;
    loading.setWindowTitle("Loading");
    // Передать в консоль ./XrayFilt

    // Ловим от XrayFilt сведения о статусе счета

    // По заполнении loadingBar закрываем виджет

    loading.exec();
}

void MainWindow::on_quitAction_triggered()
{
    QApplication::quit();
}
