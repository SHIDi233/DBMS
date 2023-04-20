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

    loadDB();
    db = dbs[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadDB() {

    //加载数据库
    //创建文件操作对象
    bool isRead = true;
    QFile dbFile(rootPath.absoluteFilePath("ruanko.db"));
    if(!dbFile.open(QIODevice::ReadOnly)) { isRead = false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[DBBYTE];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, DBBYTE);
        DB *d = new DB();
        d->deSerialize(buf);
        dbs.append(d);
    }

    return isRead;
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

