#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "db.h"

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

private:
    Ui::MainWindow *ui;

    void showList();

    Client * client;

};
#endif // MAINWINDOW_H
