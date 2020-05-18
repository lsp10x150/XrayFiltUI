#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("XrayFiltUI"); //Название окна
    w.show();
    return a.exec();
}
