#include "integrity.h"
#include "cstring"

Integrity::Integrity(QString name, QString field, ITGTYPE type, QString param) :
    _type(type)
{
    strcpy(_name, name.toLatin1().data());
    strcpy(_field, field.toLatin1().data());
    strcpy(_param, param.toLatin1().data());
}

Integrity::Integrity() {}

int Integrity::serialize(char buf[]) {
    int offset = 0;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, _field, 128);
    offset += 128;
    memcpy(buf + offset, &_type, 4);
    offset += 4;
    memcpy(buf + offset, _param, 256);
    offset += 256;

    return offset;
}

int Integrity::deSerialize(char buf[]) {
    int offset = 0;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(_field, buf + offset, 128);
    offset += 128;
    memcpy(&_type, buf + offset, 4);
    offset += 4;
    memcpy(_param, buf + offset, 256);
    offset += 256;

    return offset;
}

QString Integrity::getName() {
    return _name;
}

QString Integrity::getParam() {
    return _param;
}

ITGTYPE Integrity::getType() {
    return _type;
}

QString Integrity::getField() {
    return _field;
}
