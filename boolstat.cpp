#include "boolstat.h"

BoolStat::BoolStat(BoolType type, QString name, bool connect) :
    _type(type), _columnName(name), _connect(connect)
{

}

bool BoolStat::writeToFile(QDataStream &covOut) {
    return true;
}

void BoolStat::setValue(QString value) {

}

QString BoolStat::getValue() {

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

void Compare::setValue(QString value) {
    _value = value;
}

QString Compare::getValue() {
    return _value;
}

bool Compare::writeToFile(QDataStream &bovOut) {
    bovOut << QString::number(0) << getColumnName() << _value << _ctype << QString::number(getConnect());
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
    bovOut << QString::number(1) << getColumnName() << _value1 << _value2 << QString::number(getConnect());
    return true;
}

IsIn::IsIn(QString columnName, const QVector<QString> &values, bool connect) :
    BoolStat(BoolType::ISIN, columnName, connect), _values(values)
{
}

bool IsIn::writeToFile(QDataStream &bovOut) {
    bovOut << QString::number(2) << getColumnName() << _values.size();
    for(auto & v : _values) {
        bovOut << v;
    }
    bovOut << QString::number(getConnect());
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
