#include "boolstat.h"

BoolStat::BoolStat(QString name, bool connect) :
    _columnName(name), _connect(connect)
{

}

bool BoolStat::judge(Basic_Data) {
    return false;
}

QString BoolStat::getColumnName() {
    return _columnName;
}

bool BoolStat::getConnect() {
    return _connect;
}

Compare::Compare(QString columnName, QString value, QString type, bool connect) :
    BoolStat(columnName, connect), _value(value), _type(type)
{
}

bool Compare::judge(Basic_Data data) {
    if(_type == "=") {
        return data == _value;
    }
    if(_type == ">=") {
        return data >= _value;
    }
    if(_type == "<=") {
        return data <= _value;
    }
    if(_type == ">") {
        return data > _value;
    }
    if(_type == "<") {
        return data < _value;
    }
    return false;
}

Between::Between(QString columnName, QString value1, QString value2, bool connect) :
    BoolStat(columnName, connect), _value1(value1), _value2(value2)
{
}

bool Between::judge(Basic_Data data) {
    return data >= _value1 && data <= _value2;
}

IsIn::IsIn(QString columnName, const QVector<QString> &values, bool connect) :
    BoolStat(columnName, connect), _values(values)
{
}

bool IsIn::judge(Basic_Data data) {
    bool isIn = false;
    for(auto &v : _values) {
        if(data == v) {
            isIn = true;
            break;
        }
    }

    return isIn;
}
