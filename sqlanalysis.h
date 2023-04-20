﻿/**
*  Name:SqlAnalysis
*  Author: Li Wenjie, Shi haoyuan
*  Description:Sql语句分析
*  Date: 2023-4-19
*/

#ifndef SQLANALYSIS_H
#define SQLANALYSIS_H
#include "basic_data.h"
#include "db.h"
#include<QString>
#include<QVector>
#include "mainwindow.h"


class SqlAnalysis
{
public:
    SqlAnalysis();
    SqlAnalysis(DB*,MainWindow*);
    void parse_sql(QString sql);//sql语句传入
    void trim_create(QString,QVector<QString>*);//sql语句修剪-表添加
    void trim_insert(QString,QString,QVector<QString>*,QVector<QString>*);//sql语句修剪-行添加
    void trim_delete(QString,QVector<QString>*);//sql语句修剪-行删除
    void trim_update(QString,QVector<QString>*);//sql语句修剪-表创建
    //void trim_create(QString,QVector<QString>*);//sql语句修剪-表创建
    void trim_select(QString,QVector<QString>*);//sql语句修剪-表搜索


    TYPE get_type(QString);//获得类型
    int get_size(QString);//获得VARCHAR长度

private:
    DB* db;
    MainWindow* m;
};

#endif // SQLANALYSIS_H
