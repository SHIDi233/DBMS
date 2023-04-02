/**
 * Created by HuangYipu
 * 基础数据类型封装类
 */
#ifndef BASIC_DATA_H
#define BASIC_DATA_H

#include "QString"

enum TYPE
{
    INTEGER,
    BOOL,
    VARCHAR,
    DOUBLE,
    DATETIME
};

class Basic_Data
{
private:
    int _size;//字节长度
    TYPE _type;//数据类型
public:
    Basic_Data(int size, TYPE type);
    virtual void setValue(QString);
    virtual QString getValue();
};

//各种数据类型子类
//浮点数类型
class Double : public Basic_Data
{
public:
    Double(int len);
};

//变长字符串
class Varchar : public Basic_Data
{
public:
    Varchar(int len);
};

//整数类型
class Integer : public Basic_Data
{
public:
    Integer();
};

//日期时间
class DateTime : public Basic_Data
{
public:
    DateTime();
};

//布尔类型
class Bool : public Basic_Data
{
public:
    Bool();
};

#endif // BASIC_DATA_H
