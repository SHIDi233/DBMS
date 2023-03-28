/**
 * Created by HuangYipu
 * 基础数据类型封装类
 */
#ifndef BASIC_DATA_H
#define BASIC_DATA_H


class Basic_Data
{
private:
    int _size;//字节长度

public:
    Basic_Data();
    virtual void setValue();
};

//各种数据类型子类
class Number : public Basic_Data
{
public:
    Number();
    Number(int len);
    Number(int len, int pre);//pre:精度
};

class Char : public Basic_Data
{
public:
    Char(int len);
};

class Varchar : public Basic_Data
{
public:
    Varchar(int len);
};

class Integer : public Basic_Data
{
public:
    Integer();
};

#endif // BASIC_DATA_H
