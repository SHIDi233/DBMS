#include "boolstat.h"

BoolStat::BoolStat(BoolType type, QString name, bool connect) :
    _type(type), _columnName(name), _connect(connect)
{

}

bool BoolStat::judge(Basic_Data *data) {
    return false;
}

BoolType BoolStat::getType() {
    return _type;
}

QString BoolStat::getColumnName() {
    return _columnName;
}

bool BoolStat::getConnect() {
    return _connect;
}

Compare::Compare(QString columnName, QString value, QString type, bool connect) :
    BoolStat(BoolType::COMPARE, columnName, connect), _value(value), _ctype(type)
{
}

bool Compare::judge(Basic_Data *data) {
    if(_ctype == "=") {
        return data->operator==(_value);
    }
    if(_ctype == ">=") {
        return data->operator>=(_value);
    }
    if(_ctype == "<=") {
        return data->operator<=(_value);
    }
    if(_ctype == ">") {
        return data->operator>(_value);
    }
    if(_ctype == "<") {
        return data->operator<(_value);
    }
    return false;
}

bool Compare::writeToFile(QDataStream &bovOut) {
    bovOut << BoolType::COMPARE << getColumnName() << _value << _ctype << getConnect();
    return true;
}

Between::Between(QString columnName, QString value1, QString value2, bool connect) :
    BoolStat(BoolType::BETWEEN, columnName, connect), _value1(value1), _value2(value2)
{
}

bool Between::judge(Basic_Data *data) {
    return data->getValue() >= _value1 && data->getValue() <= _value2;
}

bool Between::writeToFile(QDataStream &bovOut) {
    bovOut << BoolType::BETWEEN << getColumnName() << _value1 << _value2 << getConnect();
    return true;
}

IsIn::IsIn(QString columnName, const QVector<QString> &values, bool connect) :
    BoolStat(BoolType::ISIN, columnName, connect), _values(values)
{
}

bool IsIn::writeToFile(QDataStream &bovOut) {
    bovOut << BoolType::ISIN << getColumnName() << _values.size();
    for(auto & v : _values) {
        bovOut << v;
    }
    bovOut << getConnect();
    return true;
}

bool IsIn::judge(Basic_Data *data) {
    bool isIn = false;
    for(auto &v : _values) {
        if(data->getValue() == v) {
            isIn = true;
            break;
        }
    }

    return isIn;
}
