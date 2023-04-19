#ifndef ROW_H
#define ROW_H
#include<QList>
#include"Basic_Data.h"
#include "column.h"
#include "QVector"

class Row
{
private:
    QVector<Basic_Data*> datas;
public:
    Row();

    int serialize(char buf[], const QVector<Column*>& columns);//序列化
    int deSerialize(char buf[], const QVector<Column*>& columns);//反序列化

    bool addData(Basic_Data *data);
    QString getValue(int index);
};
#endif // ROW_H
