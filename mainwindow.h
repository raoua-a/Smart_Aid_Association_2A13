#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPieSlice>
#include <QPieSeries>
#include <QtCharts>
#include <QMainWindow>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTableView>
#include<Qprinter>
#include "enfant.h"
#include "ui_mainwindow.h"


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
    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_label_2_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    enfant tmpenfant;

};
#endif // MAINWINDOW_H