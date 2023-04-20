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
    //user.createDb("testDB");

    //loadDB();
    user.loadDB();
    db = user.getDB("testDB");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_5_clicked()
{
    QString tableName = ui->lineEdit->text();
    if(tableName.isEmpty()) { return; }
    db->createTable(tableName);
}


void MainWindow::on_pushButton_6_clicked()
{
    QString tableName = ui->lineEdit->text();
    if(tableName.isEmpty()) { return; }
    QString columnName = ui->lineEdit_2->text();
    if(columnName.isEmpty()) { return; }
    db->addColumn(tableName, columnName, TYPE::INTEGER, 4);
}

