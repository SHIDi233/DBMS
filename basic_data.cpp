#include "basic_data.h"
#include "cstring"

Basic_Data::Basic_Data(int size, TYPE type) : _size(size), _type(type) {}

Double::Double() : Basic_Data(8, TYPE::DOUBLE) {}

Double::Double(double value) : Basic_Data(8, TYPE::DOUBLE), _value(value) {}

Varchar::Varchar(int len) : Basic_Data(len + 1, TYPE::VARCHAR) {
    _value = new char[len + 1];
}

Varchar::Varchar(QString value) : Basic_Data(value.size() + 1, TYPE::VARCHAR) {
    _value = new char[value.size() + 1];
    strcpy(_value, value.toLatin1().data());
}

Integer::Integer() : Basic_Data(4, TYPE::INTEGER) {}

Integer::Integer(int value) : Basic_Data(4, TYPE::INTEGER), _value(value) {}

Bool::Bool() : Basic_Data(1, TYPE::BOOL) {}

Bool::Bool(bool value) : Basic_Data(1, TYPE::BOOL), _value(value) {}

QString Double::getValue() {
    return QString::number(_value);
}

int Double::getCharArray(char buf[]) {
    memcpy(buf, &_value, 8);
    return 8;
}

bool Double::operator==(const QString& r) {
    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(b - _value < 0.000001 && b - _value > 0.000001) { return true; }
    else { return false; }
}

bool Double::operator>=(const QString& r) {

}

bool Double::operator>(const QString& r) {

}

bool Double::operator<=(const QString& r) {

}

bool Double::operator<(const QString& r) {

}

QString Varchar::getValue() {
    return QString(_value);
}

bool Varchar::operator==(const QString& r) {
    if(r.compare(_value) != 0) { return false; }
    else { return true; }
}

QString Integer::getValue() {
    return QString::number(_value);
}

bool Integer::operator==(const QString& r) {
    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b == _value) { return true; }
    else { return false; }
}
