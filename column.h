#ifndef COLUMN_H
#define COLUMN_H
#include<QString>
#include"basic_data.h"

class Column
{
public:
    Column();

    int order;//字段顺序
    char name[128];//字段名称
//    int type;//字段类型
//    int param;//字段类型参数
    TYPE bd;
    int typeLen;//数据类型及长度

    QString mtime;//最后修改时间
    int integrities;//完整性约束信息
};

#endif // COLUMN_H
