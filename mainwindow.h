#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "db.h"
#include<QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void send_table(QString);
public:
    MainWindow(Client* c = nullptr,QWidget *parent = nullptr);
    //MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DB* db = nullptr;

    void showTable(QVector<QString>,QVector<QVector<QString>>);
    //void showTableAll(QVector<QVector<QString>>);//显示表信息
    void appendText(QString output);//附加输出
public slots:
    void showTableAll(QVector<QVector<QString>>);//显示表信息
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_action1_triggered();

    void on_action_4_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_5_triggered();

    void on_action_7_triggered();


    void on_action_CCN_triggered();

    void on_action_15_triggered();

    void on_action_16_triggered();

    void on_action_12_triggered();

    void on_action_13_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

private:
    Ui::MainWindow *ui;

    void showList();

    Client * client;

    QString fileName;//当前打开的文件

};
#endif // MAINWINDOW_H
