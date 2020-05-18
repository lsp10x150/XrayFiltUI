#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include "about.h"
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    ui->spectrasTreeView->setModel(model);
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
    // Передать в консоль ./XrayFilt
    QString program = "XrayFilt config.dat";
    QProcess myProcess(this);
    myProcess.execute(program);
    myProcess.waitForFinished(-1);
}

void MainWindow::on_quitAction_triggered()
{
    QApplication::quit();
}

void MainWindow::on_visualisePushButton_clicked()
{
    // Вызвать в консоли исполнение ./XrayFilt config.dat vis1.mac
    QString program = "XrayFilt config.dat vis1.mac";
    QProcess *myProcess = new QProcess(this);
    myProcess->setReadChannel(QProcess::StandardOutput);
    myProcess->execute(program);
}

void MainWindow::on_chooseInitSpectraFilePushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                    tr("Выберите файл исходного спектра"),
                                                    "~/",
                                                tr("Файл исходного спектра (*.dat *.isdat)"));
    qDebug() << "Your choosen file is: " << file;
    ui->intialSpectraPathLineEdit->setText(file);
}


void MainWindow::on_iterationsGroupBox_clicked()
{
    if(!ui->iterationsGroupBox->isChecked()){
        ui->n_iterationsSpinBox->setValue(1);
        return;
    }
    ui->n_iterationsSpinBox->setValue(0);
}

void MainWindow::on_infoAction_triggered()
{
    about about;
    about.setWindowTitle("Authors");
    about.exec();

}

void MainWindow::on_actionO_GEANT4_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About GEANT4");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Данная программа разработана с использованием библиотеки GEANT4 версии 10.4.1. \n"
                   "Подробнее с GEANT4 можно ознакомиться по ссылке:");
    msgBox.setInformativeText("<a href='http://www.geant4.org/geant4/'>http://www.geant4.org/geant4/</a>");
    msgBox.exec();


}

void MainWindow::on_actionO_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_savePushButton_clicked()
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
        static const char* const FILE_NAME = "config.dat";
        QFile out( FILE_NAME );
        if( out.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &out );
            stream << ui->intialSpectraPathLineEdit->text() << "\n";
            stream << "N_PARTICLES " << ui->n_particlesSpinBox->value() << "\n";
            stream << "FILTER_WIDTH " << ui->filterWidthDoubleSpinBox->value() << "\n";
            stream << "N_ITERATIONS " << ui->n_iterationsSpinBox->value() << "\n";
            stream << "STEP_REDUCING_FILTER_WIDTH " << ui->stepDecreasingWidthDoubleSpinBox->value() << "\n";
            stream << "INHERENT_FILTRATION " << (ui->checkBox->isChecked() ? 1 : 0) << "\n";
            out.close();
            QMessageBox::information(this,"Success", "Настройки успешно применены.");
            ui->startPushButton->setEnabled(true);
            ui->visualisePushButton->setEnabled(true);
        } else {
            QMessageBox::warning(this,"Fail", "Не удалось создать/сохранить файл настроек.\nЗапуск возможен только с настройками по умолчанию.");
            ui->startPushButton->setEnabled(true);
            ui->visualisePushButton->setEnabled(true);
        }
}

void MainWindow::makePlot(QString fileName){
    QFile in(fileName);
    QVector<double> x(200), y(200);
    if( in.open( QIODevice::WriteOnly ) ) {

        QTextStream stream( &in );

        for (int i = 0; i < 200; ++i){
            stream >> x[i] >> y[i];
            qDebug() << x[i] << " " << y [i];
        }

        in.close();

        qDebug() << "Spectra has been read successfilly.";

    } else {
        QMessageBox::warning(this,"Fail", "Не удалось прочесть файл спектра.");
    }

    // create graph and assign data to it:
    ui->plotBox->addGraph();
    ui->plotBox->graph(0)->setData(x, y);
    // give the axes some labels:
    qDebug() << "makin' plot 3";
    ui->plotBox->xAxis->setLabel("Е, кэВ");
    ui->plotBox->yAxis->setLabel("Отн.ед.");
    // set axes ranges, so we see all data:

    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0, 1.25);

    ui->plotBox->replot();
}

void MainWindow::on_simulatedSpectraPushButton_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this,
                                                        tr("Выберите директорию с симулированными спектрами"),
                                                        "~/");
        qDebug() << "Your choosen dir is: " << file;
        ui->simulatedSpectraLineEdit->setText(file);
}

void MainWindow::on_simulatedSpectraLineEdit_textChanged(const QString &arg1)
{
    ui->spectrasTreeView->setWindowFilePath(arg1);
    ui->spectrasTreeView->update();
}

void MainWindow::on_spectrasTreeView_doubleClicked(const QModelIndex &index)
{

}
