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
    DATETIME = 4,
    NULLDATA = 5
};

class Basic_Data
{
private:
    int _size;//字节长度
    TYPE _type;//数据类型
public:
    Basic_Data(int size, TYPE type);
    ~Basic_Data();
    int getSize();//获取字节大小
    TYPE getType();//获取类型
    void setType(TYPE type);

    virtual bool setValue(QString);
    virtual QString getValue();
    virtual int getCharArray(char buf[]);//序列化
    virtual int readCharArray(char buf[]);//反序列化

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
    int getCharArray(char buf[]);
    int readCharArray(char buf[]);
    bool setValue(QString);

    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//变长字符串
class Varchar : public Basic_Data
{
private:
    char* _value;
public:
    Varchar(int len);
    Varchar(QString value);

    QString getValue();
    int getCharArray(char buf[]);
    int readCharArray(char buf[]);
    bool setValue(QString);

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
    int getCharArray(char buf[]);
    int readCharArray(char buf[]);
    bool setValue(QString);

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

    QString getValue();
    int getCharArray(char buf[]);
    int readCharArray(char buf[]);
    bool setValue(QString);

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

    QString getValue();
    int getCharArray(char buf[]);
    int readCharArray(char buf[]);
    bool setValue(QString);

    bool operator==(const QString& r);
    bool operator>=(const QString& r);
    bool operator>(const QString& r);
    bool operator<=(const QString& r);
    bool operator<(const QString& r);
};

//class NullData : public Basic_Data
//{
//private:
//public:
//    NullData(int len);

//    QString getValue();
//    int getCharArray(char buf[]);
//    int readCharArray(char buf[]);
//    bool setValue(QString);

//    bool operator==(const QString& r);
//    bool operator>=(const QString& r);
//    bool operator>(const QString& r);
//    bool operator<=(const QString& r);
//    bool operator<(const QString& r);
//};

#endif // BASIC_DATA_H
