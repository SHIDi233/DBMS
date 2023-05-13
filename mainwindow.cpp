#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sqlanalysis.h"
#include "control.h"
#include<QDateTime>
#include<QStandardItemModel>
#include<QVector>
#include<QMouseEvent>
#include"highlighter.h"

Highlighter *highlighter;

MainWindow::MainWindow(Client* c,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = c;
    if(c!=nullptr){
        connect(this,SIGNAL(send_table(QString)),c,SLOT(send(QString)));
    }
    //user.createDb("testDB");

    user.loadDB();

    if(c==nullptr){
        showList();
    }

    //db = user.getDB("testDB");


    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->textEdit->setFont(font);
    highlighter = new Highlighter(ui->textEdit->document());

    ui->comboBox_2->addItem("sql");
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_5_clicked()
{
//    QString tableName = ui->lineEdit->text();
//    if(tableName.isEmpty()) { return; }
//    db->createTable(tableName);
}


void MainWindow::on_pushButton_6_clicked()
{
//    QString tableName = ui->lineEdit->text();
//    if(tableName.isEmpty()) { return; }
//    QString columnName = ui->lineEdit_2->text();
//    if(columnName.isEmpty()) { return; }
//    db->addColumn(tableName, columnName, TYPE::INTEGER, 4);

//    showList();
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
    if(db==nullptr&&this->client==nullptr){
        qDebug()<<"未选定数据库";
        return;
    }

    //记录所有语句
    QString all= ui->textEdit->toPlainText();
    all=all.replace("\n","");
    QStringList list = all.split(";");

    //预编译
    //...
    //执行语句
    if(this->client==nullptr){//本地连接模式
        SqlAnalysis sa(db,this);
        for(QString s : list){
            if(s=="")
                continue;
            showTableAll(sa.parse_sql(s));
            showList();
        }
    }
    else{//网络IP连接模式
        SqlAnalysis sa(db,this);
        for(QString s : list){
            if(s=="")
                continue;
            emit send_table(s);
        }
    }
}

void MainWindow::showTable(QVector<QString> name,QVector<QVector<QString>> table){
    QStandardItemModel *model = new QStandardItemModel(this);

    int i=0;
    for(QString s : name){
        model->setHorizontalHeaderItem(i, new QStandardItem(s));
        this->ui->tableView->setColumnWidth(i, 50);
        i++;
    }

    int j=0,k=0;
    for(QVector<QString> qs : table){
        for(QString s:qs){
            model->setItem(j, k, new QStandardItem(s));
            k++;
        }
        model->setHeaderData(0,Qt::Vertical, QString::number(j));
        j++;
    }
    this->ui->tableView->setModel(model);   //将tableview设置成model这个标准条目模型的模板, model设置的内容都将显示在tableview上
}

void MainWindow::showTableAll(QVector<QVector<QString>> table){
    QStandardItemModel *model = new QStandardItemModel(this);

    int j=0,k=0;
    for(QVector<QString> qs : table){
        k=0;
        for(QString s:qs){
            if(j==0){
                model->setHorizontalHeaderItem(k, new QStandardItem(s));
            }
            else{
                model->setItem(j, k, new QStandardItem(s));
            }
            k++;
        }
        model->setHeaderData(0,Qt::Vertical, QString::number(j));
        j++;
    }

    this->ui->tableView->setModel(model);   //将tableview设置成model这个标准条目模型的模板, model设置的内容都将显示在tableview上
}

//命令执行后的输出
void MainWindow::appendText(QString output){
    ui->textEdit->setText(ui->textEdit->toPlainText()+"\n"+output);//附加输出
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QModelIndex inp=index;
    QModelIndex parent =inp.parent();
    if(parent.column()==-1){//选定为数据库
       QVariant db_name = inp.data();
       QString qs_db = db_name.toString();
       db = user.getDB(qs_db);
    }
    return;
}

