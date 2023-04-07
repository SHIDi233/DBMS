#include "table.h"

Table::Table(QString name, QString tdf, QString tic, QString trd, QString tid)
{
    //表名
    QByteArray ba = name.toLocal8Bit();
    memcpy(_name,ba.data(),ba.size()+1);

    //表定义
    QByteArray ba_tdf = tdf.toLocal8Bit();
    memcpy(_tdf,ba_tdf.data(),ba_tdf.size()+1);

    //表完整性
    QByteArray ba_tic = tic.toLocal8Bit();
    memcpy(_tic,ba_tic.data(),ba_tic.size()+1);

    //表记录文件
    QByteArray ba_trd = trd.toLocal8Bit();
    memcpy(_trd,ba_trd.data(),ba_trd.size()+1);

    //表索引文件
    QByteArray ba_tid = tid.toLocal8Bit();
    memcpy(_tid,ba_tid.data(),ba_tid.size()+1);
}

int Table::insert(int no,Basic_Data input){

}

int Table::insert(bool no[],Basic_Data input[]){
    for(int i=0;i<_fieldNum;i++){
        if(no[i]){
            //Basic_Data类型检测


        }
    }
}

int Table::insert(Row input){
    doList.append(WaitToDo_Table(WaitToDo_Table::INSERT,input));//将加入行放入缓存
}
