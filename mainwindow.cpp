#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sqlanalysis.h"
#include "control.h"
#include<QDateTime>
#include<QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    user.createDb("testDB");

//    loadDB();
//    db = dbs[0];


    //test for ui
    QStandardItemModel *model = new QStandardItemModel(this);

    //设置表头隐藏
    //ui->treeView->setHeaderHidden(true);

    //设置表头
    model->setHorizontalHeaderLabels(QStringList()<<"Manage");

    //设置model
    ui->treeView->setModel(model);

    //设置展开
    ui->treeView->expandAll();

        QStandardItem *item1 = new QStandardItem("DB1");

        QStandardItem *item00 = new QStandardItem("student");
        QStandardItem *item10 = new QStandardItem("course");
        QStandardItem *item20 = new QStandardItem("sc");

        QStandardItem *item001 = new QStandardItem("sno");
        QStandardItem *item002 = new QStandardItem("sname");
        QStandardItem *item003 = new QStandardItem("classno");

        item1->setChild(0,0,item00);
        item1->setChild(1,0,item10);
        item1->setChild(2,0,item20);
        item00->setChild(0,0,item001);
        item00->setChild(1,0,item002);
        item00->setChild(2,0,item003);

        model->setItem(0,0,item1);


        QStandardItem *item2 = new QStandardItem("DB2");

        QStandardItem *item200 = new QStandardItem("data");


        QStandardItem *item202 = new QStandardItem("dname");
        QStandardItem *item212 = new QStandardItem("ctime");
        QStandardItem *item222 = new QStandardItem("dd");

        item2->setChild(0,0,item200);
        item200->setChild(0,0,item202);
        item200->setChild(1,0,item212);
        item200->setChild(2,0,item222);

        model->setItem(1,0,item2);
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

