#include "basic_data.h"
#include "cstring"

Basic_Data::Basic_Data(int size, TYPE type) : _size(size), _type(type) {}

TYPE Basic_Data::getType() {
    return _type;
}

int Basic_Data::getSize() {
    return _size;
}

Basic_Data::~Basic_Data() {

}

bool Basic_Data::setValue(QString) {
    return false;
}

QString Basic_Data::getValue() {
    return "false";
}

int Basic_Data::getCharArray(char buf[]) {
    return 1;
}

int Basic_Data::readCharArray(char buf[]) {
    return 1;
}


bool Basic_Data::operator==(const QString& r) {
    return false;
}

bool Basic_Data::operator>=(const QString& r) {
    return false;
}

bool Basic_Data::operator>(const QString& r) {
    return false;
}

bool Basic_Data::operator<=(const QString& r) {
    return false;
}

bool Basic_Data::operator<(const QString& r) {
    return false;
}


Double::Double() : Basic_Data(8, TYPE::NULLDATA) {}

Double::Double(double value) : Basic_Data(8, TYPE::DOUBLE), _value(value) {}

Varchar::Varchar(int len) : Basic_Data(len + 1, TYPE::NULLDATA) {
    _value = new char[len + 1];
    memset(_value, 0, sizeof(char) * (len + 1));
}

Varchar::Varchar(QString value) : Basic_Data(value.size() + 1, TYPE::VARCHAR) {
    _value = new char[value.size() + 1];
    strcpy(_value, value.toLatin1().data());
}

Integer::Integer() : Basic_Data(4, TYPE::NULLDATA) {}

Integer::Integer(int value) : Basic_Data(4, TYPE::INTEGER), _value(value) {}

Bool::Bool() : Basic_Data(1, TYPE::NULLDATA) {}

Bool::Bool(bool value) : Basic_Data(1, TYPE::BOOL), _value(value) {}


//NullData::NullData(int len) : Basic_Data(len, TYPE::NULLDATA) {}

void Basic_Data::setType(TYPE type) {
    _type = type;
}

//Double函数
QString Double::getValue() {
    if(getType() == TYPE::NULLDATA) { return ""; }
    return QString::number(_value);
}

int Double::getCharArray(char buf[]) {
    if(getType() == TYPE::NULLDATA) {
        memset(buf, 0, getSize());
        return getSize();
    }
    memcpy(buf, &_value, 8);
    return 8;
}

int Double::readCharArray(char buf[]) {
    //判断是否为null
    bool isNull = true;
    for(int i = 0; i < getSize(); i++) {
        if(*(buf + i) != 0) { isNull = false; }
    }
    if(isNull) {
        setType(TYPE::NULLDATA);
        return getSize();
    }

    memcpy(&_value, buf, 8);
    setType(TYPE::DOUBLE);
    return 8;
}

bool Double::setValue(QString value) {
    bool isOk;//判断传入字符串能否转化为double
    double v = value.toDouble(&isOk);
    if(!isOk) {
        return false;
    }
    _value = v;
    setType(TYPE::DOUBLE);

    return true;
}

bool Double::operator==(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(b - _value < 0.000001 && b - _value > 0.000001) { return true; }
    else { return false; }
}

bool Double::operator>=(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(_value >= b) { return true; }
    else { return false; }
}

bool Double::operator>(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(_value > b) { return true; }
    else { return false; }

}

bool Double::operator<=(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(_value <= b) { return true; }
    else { return false; }

}

bool Double::operator<(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    double b = r.toDouble(&isOk);//把数组转为数字
    if(!isOk) { return false; }
    if(_value < b) { return true; }
    else { return false; }
}


//Varchar函数
QString Varchar::getValue() {
    if(getType() == TYPE::NULLDATA) { return ""; }
    return QString(_value);
}

bool Varchar::setValue(QString value) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(value.at(0) == '\'' && value.at(value.size() - 1) == '\'') { isOk = true; }
    else if(value.at(0) == '\"' && value.at(value.size() - 1) == '\"') { isOk = true; }
    if(!isOk) {
        //判断是否超过长度
        if(value.size() > getSize() - 1) { return false; }

        //赋值
        memcpy(_value, value.toLatin1().data(), value.size());
        setType(TYPE::VARCHAR);
        return true;
    }

    //判断是否超过长度
    if(value.size() - 2 > getSize() - 1) { return false; }

    //赋值
    memcpy(_value, value.toLatin1().data() + 1, value.size() - 2);
    setType(TYPE::VARCHAR);
    return true;
}

int Varchar::getCharArray(char buf[]) {
    if(getType() == TYPE::NULLDATA) {
        memset(buf, 0, getSize());
        return getSize();
    }
    memcpy(buf, _value, getSize());
    return getSize();
}

int Varchar::readCharArray(char buf[]) {
    //判断是否为null
    bool isNull = true;
    for(int i = 0; i < getSize(); i++) {
        if(*(buf + i) != 0) { isNull = false; }
    }
    if(isNull) {
        setType(TYPE::NULLDATA);
        return getSize();
    }

    memcpy(_value, buf, getSize());
    setType(TYPE::VARCHAR);
    return getSize();
}

bool Varchar::operator==(const QString& r) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(r.at(0) == '\'' && r.at(r.size() - 1) == '\'') { isOk = true; }
    else if(r.at(0) == '\"' && r.at(r.size() - 1) == '\"') { isOk = true; }
    if(!isOk) { return false; }
    QString r2 = r.mid(1, r.size() - 2);

    if(getType() == TYPE::NULLDATA) { return false; }

    if(r2.compare(_value) != 0) { return false; }
    else { return true; }
}

bool Varchar::operator>=(const QString& r) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(r.at(0) == '\'' && r.at(r.size() - 1) == '\'') { isOk = true; }
    else if(r.at(0) == '\"' && r.at(r.size() - 1) == '\"') { isOk = true; }
    if(!isOk) { return false; }
    QString r2 = r.mid(1, r.size() - 2);

    if(getType() == TYPE::NULLDATA) { return false; }

    if(r2.compare(_value) >= 0) { return true; }
    else { return false; }
}

bool Varchar::operator>(const QString& r) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(r.at(0) == '\'' && r.at(r.size() - 1) == '\'') { isOk = true; }
    else if(r.at(0) == '\"' && r.at(r.size() - 1) == '\"') { isOk = true; }
    if(!isOk) { return false; }
    QString r2 = r.mid(1, r.size() - 2);

    if(getType() == TYPE::NULLDATA) { return false; }

    if(r2.compare(_value) > 0) { return true; }
    else { return false; }
}

bool Varchar::operator<=(const QString& r) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(r.at(0) == '\'' && r.at(r.size() - 1) == '\'') { isOk = true; }
    else if(r.at(0) == '\"' && r.at(r.size() - 1) == '\"') { isOk = true; }
    if(!isOk) { return false; }
    QString r2 = r.mid(1, r.size() - 2);

    if(getType() == TYPE::NULLDATA) { return false; }

    if(r2.compare(_value) <= 0) { return true; }
    else { return false; }
}

bool Varchar::operator<(const QString& r) {
    //判断是否符合字符串结构
    bool isOk = false;
    if(r.at(0) == '\'' && r.at(r.size() - 1) == '\'') { isOk = true; }
    else if(r.at(0) == '\"' && r.at(r.size() - 1) == '\"') { isOk = true; }
    if(!isOk) { return false; }
    QString r2 = r.mid(1, r.size() - 2);

    if(getType() == TYPE::NULLDATA) { return false; }

    if(r2.compare(_value) < 0) { return true; }
    else { return false; }
}


//Integer函数
QString Integer::getValue() {
    if(getType() == TYPE::NULLDATA) { return ""; }
    return QString::number(_value);
}

bool Integer::setValue(QString value) {
    bool isOk;//判断传入字符串能否转化为int
    int v = value.toInt(&isOk);
    if(!isOk) {
        return false;
    }
    _value = v;
    setType(TYPE::INTEGER);
    return true;
}

int Integer::getCharArray(char buf[]) {
    if(getType() == TYPE::NULLDATA) {
        memset(buf, 0, getSize());
        return getSize();
    }
    memcpy(buf, &_value, 4);
    return 4;
}

int Integer::readCharArray(char buf[]) {
    //判断是否为null
    bool isNull = true;
    for(int i = 0; i < getSize(); i++) {
        if(*(buf + i) != 0) { isNull = false; }
    }
    if(isNull) {
        setType(TYPE::NULLDATA);
        return getSize();
    }

    memcpy(&_value, buf, 4);
    setType(TYPE::INTEGER);
    return 4;
}

bool Integer::operator==(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b == _value) { return true; }
    else { return false; }
}

bool Integer::operator>=(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b >= _value) { return true; }
    else { return false; }
}

bool Integer::operator>(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b > _value) { return true; }
    else { return false; }

}

bool Integer::operator<=(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b <= _value) { return true; }
    else { return false; }

}

bool Integer::operator<(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    bool isOk;
    int b = r.toInt(&isOk);
    if(isOk && b < _value) { return true; }
    else { return false; }
}


//Bool类型的函数
QString Bool::getValue() {
    if(getType() == TYPE::NULLDATA) { return ""; }
    if(_value) { return "ture"; }
    return "false";
}

bool Bool::setValue(QString value) {
    if(value.compare("true") == 0) {
        _value = true;
    } else if(value.compare("false") == 0) {
        _value = false;
    } else {
        return false;
    }
    setType(TYPE::BOOL);
    return true;
}

int Bool::getCharArray(char buf[]) {
    if(getType() == TYPE::NULLDATA) {
        memset(buf, 0, getSize());
        return getSize();
    }
    memcpy(buf, &_value, 1);
    return 1;
}

int Bool::readCharArray(char buf[]) {
    //判断是否为null
    bool isNull = true;
    for(int i = 0; i < getSize(); i++) {
        if(*(buf + i) != 0) { isNull = false; }
    }
    if(isNull) {
        setType(TYPE::NULLDATA);
        return getSize();
    }

    memcpy(&_value, buf, 1);
    setType(TYPE::BOOL);
    return 1;
}

bool Bool::operator==(const QString& r) {
    if(getType() == TYPE::NULLDATA) { return false; }

    if(_value && r.compare("true")) { return true; }
    if(!_value && r.compare("false")) { return true; }
    return false;
}

bool Bool::operator>=(const QString& r) {
    return false;
}

bool Bool::operator>(const QString& r) {
    return false;
}

bool Bool::operator<=(const QString& r) {
    return false;
}

bool Bool::operator<(const QString& r) {
    return false;
}


//DateTime类
DateTime::DateTime() : Basic_Data(32, TYPE::NULLDATA) {

}

QString DateTime::getValue() {
    return "";
}

int DateTime::getCharArray(char buf[]) {
    return 1;
}

int DateTime::readCharArray(char buf[]) {
    return 1;
}

bool DateTime::setValue(QString) {
    return 1;
}


bool DateTime::operator==(const QString& r) {
    return 1;
}

bool DateTime::operator>=(const QString& r) {
    return 1;
}

bool DateTime::operator>(const QString& r) {
    return 1;
}

bool DateTime::operator<=(const QString& r) {
    return 1;
}

bool DateTime::operator<(const QString& r) {
    return 1;
}
