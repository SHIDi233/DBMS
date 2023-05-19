#include "mainwindow.h"
#include "log.h"
#include "ui_mainwindow.h"
#include"sqlanalysis.h"
#include "control.h"
#include<QDateTime>
#include<QStandardItemModel>
#include<QVector>
#include<QMouseEvent>
#include"highlighter.h"
#include<QFileDialog>
#include<QTextCodec>
#include<QMessageBox>
#include<QProcess>
#include"sqldebug.h"
#include<QToolBar>
#include"about.h"

Highlighter *highlighter;

MainWindow::MainWindow(Client* c,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    QMenuBar *mBar=menuBar();
    mBar->setGeometry(10,25,1300,769);

    ui->tableView->setVisible(false);
    ui->textEdit_4->setVisible(false);

    client = c;
    if(c!=nullptr){
        connect(this,SIGNAL(send_table(QString)),c,SLOT(send(QString)));
        ui->label->setText("远程数据库");
    }
    //user.createDb("testDB");
    ui->label->setText("未选定数据库");

    User::getUser()->loadDB();

    if(c==nullptr){
        showList();
    }

    //db = user.getDB("testDB");


    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);

    ui->textEdit->setFont(font);
    highlighter = new Highlighter(ui->textEdit->document());

    ui->comboBox_2->addItem("sql");

    ui->pushButton_7->setEnter("color: rgb(255, 255, 255);border-width: 1px;border-color: rgb(14 , 150 , 254);border-style: solid;border-radius:5px;background-color:rgb(14 , 135 , 228);");
    ui->pushButton_7->setLeave("color: rgb(255, 255, 255);border-width: 1px;border-color: rgb(14 , 135 , 228);border-style: solid;border-radius:5px;background-color:rgb(14 , 150 , 254);");
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
    QVector<DB*> dbs = User::getUser()->getDbs();
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

    if(i==0){//首次使用创建root数据库
        User::getUser()->createDb("root");
        int ret2 = QMessageBox::information(nullptr, tr("CCN"),tr("首次使用，需要重启程序完成配置!"), QMessageBox::Ok);
        qApp->quit();
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }

    model->setHorizontalHeaderLabels(QStringList()<<"");

    //设置model
    ui->treeView->setModel(model);
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->textEdit_4->setText("\n");

    if(db==nullptr&&this->client==nullptr){
        qDebug()<<"未选定数据库";
        this->appendText("未选定数据库");
        return;
    }



    //记录所有语句
    QString all= ui->textEdit->toPlainText();
    all=all.replace("\n","");
    all=all.replace("/",";");
    QStringList list = all.split(";");

    //预编译
    //...
    //执行语句
    if(this->client==nullptr){//本地连接模式

        Log log("D:\\QTtest\\testDB\\dba\\data\\root\\root.log");


//        //检查错误
//            SqlDebug sd;
//        //        SqlAnalysis sa(db,this);
//            int i=1;
//            int isFalse[100]={0};
//            bool isTrue = true;
//            for(QString s : list){
//                if(s=="")
//                    continue;
//                QString back = sd.parse_sql(s);
//                if(back!=""){
//                    //qDebug()<<s+" "+back;
//                    qDebug()<<"第"+QString::number(i)+"句问题";
//                    this->appendText("第"+QString::number(i)+"句问题");
//                    isFalse[i]=1;
//                    isTrue=false;
//                }
//                i++;
//            }

//            for(int k=1;k<=100;k++){
//                if(isFalse[k]==1){
//                    ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+"→\n");
//                }
//                else{
//                    ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+"\n");
//                }
//            }

//            if(!isTrue)
//                return;

        SqlAnalysis sa(db,this);
        for(QString s : list){
            if(s=="")
                continue;
            log.insert(s,"");
            showTableAll(sa.parse_sql(s));
            showList();
        }
        log.save();
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
    if(table.length()==0)
        return;
    ui->tableView->setVisible(true);
    ui->textEdit_4->setVisible(false);
    QStandardItemModel *model = new QStandardItemModel(this);

    int j=0,k=0;
    for(QVector<QString> qs : table){
        k=0;
        for(QString s:qs){
//            if(j==0){
//                model->setHorizontalHeaderItem(k, new QStandardItem(s));
//            }
//            else{
                model->setItem(j, k, new QStandardItem(s));
            //}
            k++;
        }
        model->setHeaderData(0,Qt::Vertical, QString::number(j));
        j++;
    }

    this->ui->tableView->setModel(model);   //将tableview设置成model这个标准条目模型的模板, model设置的内容都将显示在tableview上
}

//命令执行后的输出
void MainWindow::appendText(QString output){
    ui->tableView->setVisible(false);
    ui->textEdit_4->setVisible(true);
    ui->textEdit_4->setText(ui->textEdit_4->toPlainText()+"\n"+output);//附加输出
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QModelIndex inp=index;
    QModelIndex parent =inp.parent();
    if(parent.column()==-1){//选定为数据库
       QVariant db_name = inp.data();
       QString qs_db = db_name.toString();
       db = User::getUser()->getDB(qs_db);
       ui->label->setText(qs_db);
       current_db = qs_db;
    }
    return;
}

//打开文件
void MainWindow::on_action1_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Sql"),
                                                      "/home",
                                                      tr("sql (*.sql *.txt *.cpp)"));
    if(fileName=="")
        return;
    QString displayString;
    QFile file(fileName);
    //目标文件路径
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!";
        return;
    }
    this->fileName = fileName;
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QByteArray arr = file.readAll();
    ui->textEdit->clear();
    ui->textEdit->setPlainText(codec->toUnicode(arr));
    file.close();
}


void MainWindow::on_action_4_triggered()
{
    qApp->quit();
}

//保存文件
void MainWindow::on_action_triggered()
{
    if(fileName=="")
        return;

    QFile file(fileName);
    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
//        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QTextStream out(&file); //IO设备对象的地址对其进行初始化
        out.setCodec("UTF-8");

        QString tem1 = ui->textEdit->toPlainText();
        string std = tem1.toStdString();
        QByteArray arr = tem1.toUtf8();

        out << arr; //输出
        file.close();
     }
}

//另存为
void MainWindow::on_action_2_triggered()
{
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString fileName = dlg.getSaveFileName(this, tr("保存sql到"), "./", tr("Sql File(*.sql)"));

    if( fileName == "" )
    {
        return;
    }
    QFile file(fileName);

    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
//        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QTextStream out(&file); //IO设备对象的地址对其进行初始化
        out.setCodec("UTF-8");

        QString tem1 = ui->textEdit->toPlainText();
        string std = tem1.toStdString();
        QByteArray arr = tem1.toUtf8();

        out << arr; //输出
        file.close();
     }
}


void MainWindow::on_action_5_triggered()
{
    if(db==nullptr&&this->client==nullptr){
        qDebug()<<"未选定数据库";
        return;
    }

    //记录所有语句
    QString all= ui->textEdit->toPlainText();
    all=all.replace("\n","");
    all=all.replace("/",";");
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

//检查
void MainWindow::on_action_7_triggered()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);

    ui->textEdit_2->setFont(font);
    ui->textEdit_2->setText("");

    //记录所有语句
    QString all= ui->textEdit->toPlainText();
    all=all.replace("\n","");
    all=all.replace("/",";");
    QStringList list = all.split(";");

    //预编译
    //...
    //执行语句
    if(this->client==nullptr){//本地连接模式
        SqlDebug sd;
//        SqlAnalysis sa(db,this);
        int i=1;
        int isFalse[100]={0};
        bool isTrue = false;
        for(QString s : list){
            if(s=="")
                continue;
            QString back = sd.parse_sql(s);
            if(back!=""){
                //qDebug()<<s+" "+back;
                qDebug()<<"第"+QString::number(i)+"句问题";
                this->appendText("第"+QString::number(i)+"句问题");
                isFalse[i]=1;
                isTrue=false;
            }
            i++;
        }

        for(int k=1;k<=100;k++){
            if(isFalse[k]==1){
                ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+"→\n");
            }
            else{
                ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+"\n");
            }
        }
    }
//    else{//网络IP连接模式
//        SqlAnalysis sa(db,this);
//        for(QString s : list){
//            if(s=="")
//                continue;
//            emit send_table(s);
//        }
//    }
}



void MainWindow::on_action_CCN_triggered()
{
    About* a = new About;
    a->show();
}

//备份功能
void MainWindow::on_action_15_triggered()
{
    QProcess process(this);
    process.setProgram("cmd");
    QStringList argument;
    argument<<"/c"<<"xcopy d:\\QTtest e:\\QTtest /e /s /q /i /y";
    process.setArguments(argument);
    process.start();
    process.waitForStarted(); //等待程序启动
    process.waitForFinished();//等待程序关闭
    QString temp=QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息
    qDebug()<<temp;
}

//恢复功能
void MainWindow::on_action_16_triggered()
{
    QProcess process(this);
    process.setProgram("cmd");
    QStringList argument;
    argument<<"/c"<<"xcopy e:\\QTtest d:\\QTtest /e /s /q /i /y";
    process.setArguments(argument);
    process.start();
    process.waitForStarted(); //等待程序启动
    process.waitForFinished();//等待程序关闭
    QString temp=QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息
    qDebug()<<temp;
}

//查看日志
void MainWindow::on_action_12_triggered()
{
    QString displayString;
    Log log("D:\\QTtest\\testDB\\dba\\data\\root\\root.log");

    ui->textEdit->setPlainText(log.getAll());
}

//日志回滚
void MainWindow::on_action_13_triggered()
{

}


void MainWindow::on_action_9_triggered()
{
    ui->tableView->setVisible(false);
    ui->textEdit_4->setVisible(true);
}


void MainWindow::on_action_10_triggered()
{
    ui->tableView->setVisible(true);
    ui->textEdit_4->setVisible(false);
}

