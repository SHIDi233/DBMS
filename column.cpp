#include "column.h"
#include "cstring"

Column::Column(QString name, TYPE type, int typeLen, int integrities) :
    _type(type), _typeLen(typeLen), _integrities(integrities)
{
    strcpy(_name, name.toLatin1().data());
    if(type == TYPE::INTEGER) _typeLen = 4;
    else if(type == TYPE::BOOL) _typeLen = 1;
    else if(type == TYPE::DOUBLE) _typeLen = 8;
    else if(type == TYPE::DATETIME) _typeLen = 32;
}

Column::Column() {

}

int Column::serialize(char buf[]) {

    int offset = 0;
    memcpy(buf + offset, &_order, 4);
    offset += 4;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, &_type, 4);
    offset += 4;
    memcpy(buf + offset, &_typeLen, 4);
    offset += 4;
    memcpy(buf + offset, _mtime, 32);
    offset += 32;
    memcpy(buf + offset, &_integrities, 4);
    offset += 4;

    return offset;
}

int Column::deSerialize(char buf[]) {

    int offset = 0;
    memcpy(&_order, buf + offset, 4);
    offset += 4;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(&_type, buf + offset, 4);
    offset += 4;
    memcpy(&_typeLen, buf + offset, 4);
    offset += 4;
    memcpy(_mtime, buf + offset, 32);
    offset += 32;
    memcpy(&_integrities, buf + offset, 4);
    offset += 4;

    return offset;
}

TYPE Column::getType() { return _type; }

int Column::getTypeLen() { return _typeLen; }

QString Column::getName() {
    return _name;
}

bool Column::modify(TYPE newType, int newTypeLen, int integrity) {
    _type = newType;
    _typeLen = newTypeLen;
    _integrities = integrity;
    return true;
}

int Column::getIntegrities() {
    return _integrities;
}

void Column::setTable(QString table) {
    _table = table;
}
