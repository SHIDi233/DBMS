#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sqlanalysis.h"
#include "control.h"
#include<QDateTime>
#include<QStandardItemModel>
#include<QVector>
#include<QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //user.createDb("testDB");

    user.loadDB();

    showList();

    db = user.getDB("testDB");





    //test for ui
        QStandardItemModel *model = new QStandardItemModel(this);

        //设置表头隐藏
        //ui->treeView->setHeaderHidden(true);

        //设置表头
//        model->setHorizontalHeaderLabels(QStringList()<<"Manage");

//        //设置model
//        ui->treeView->setModel(model);

//        //设置展开
//        ui->treeView->expandAll();

//            QStandardItem *item1 = new QStandardItem("DB1");

//            QStandardItem *item00 = new QStandardItem("student");
//            QStandardItem *item10 = new QStandardItem("course");
//            QStandardItem *item20 = new QStandardItem("sc");

//            QStandardItem *item001 = new QStandardItem("sno");
//            QStandardItem *item002 = new QStandardItem("sname");
//            QStandardItem *item003 = new QStandardItem("classno");

//            item1->setChild(0,0,item00);
//            item1->setChild(1,0,item10);
//            item1->setChild(2,0,item20);
//            item00->setChild(0,0,item001);
//            item00->setChild(1,0,item002);
//            item00->setChild(2,0,item003);

//            model->setItem(0,0,item1);


//            QStandardItem *item2 = new QStandardItem("DB2");

//            QStandardItem *item200 = new QStandardItem("data");


//            QStandardItem *item202 = new QStandardItem("dname");
//            QStandardItem *item212 = new QStandardItem("ctime");
//            QStandardItem *item222 = new QStandardItem("dd");

//            item2->setChild(0,0,item200);
//            item200->setChild(0,0,item202);
//            item200->setChild(1,0,item212);
//            item200->setChild(2,0,item222);

//            model->setItem(1,0,item2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::mousePressEvent(QMouseEvent *e)
//{
//    if(e->button()==Qt::LeftButton)
//        clickPos=e->pos();
//}
//void MainWindow::mouseMoveEvent(QMouseEvent *e)
//{
//    if(e->buttons()&Qt::LeftButton  //左键点击并且移动
//            && e->pos().x()>=0      //范围在窗口的上面部分
//            && e->pos().y()>=0
//            && e->pos().x()<= geometry().width()
//            && e->pos().y() <= geometry().height()/10)
//    {
//        move(e->pos()+pos()-clickPos);  //移动窗口
//    }
//}


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

    showList();
}


void MainWindow::showList(){
    QStandardItemModel *model = new QStandardItemModel(this);
    QVector<DB*> dbs = user.getDbs();
    int i=0;
    for(DB* temp : dbs){
        QStandardItem *db = new QStandardItem(temp->getName());
        QList<Table*> tables = temp->getTable();
        int j=0;

        for(Table* t : tables){
            QStandardItem *table = new QStandardItem(t->getName());
            QVector<Column*> columns = t->getColumns();
            int k=0;

            for(Column* c : columns){
                QStandardItem *item = new QStandardItem(c->getName());
                table->setChild(k,0,item);
                k++;
            }

            db->setChild(j,0,table);
            j++;
        }

        model->setItem(i,0,db);
        i++;
    }

    model->setHorizontalHeaderLabels(QStringList()<<"Manage");

    //设置model
    ui->treeView->setModel(model);
}


void MainWindow::on_pushButton_7_clicked()
{

}

