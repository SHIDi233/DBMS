/**
 * Created by Huang
 * 布尔语句的类，用于实现select中where的功能
 * 每个节点存在一个判断语句与连接符号(and或者or)
 * 第一个连接符号默认为and
 */
#ifndef BOOLSTAT_H
#define BOOLSTAT_H
#include "basic_data.h"
#include <QVector>

enum BoolType {
    COMPARE = 0,
    BETWEEN = 1,
    ISIN = 2
};

class BoolStat
{
private:
    BoolType _type;
    QString _columnName;//字段名
    bool _connect;//true表示用and连接，false表示用or连接
public:
    BoolStat(BoolType type, QString name, bool connect);
    BoolStat();
    virtual bool judge(Basic_Data *data);
    virtual void setValue(QString value);
    virtual QString getValue();
    QString getColumnName();
    void setConnect(bool connect);
    bool getConnect();
    BoolType getType();
    virtual bool writeToFile(QDataStream &bovOut);
};

//比较类
class Compare : public BoolStat
{
private:
    QString _value;
    QString _ctype;//表示比较类型s
public:
    Compare(QString columnName, QString value, QString type, bool connect = true);//type："<", "<=", ">", ">=", "="
    bool judge(Basic_Data *data);
    bool writeToFile(QDataStream &bovOut);
    void setValue(QString value);
    QString getValue();
};

//between类
class Between : public BoolStat
{
private:
    QString _value1;
    QString _value2;
public:
    Between(QString columnName, QString value1, QString value2, bool connect = true);
    bool judge(Basic_Data *data);
    bool writeToFile(QDataStream &bovOut);
};

//in类
class IsIn : public BoolStat
{
private:
    QVector<QString> _values;
public:
    IsIn(QString columnName, const QVector<QString> &values, bool connect = true);
    bool judge(Basic_Data *data);
    bool writeToFile(QDataStream &bovOut);
};

#endif // BOOLSTAT_H
