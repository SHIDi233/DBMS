#ifndef ROW_H
#define ROW_H
#include<QList>
#include"Basic_Data.h"


class Row
{
public:
    Row();
private:
    QList<Basic_Data> fields;
};
#endif // ROW_H
