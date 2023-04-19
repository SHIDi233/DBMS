#ifndef SQLANALYSIS_H
#define SQLANALYSIS_H
#include<QString>


class SqlAnalysis
{
public:
    SqlAnalysis();
    void parse_sql(QString sql);
};

#endif // SQLANALYSIS_H
