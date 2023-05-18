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
    Basic_Data* getData(int index);
    bool setData(int index, QString data);

    Row *operator+(Row *const r) {
        Row *res = new Row();
        res->datas.append(this->datas);
        res->datas.append(r->datas);
        return res;
    }
};
#endif // ROW_H
