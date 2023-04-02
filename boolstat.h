/**
 * Created by Huang
 * 布尔语句的类，用于实现select中where的功能
 * 每个节点存在一个判断语句与连接符号(and或者or)
 * 第一个连接符号默认为and
 */
#ifndef BOOLSTAT_H
#define BOOLSTAT_H
#include "basic_data.h"

class BoolStat
{
private:
    char _columnName[128];//字段名
    bool _connect;//true表示用and连接，false表示用or连接
public:
    BoolStat();
    virtual bool judge(Basic_Data);
    const char* getColumnName();
};

//比较类
class Compare : public BoolStat
{
private:
    Basic_Data _value;
public:
    Compare(QString columnName, Basic_Data value, QString type);//type："<", "<=", ">", ">=", "="
    bool judge(Basic_Data data);
};

//between类
class Between : public BoolStat
{
private:
    Basic_Data _value1;
    Basic_Data _value2;
public:
    Between(QString columnName, Basic_Data value1, Basic_Data value2);
    bool judge(Basic_Data data);
};

//in类
class IsIn : public BoolStat
{
private:
    QVector<Basic_Data>* _values;
public:
    IsIn(QString columnName, const QVector<Basic_Data>* values);
    bool judge(Basic_Data data);
};

#endif // BOOLSTAT_H
