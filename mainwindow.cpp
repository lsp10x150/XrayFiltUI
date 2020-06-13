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
#include <QDir>
/// Нормировка
void Normalize(QVector<double> &y){
    double max = *std::max_element(y.begin(), y.end());
    for (int i = 0; i < 200; ++i){
        y[i] /= max; } }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Конструктор основного окна приложения
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    for(int i = 0; i < 200; ++i){
        simSpectraX.push_back(0);
        simSpectraY.push_back(0);
        wantedSpectraX.push_back(0);
        wantedSpectraY.push_back(0); }
    ui->setupUi(this);
    // Создаем модель файловой системы
    model = new QFileSystemModel;
    QString path = QString("~/");
    // Определяем исходный путь модели
    model->setRootPath(path);
    model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ui->simulatedSpectrasListView->setModel(model);
    ui->comboBox->addItem("Al");
    ui->comboBox->addItem("Cu");
    ui->comboBox->addItem("Sn");
    ui->comboBox->addItem("Zn");
    // Настройки плотБокса
    ui->plotBox->xAxis->setLabel("Е, кэВ");
    ui->plotBox->yAxis->setLabel("Отн.ед.");
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0, 1.25);
    ui->plotBox->addGraph();
    ui->plotBox->addGraph();
    ui->plotBox->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    ui->plotBox->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    ui->plotBox->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->plotBox->legend->setFont(legendFont);
    ui->plotBox->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->plotBox->graph(0)->setName("Симулированный спектр");
    ui->plotBox->graph(1)->setName("Желаемый спектр");
    ui->plotBox->graph(0)->setPen(QPen(Qt::blue));
    ui->plotBox->graph(1)->setPen(QPen(Qt::red)); }
MainWindow::~MainWindow() {
    delete ui;}
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выхода
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_quitPushButton_clicked() {
    // Выход из приложения
     QApplication::quit(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка запуска расчета
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_startPushButton_clicked() {
    // Запустить процесс ./XrayFilt с аргументом config.dat
    QString program = "XrayFilt config.dat";
    QProcess myProcess(this);
    myProcess.execute(program); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выхода из вкладки "файл"
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_quitAction_triggered() {
    QApplication::quit(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка "визуализация"
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_visualisePushButton_clicked() {
    // Запустить процесс ./XrayFilt с аргументамим config.dat vis1.mac
    QString program = "XrayFilt config.dat vis1.mac";
    // ./XrayFilt построен на основе GEANT4 - простая симуляция фильтрации рентгеновского излучения
    QProcess *myProcess = new QProcess(this);
    myProcess->execute(program); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка выбора файла с исходным спектром
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_chooseInitSpectraFilePushButton_clicked() {
    // Открывается файловый диалог, который передает в
    // initialSpectraPathLineEdit абсолютный путь к файлу исходного спектра
    QString file = QFileDialog::getOpenFileName(this,
        tr("Выберите файл исходного спектра"),
        "~/",
        tr("Файл исходного спектра (*.dat *.isdat)"));
    qDebug() << "Your choosen file is: " << file;
    ui->intialSpectraPathLineEdit->setText(file); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Чекбокс повторного расчета
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_iterationsGroupBox_clicked() {
    // Если чекнут, то включается групбокс, позволяя устанавливать число итераций
    // Если анчекнут, групбокс выключается, а число итераций сбрасывается.
    if(!ui->iterationsGroupBox->isChecked()){
        ui->n_iterationsSpinBox->setValue(1);
        ui->stepDecreasingWidthDoubleSpinBox->setValue(0.01); } }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о программе
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_infoAction_triggered() {
    // Открывается виджет about
    about about;
    about.setWindowTitle("Authors");
    about.exec(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о GEANT4
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_actionO_GEANT4_triggered() {
    // Вылетает в месседжБоксе
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About GEANT4");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Данная программа разработана с использованием библиотеки GEANT4 версии 10.4.1. \n"
                   "Подробнее с GEANT4 можно ознакомиться по ссылке:");
    msgBox.setInformativeText("<a href='http://www.geant4.org/geant4/'>http://www.geant4.org/geant4/</a>");
    msgBox.exec(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Информация о Qt
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_actionO_Qt_triggered(){
    QApplication::aboutQt(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Кнопка "сохранить" на основной вкладке
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_savePushButton_clicked() {
    // Сохраняет настройки в файл config.dat, который затем принимает ./XrayFilt для настройки симуляции
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
        static const char* const FILE_NAME = "config.dat";
        QFile out( FILE_NAME );
        if ((ui->filterWidthDoubleSpinBox->value() - ui->n_iterationsSpinBox->value() *
             ui->stepDecreasingWidthDoubleSpinBox->value()) < 0){
            int n = ui->filterWidthDoubleSpinBox->value() / ui->stepDecreasingWidthDoubleSpinBox->value();
            QString warn = "После %1 итерации толщина фильтра станет меньше нуля.\nПоправьте значение и попробуйте снова.";
            QMessageBox::warning(this,"Warning", warn.arg(n));
        } else if( out.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &out );
            int filterMaterial = 0;
            if (ui->comboBox->currentText() == "Al")
                filterMaterial = 0;
                else if (ui->comboBox->currentText() == "Cu")
                    filterMaterial = 1;
                    else if (ui->comboBox->currentText() == "Sn")
                        filterMaterial = 2;
                        else if (ui->comboBox->currentText() == "Zn")
                            filterMaterial = 3;
            stream << ui->intialSpectraPathLineEdit->text() << "\n";
            stream << "N_PARTICLES " << ui->n_particlesSpinBox->value() << "\n";
            stream << "FILTER_WIDTH " << ui->filterWidthDoubleSpinBox->value() << "\n";
            stream << "N_ITERATIONS " << ui->n_iterationsSpinBox->value() << "\n";
            stream << "STEP_REDUCING_FILTER_WIDTH " << ui->stepDecreasingWidthDoubleSpinBox->value() << "\n";
            stream << "INHERENT_FILTRATION " << (ui->checkBox->isChecked() ? 1 : 0) << "\n";
            stream << "FILTER_MATERIAL " << filterMaterial;
            QMessageBox::information(this,"Success", "Настройки успешно применены.");
            // После сохраенения настроек активируются клавиши Запуск и Визуализация
            ui->startPushButton->setEnabled(true);
            ui->visualisePushButton->setEnabled(true);
        } else {
            QMessageBox::warning(this,"Fail", "Не удалось создать/сохранить файл настроек.\n"
                                              "Запуск возможен только с настройками по умолчанию.");
            // Даже в случае неудачи кнопки Запуск и
            // Визуализация все равно активируются, поскольку в ./XrayFilt встроена
            // инициализация дефолтных настроек и симуляцию все равно можно
            // будет запустить, хотя она и будет бесполезной.
            ui->startPushButton->setEnabled(true);
            ui->visualisePushButton->setEnabled(true); }
        out.close(); }

/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Просмотр::Построение симулированного спектра в QCustomPlot
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::plotSimSpectra(QString fileName){
    QFile in(fileName); // Открываем файл
    if( in.open( QIODevice::ReadOnly ) ) { //Если получилось открыть файл,
        QTextStream stream( &in ); // то инициализируем поток
        for (int i = 0; i < 200; ++i){ // Читаем файл спектра в векторы икс и игрек
            stream >> simSpectraX[i] >> simSpectraY[i];
            simSpectraX[i] = i; }
        in.close(); // Закрываем файл
        qDebug() << "Spectra has been read successfilly.";
    } else {
        QMessageBox::warning(this,"Fail", "Не удалось прочесть файл спектра."); }
    if(ui->normalizeRadioButton->isChecked())
            Normalize(simSpectraY);
    ui->plotBox->graph(0)->setPen(QPen(Qt::blue));
    ui->plotBox->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->plotBox->graph(0)->setData(simSpectraX, simSpectraY);
    ui->plotBox->xAxis->setLabel("Е, кэВ");
    ui->normalizeRadioButton->isChecked() ?
        ui->plotBox->yAxis->setLabel("Отн.ед.") :
                ui->plotBox->yAxis->setLabel("Кол-во частиц");
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0,std::max(*std::max_element
                                            (simSpectraY.begin(),simSpectraY.end()),
                                            *std::max_element(wantedSpectraY.begin(),
                                                              wantedSpectraY.end())));
    ui->plotBox->replot(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Просмотр::Построение желаемого спектра в QCustomPlot
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::plotWantedSpectra(QString fileName){
    QFile in(fileName); // Открываем файл
    if( in.open( QIODevice::ReadOnly ) ) { //Если получилось открыть файл,
        QTextStream stream( &in ); // то инициализируем поток
        for (int i = 0; i < 200; ++i){ // Читаем файл спектра в векторы икс и игрек
            stream >> wantedSpectraX[i] >> wantedSpectraY[i]; }
        in.close(); // Закрываем файл
        qDebug() << "Wanted spectra has been read successfilly.";
    } else {
        QMessageBox::information(this,"Note", "Выберите файл желаемого спектра."); }
    if(ui->normalizeRadioButton->isChecked())
            Normalize(wantedSpectraY);
    ui->plotBox->graph(1)->setPen(QPen(Qt::red));
    ui->plotBox->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));
    ui->plotBox->graph(1)->setData(wantedSpectraX, wantedSpectraY);
    ui->plotBox->xAxis->setLabel("Е, кэВ");
    ui->normalizeRadioButton->isChecked() ?
        ui->plotBox->yAxis->setLabel("Отн.ед.") :
                ui->plotBox->yAxis->setLabel("Кол-во частиц");
    ui->plotBox->xAxis->setRange(0, 180);
    ui->plotBox->yAxis->setRange(0,std::max(*std::max_element(simSpectraY.begin(),simSpectraY.end()),
                                            *std::max_element(wantedSpectraY.begin(),wantedSpectraY.end())));
    ui->plotBox->replot(); }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Просмотр::кнопка выбора директории со спектрами
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_simulatedSpectraPushButton_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this,
        tr("Выберите директорию с симулированными спектрами"),
        "~/");
    qDebug() << "Your choosen dir is: " << dir;
    ui->simulatedSpectraLineEdit->setText(dir);
    ui->simulatedSpectrasListView->setRootIndex(model->setRootPath(dir)); }
void MainWindow::on_simulatedSpectrasListView_doubleClicked(const QModelIndex &index) {
    if(model->fileInfo(index).isFile()){
        plotSimSpectra(model->fileInfo(index).absoluteFilePath());
        plotWantedSpectra(wantedSpectraPath);
        simSpectra = model->fileInfo(index).absoluteFilePath(); } }
void MainWindow::on_normalizeRadioButton_clicked() {
    plotSimSpectra(simSpectra);
    plotWantedSpectra(wantedSpectraPath); }
void MainWindow::on_wantedSpectrPushButton_clicked() {
    QString file = QFileDialog::getOpenFileName(this,
        tr("Выберите файл желаемого спектра"),
        "~/",
        tr("Файл спектра (*.dat)"));
    qDebug() << "Your choosen file is: " << file;
    ui->wantedSpectraLineEdit->setText(file); }
void MainWindow::on_wantedSpectraLineEdit_textChanged(const QString &arg1) {
    plotWantedSpectra(arg1);
    wantedSpectraPath = arg1; }
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
/// Просмотр::Поиск оптимального фильтра
/// ооооОООооооОООооооОООооооОООооооОООооооОООооо
void MainWindow::on_findOptimalFilterPushButton_clicked() {
    QDir dir;
    dir.setFilter(QDir::Files);
    dir.cd(simSpectraPath);
    QVector<QString> paths;
    sumSquareErrors.clear();
    for (QFileInfo temp : dir.entryInfoList()){
        paths.push_back(QString(temp.absoluteFilePath())); }
    for (QString path : paths){
        QFile in(path); // Открываем файл
        if( in.open( QIODevice::ReadOnly ) ) { //Если получилось открыть файл,
            QTextStream stream( &in ); // то инициализируем поток
            QVector<double> x(200), y(200);
            double sum = 0;
            for (int i = 0; i < 200; ++i){ // Читаем файл спектра в векторы икс и игрек
                stream >> x[i] >> y[i]; }
            in.close(); // Закрываем файл
            QVector<double> fakeWantedSpectraY = wantedSpectraY;
            Normalize(y);
            Normalize(fakeWantedSpectraY);
            for (int i = 0; i < 200; ++i){ //
                sum += (y[i] - fakeWantedSpectraY[i])*(y[i] - fakeWantedSpectraY[i]); }
            sumSquareErrors.insert(path, sum); }  }
    auto minElement = std::min_element(sumSquareErrors.begin(), sumSquareErrors.end());
    QMessageBox::information(this, "Optimal filter", "Профиль спектра в файле: \n" +
                             QFileInfo(minElement.key()).fileName() + "\nНаиболее похож на профиль желаемого."); }
void MainWindow::on_simulatedSpectraLineEdit_textChanged(const QString &arg1) {
    simSpectraPath = arg1; }
