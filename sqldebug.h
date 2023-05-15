#ifndef SQLDEBUG_H
#define SQLDEBUG_H
#include<QString>
#include "basic_data.h"

class SqlDebug
{
public:
    SqlDebug();
    QString parse_sql(QString qsql);
private:
    void trim_where(QString input,QVector<QString>* output);
    void trim_update(QString input,QVector<QString>* cnames,QVector<QString>* values);
    int get_size(QString input);
    void trim_create(QString input,QVector<QString>* output);
    void trim_insert(QString columns,QString values,QVector<QString>* output1,QVector<QString>* output2);
    void trim_select(QString input,QVector<QString>* output);
    TYPE get_type(QString input);
};

#endif // SQLDEBUG_H
