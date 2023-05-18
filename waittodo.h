#ifndef WAITTODO_H
#define WAITTODO_H
#include"row.h"

class WaitToDo
{
public:
    WaitToDo();
private:
    //int type;//缓存类型
};

#endif // WAITTODO_H

class WaitToDo_Table : public WaitToDo
{
public:
    static const int INSERT = 10001,//添加行
              DELETE = 10002;//删除行
    WaitToDo_Table(int type,Row r);
private:

};
