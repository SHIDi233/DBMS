/**
 * Created by HuangYipu
 * 基础数据类型封装类
 */
#ifndef BASIC_DATA_H
#define BASIC_DATA_H

#include "QString"

enum TYPE
{
    INTEGER = 0,
    BOOL = 1,
    VARCHAR = 2,
    DOUBLE = 3,
    DATETIME = 4
};

class Basic_Data
{
private:
    int _size;//字节长度
    TYPE _type;//数据类型
public:
    Basic_Data(int size, TYPE type);
    int getSize();//获取字节大小
    TYPE getType();//获取类型

    virtual void setValue(QString);
    virtual QString getValue();
    virtual bool operator==(const QString& r);
    virtual bool operator>=(const QString& r);
    virtual bool operator>(const QString& r);
    virtual bool operator<=(const QString& r);
    virtual bool operator<(const QString& r);
};

//各种数据类型子类
//浮点数类型
class Double : public Basic_Data
{
private:
    double _value;
public:
    Double();
    Double(double value);
    QString getValue();
    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//变长字符串
class Varchar : private Basic_Data
{
private:
    char* _value;
public:
    Varchar(int len);
    Varchar(QString value);
    QString getValue();
    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//整数类型
class Integer : public Basic_Data
{
    int _value;
public:
    Integer();
    Integer(int value);
    QString getValue();
    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//日期时间
class DateTime : public Basic_Data
{
public:
    DateTime();
    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//布尔类型
class Bool : public Basic_Data
{
private:
    bool _value;
public:
    Bool();
    Bool(bool value);
    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

#endif // BASIC_DATA_H
