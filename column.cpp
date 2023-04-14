#include "column.h"
#include "cstring"

Column::Column(QString name, TYPE bd, int typeLen, int integrities) :
    _bd(bd), _typeLen(typeLen), _integrities(integrities)
{
    strcpy(_name, name.toLatin1().data());
}

int Column::serialize(char buf[]) {

    int offset = 0;
    memcpy(buf + offset, &_order, 4);
    offset += 4;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, &_bd, 4);
    offset += 4;
    memcpy(buf + offset, &_typeLen, 4);
    offset += 4;
    memcpy(buf + offset, _mtime, 32);
    offset += 32;

    return offset;
}

int Column::deSerialize(char buf[]) {

    int offset = 0;
    memcpy(&_order, buf + offset, 4);
    offset += 4;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(&_bd, buf + offset, 4);
    offset += 4;
    memcpy(&_typeLen, buf + offset, 4);
    offset += 4;
    memcpy(_mtime, buf + offset, 32);
    offset += 32;

    return offset;
}
