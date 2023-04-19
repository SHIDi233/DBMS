#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sqlanalysis.h"
#include "control.h"
#include<QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    user.createDb("testDB");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_5_clicked()
{
//    SqlAnalysis s;
//    s.parse_sql(ui->textEdit_3->toPlainText());

    QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    db = new DB("testDB",true,"E:\\ST\\qt\\DBMS\\test\\data\\testDB",current_date);

    db->createTable("6");
}

