#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_quitPushButton_clicked();

    void on_startPushButton_clicked();

    void on_quitAction_triggered();

    void on_visualisePushButton_clicked();

    void on_chooseInitSpectraFilePushButton_clicked();

    void on_iterationsGroupBox_clicked();

    void on_infoAction_triggered();

    void on_actionO_GEANT4_triggered();

    void on_actionO_Qt_triggered();

    void on_savePushButton_clicked();

    void plotSimSpectra(QString);

    void plotWantedSpectra(QString);

    void on_simulatedSpectraPushButton_clicked();

    void on_simulatedSpectrasListView_doubleClicked(const QModelIndex &index);

    void on_normalizeRadioButton_clicked();

    void on_wantedSpectrPushButton_clicked();

    void on_wantedSpectraLineEdit_textChanged(const QString &arg1);

    void on_findOptimalFilterPushButton_clicked();

    void on_simulatedSpectraLineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QModelIndex index;
    QModelIndexList indexList;
    QString simSpectraPath;
    QString simSpectra;
    QString wantedSpectraPath;
    QVector<double> simSpectraX, simSpectraY;
    QVector<double> wantedSpectraX, wantedSpectraY;
    QMap<QString, double> sumSquareErrors;
};
#endif // MAINWINDOW_H
