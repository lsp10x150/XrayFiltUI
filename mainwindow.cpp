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
#include <algorithm>

/// Нормировка
void Normalize(QVector<double> &y){
    double max = *std::max_element(y.begin(), y.end());
    for (int i = 0; i < 200; ++i){
        y[i] /= max;
    }
}


/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Конструктор основного окна приложения
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    for(int i = 0; i < 200; ++i){
        simSpectraX.push_back(0);
        simSpectraY.push_back(0);
        wantedSpectraX.push_back(0);
        wantedSpectraY.push_back(0);
    }
    qDebug() << simSpectraX.capacity()<< simSpectraX.size();
    ui->setupUi(this);
    model = new QFileSystemModel; // Создаем модель файловой системы
    QString path = QString("~/");
    model->setRootPath(path); // Определяем исходный путь модели
    model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ui->simulatedSpectrasListView->setModel(model);

    /// Настройки плотБокса
    ui->plotBox->xAxis->setLabel("Е, кэВ");
    ui->plotBox->yAxis->setLabel("Отн.ед.");
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0, 1.25);
    ui->plotBox->addGraph();
    ui->plotBox->addGraph();
    ui->plotBox->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->plotBox->legend->setFont(legendFont);
    ui->plotBox->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->plotBox->graph(0)->setName("Симулированный спектр");
    ui->plotBox->graph(1)->setName("Желаемый спектр");
    ui->plotBox->graph(0)->setPen(QPen(Qt::blue));
    ui->plotBox->graph(1)->setPen(QPen(Qt::red));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выхода
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_quitPushButton_clicked()
{
    QApplication::quit(); // Выход из приложения
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка запуска расчета
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_startPushButton_clicked()
{
    // Запустить процесс ./XrayFilt с аргументом config.dat
    QString program = "XrayFilt config.dat";
    QProcess myProcess(this);
    myProcess.execute(program);
    //myProcess.waitForFinished(-1); // Ждать завершения не нужно при использовании команды execute()
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выхода из вкладки "файл"
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_quitAction_triggered()
{
    QApplication::quit();
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка "визуализация"
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_visualisePushButton_clicked()
{
    // Запустить процесс ./XrayFilt с аргументамим config.dat vis1.mac
    QString program = "XrayFilt config.dat vis1.mac";
    QProcess *myProcess = new QProcess(this); // ./XrayFilt построен на основе GEANT4 - простая симуляция фильтрации рентгеновского излучения
    myProcess->setReadChannel(QProcess::StandardOutput); // Можно убрать, все равно ничего не читаю
    myProcess->execute(program);
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выбора файла с исходным спектром
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_chooseInitSpectraFilePushButton_clicked()
{
    // Открывается файловый диалог, который передает в initialSpectraPathLineEdit абсолютный путь к файлу исходного спектра
    QString file = QFileDialog::getOpenFileName(this,
                                                    tr("Выберите файл исходного спектра"),
                                                    "~/",
                                                tr("Файл исходного спектра (*.dat *.isdat)"));
    qDebug() << "Your choosen file is: " << file;
    ui->intialSpectraPathLineEdit->setText(file);
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Чекбокс повторного расчета
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_iterationsGroupBox_clicked()
{
    // Если чекнут, то включается групбокс, позволяя устанавливать число итераций
    // Если анчекнут, групбокс выключается, а число итераций сбрасывается.
    if(!ui->iterationsGroupBox->isChecked()){
        ui->n_iterationsSpinBox->setValue(1);
        ui->stepDecreasingWidthDoubleSpinBox->setValue(0.01);
    }
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о программе
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_infoAction_triggered()
{
    // Открывается виджет about
    about about;
    about.setWindowTitle("Authors");
    about.exec();

}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о GEANT4
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_actionO_GEANT4_triggered()
{
    // Вылетает в месседжБоксе
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About GEANT4");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Данная программа разработана с использованием библиотеки GEANT4 версии 10.4.1. \n"
                   "Подробнее с GEANT4 можно ознакомиться по ссылке:");
    msgBox.setInformativeText("<a href='http://www.geant4.org/geant4/'>http://www.geant4.org/geant4/</a>");
    msgBox.exec();
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о Qt
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_actionO_Qt_triggered()
{
    QApplication::aboutQt();
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка "сохранить" на основной вкладке
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_savePushButton_clicked()
{
    // Сохраняет настройки в файл config.dat, который затем принимает ./XrayFilt для настройки симуляции
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
            ui->startPushButton->setEnabled(true); // После сохраенения настроек активируются клавиши Запуск и Визуализация
            ui->visualisePushButton->setEnabled(true);
        } else {
            QMessageBox::warning(this,"Fail", "Не удалось создать/сохранить файл настроек.\nЗапуск возможен только с настройками по умолчанию.");
            ui->startPushButton->setEnabled(true); // Даже в случае неудачи кнопки Запуск и Визуализация все равно активируются, поскольку в ./XrayFilt встроена
            // инициализация дефолтных настроек и симуляцию все равно можно будет запустить, хотя она и будет бесполезной.
            ui->visualisePushButton->setEnabled(true);
        }
}

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Симуляция::Построение графиков в QCustomPlot
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::plotSimSpectra(QString fileName){
    QFile in(fileName); // Открываем файл
    if( in.open( QIODevice::ReadOnly ) ) { //Если получилось открыть файл,

        QTextStream stream( &in ); // то инициализируем поток
        qDebug() << simSpectraX.capacity()<< simSpectraX.size();
        for (int i = 0; i < 200; ++i){ // Читаем файл спектра в векторы икс и игрек
            stream >> simSpectraX[i] >> simSpectraY[i];
            simSpectraX[i] = i;
            qDebug() << simSpectraX[i] << " " << simSpectraY[i];
        }

        in.close(); // Закрываем файл
        qDebug() << "Spectra has been read successfilly.";

    } else {
        QMessageBox::warning(this,"Fail", "Не удалось прочесть файл спектра.");
    }

    if(ui->normalizeRadioButton->isChecked())
            Normalize(simSpectraY);

    // create graph and assign data to it:
    //ui->plotBox->addGraph();
    ui->plotBox->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->plotBox->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->plotBox->graph(0)->setData(simSpectraX, simSpectraY);
    // give the axes some labels:
    ui->plotBox->xAxis->setLabel("Е, кэВ");

    ui->normalizeRadioButton->isChecked() ?
        ui->plotBox->yAxis->setLabel("Отн.ед.") :
                ui->plotBox->yAxis->setLabel("Кол-во частиц");

    // set axes ranges, so we see all data:
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0,std::max(*std::max_element(simSpectraY.begin(),simSpectraY.end()), *std::max_element(wantedSpectraY.begin(),wantedSpectraY.end())));
    ui->plotBox->replot();
}

void MainWindow::plotWantedSpectra(QString fileName){
    QFile in(fileName); // Открываем файл
    if( in.open( QIODevice::ReadOnly ) ) { //Если получилось открыть файл,

        QTextStream stream( &in ); // то инициализируем поток

        for (int i = 0; i < 200; ++i){ // Читаем файл спектра в векторы икс и игрек
            stream >> wantedSpectraX[i] >> wantedSpectraY[i];

            qDebug() << wantedSpectraX[i] << " " << wantedSpectraY[i];
        }

        in.close(); // Закрываем файл
        qDebug() << "Spectra has been read successfilly.";

    } else {
        QMessageBox::warning(this,"Fail", "Не удалось прочесть файл спектра.");
    }

    if(ui->normalizeRadioButton->isChecked())
            Normalize(wantedSpectraY);

    // create graph and assign data to it:
    //ui->plotBox->addGraph();
    ui->plotBox->graph(1)->setPen(QPen(Qt::red)); // line color blue for first graph
    ui->plotBox->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20))); // first graph will be filled with translucent blue
    ui->plotBox->graph(1)->setData(wantedSpectraX, wantedSpectraY);
    // give the axes some labels:
    ui->plotBox->xAxis->setLabel("Е, кэВ");

    ui->normalizeRadioButton->isChecked() ?
        ui->plotBox->yAxis->setLabel("Отн.ед.") :
                ui->plotBox->yAxis->setLabel("Кол-во частиц");

    // set axes ranges, so we see all data:
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0,std::max(*std::max_element(simSpectraY.begin(),simSpectraY.end()), *std::max_element(wantedSpectraY.begin(),wantedSpectraY.end())));
    ui->plotBox->replot();
}


/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Симуляция::кнопка выбора директории со спектрами
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_simulatedSpectraPushButton_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this,
                                                        tr("Выберите директорию с симулированными спектрами"),
                                                        "~/");
    qDebug() << "Your choosen dir is: " << file;
    ui->simulatedSpectraLineEdit->setText(file);
    ui->simulatedSpectrasListView->setRootIndex(model->setRootPath(file));
}


void MainWindow::on_simulatedSpectrasListView_doubleClicked(const QModelIndex &index)
{
    if(model->fileInfo(index).isFile()){
        simSpectraPath = model->fileInfo(index).absoluteFilePath();
        plotSimSpectra(simSpectraPath);
        plotWantedSpectra(wantedSpectraPath);
    }
}

void MainWindow::on_normalizeRadioButton_clicked()
{
    plotSimSpectra(simSpectraPath);
    plotWantedSpectra(wantedSpectraPath);

}

void MainWindow::on_wantedSpectrPushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Выберите файл желаемого спектра"),
                                                "~/",
                                                tr("Файл спектра (*.dat)"));
    qDebug() << "Your choosen dir is: " << file;
    ui->wantedSpectraLineEdit->setText(file);
}

void MainWindow::on_wantedSpectraLineEdit_textChanged(const QString &arg1)
{
    plotWantedSpectra(arg1);
    wantedSpectraPath = arg1;
}

void MainWindow::on_findOptimalFilterPushButton_clicked()
{

}
