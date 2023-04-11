#ifndef COLUMN_H
#define COLUMN_H
#include<QString>
#include"basic_data.h"

const int COLUMNBYTE = 4 + 128 + 4 + 4 + 32 + 4;

class Column
{
private:
    int _order;//字段顺序
    char _name[128];//字段名称
//    int type;//字段类型
//    int param;//字段类型参数
    TYPE _bd;
    int _typeLen;//数据类型及长度

    char _mtime[32];//最后修改时间
    int _integrities;//完整性约束信息
public:
    Column(QString name, TYPE bd, int typeLen, int integrities);
    int serialize(char buf[]);//序列化
    int deSerialize(char buf[]);//反序列化
};

#endif // COLUMN_H
