#include "row.h"

Row::Row()
{

}

int Row::serialize(char buf[], const QVector<Column*>& columns) {
    int offset = 0;
    //循环写入
    for(auto &r : datas) {
        offset += r->getCharArray(buf + offset);
    }

    return offset;
}

int Row::deSerialize(char buf[], const QVector<Column*>& columns) {

    //清空datas
    for(auto &r : datas) { delete r; }
    datas.clear();

    //读取内容
    int offset = 0;
    for(auto &c : columns) {
        if(c->getType() == TYPE::BOOL) {
            Basic_Data *data = new Bool();
            offset += data->readCharArray(buf + offset);
            datas.append(data);
        } else if (c->getType() == TYPE::VARCHAR) {
            Basic_Data *data = new Varchar(c->getTypeLen());
            offset += data->readCharArray(buf + offset);
            datas.append(data);
        } else if (c->getType() == TYPE::DATETIME) {
            Basic_Data *data = new DateTime();
            offset += data->readCharArray(buf + offset);
            datas.append(data);
        } else if (c->getType() == TYPE::DOUBLE) {
            Basic_Data *data = new Double();
            offset += data->readCharArray(buf + offset);
            datas.append(data);
        } else if (c->getType() == TYPE::INTEGER) {
            Basic_Data *data = new Integer();
            offset += data->readCharArray(buf + offset);
            datas.append(data);
        }
    }

    return offset;
}

bool Row::addData(Basic_Data *data) {
    datas.push_back(data);
    return true;
}

QString Row::getValue(int index) {
    return datas[index]->getValue();
}

Basic_Data* Row::getData(int index) {
    return datas[index];
}

bool Row::setData(int index, QString data) {
    bool isOk = datas[index]->setValue(data);
    return isOk;
}
