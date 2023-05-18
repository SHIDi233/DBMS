/**
*  Name:SqlDebug
*  Author: Shi haoyuan
*  Description:Sql语句debug
*  Date: 2023-5-13
*/

#include "sqldebug.h"
#include "sqlanalysis.h"
#include <iostream>
#include <regex>
#include <string>
#include "Control.h"
#include<qDebug>
#include<QList>

using namespace std;

SqlDebug::SqlDebug()
{

}

QString SqlDebug::parse_sql(QString qsql) {
   /**
   *  sql语句预处理:全部转换为大写
   */
   //qsql=qsql.toUpper();//旧语句，不能对引号内容进行正确转换
   bool isName=false;
   int num=0;
   for(QChar c : qsql){
       if(c=='"' || c=='\''){
           if(isName==true){
               isName=false;
           }
           else{
               isName=true;
           }
       }


       if(!isName)
           qsql[num]=qsql[num].toUpper();
       num++;
   }

   string sql = qsql.toStdString();

   // USE DATABASE 语句的正则表达式
   regex use_db_pattern(R"(USE\s+(\w+))");

   // CREATE DATABASE 语句的正则表达式
   regex create_db_pattern(R"(CREATE\s+DATABASE\s+(\w+))");

   // DROP DATABASE 语句的正则表达式
   regex drop_db_pattern(R"(DROP\s+DATABASE\s+(\w+))");

   // CREATE TABLE 语句的正则表达式
   regex create_table_pattern(R"(CREATE\s+TABLE\s+(\w+)\s*\((.+)\))");

   // DESC TABLE 语句的正则表达式
   regex desc_table_pattern(R"(DESC\s+(\w+))");

   // INSERT INTO 语句的正则表达式
   regex insert_into_pattern(R"(INSERT\s+INTO\s+(\w+)\s*\((.+)\)\s*VALUES\s*\((.+)\))");

   // DELETE FROM 语句的正则表达式
   regex delete_from_pattern(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(.+))");

   // UPDATE 语句的正则表达式
   regex update_pattern(R"(UPDATE\s+(\w+)\s+SET\s+(.+)\s+WHERE\s+(.+))");

   // DROP TABLE 语句的正则表达式
   regex drop_table_pattern(R"(DROP\s+TABLE\s+(\w+))");

   // CREATE INDEX 语句的正则表达式
   regex create_index_regex(R"(^\s*CREATE\s+(UNIQUE\s+)?(CLUSTERED|NONCLUSTERED\s+)?INDEX\s+(\w+)\s+ON\s+(\w+)\s*\((.*)\)\s*)");

   // DROP INDEX 语句的正则表达式
   regex drop_index_regex(R"(^\s*DROP\s+INDEX\s+(\w+)\s+ON\s+(\w+)\s*)");

   // SELECT 语句的正则表达式，包括可选的GROUP BY、HAVING和ORDER BY子句
   regex select_pattern(
           R"(SELECT\s+(.+)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.+))?(?:\s+GROUP\s+BY\s+(.+))?(?:\s+HAVING\s+(.+))?(?:\s+ORDER\s+BY\s+(.+))?)");

   smatch match;
   if (regex_match(sql, match, use_db_pattern)) {
       // 匹配 USE 语句
       string db_name = match[1];
       QString name = QString(QString::fromLocal8Bit(db_name.data()));
        return "";

   }
   else if (regex_match(sql, match, create_db_pattern)) {
       // 匹配 CREATE TABLE 语句
       string db_name = match[1];
       return "";
   }
   else if (regex_match(sql, match, create_table_pattern)) {
       // 匹配 CREATE TABLE 语句
       string table_name = match[1];
       string columns_str = match[2];
       return "";
   }else if(regex_match(sql, match, desc_table_pattern)){
       //匹配 Desc 语句
       string table_name = match[1];
       //db->
       return "";

   }else if (regex_match(sql, match, insert_into_pattern)) {
       // 匹配 INSERT INTO 语句
       string table_name = match[1];
       string columns_str = match[2];
       string values_str = match[3];
       return "";
   } else if (regex_match(sql, match, delete_from_pattern)) {
       // 匹配 DELETE FROM 语句
       string table_name = match[1];
       string condition = match[2];
        return "";


   } else if (regex_match(sql, match, update_pattern)) {
       // 匹配 UPDATE 语句
       string table_name = match[1];
       string set_clause = match[2];
       string condition = match[3];
        return "";

   } else if (regex_match(sql, match, drop_table_pattern)) {
       // 匹配 DROP TABLE 语句
       string table_name = match[1];
    return "";

   } else if (regex_match(sql, match, create_index_regex)) {
    return "";

   } else if (regex_match(sql, match, drop_index_regex)) {
    return "";

       //......调用 DROP INDEX 函数操作

   } else if (regex_match(sql, match, select_pattern)) {
       // 匹配 SELECT 语句
       string columns_str = match[1];
       string table_name = match[2];
       string where_clause = match[3];
       string group_by_clause = match[4];
       string having_clause = match[5];
       string order_by_clause = match[6];



       // 检查是否存在 WHERE 子句
       if (!where_clause.empty()) {
           //cout << "WHERE " << where_clause << endl;


       }

       // 检查是否存在 GROUP BY 子句
       if (!group_by_clause.empty()) {
           //cout << "GROUP BY " << group_by_clause << endl;
       }

       // 检查是否存在 HAVING 子句
       if (!having_clause.empty()) {
           //cout << "HAVING " << having_clause << endl;
       }

       // 检查是否存在 ORDER BY 子句
       if (!order_by_clause.empty()) {
           //cout << "ORDER BY " << order_by_clause << endl;
       }

       //......调用 SELECT 函数操作
       QVector<QString>* columns = new QVector<QString>;
       trim_select(QString(QString::fromLocal8Bit(columns_str.data())),columns);

       QVector<QString>* t=new QVector<QString>;
       trim_where(QString(QString::fromLocal8Bit(where_clause.data())),t);
       QVector<QString> s = *t;

       QVector<BoolStat*> bs;
        return "";

   } else {
       return "Invalid SQL statement";
   }
   return "Invalid SQL statement";
}



//sql语句预处理-表建立
void SqlDebug::trim_create(QString input,QVector<QString>* output){
   input = input.replace(QRegExp(",")," ");
   QStringList list = input.split(" ");
   for(auto &s : list){
       if(s=="")
           continue;
       output->append(s);
   }
}

//sql语句预处理-表插入
void SqlDebug::trim_insert(QString columns,QString values,QVector<QString>* output1,QVector<QString>* output2){
   columns = columns.replace(QRegExp(",")," ");
   values = values.replace(QRegExp(",")," ");
   QStringList list_column = columns.split(" ");
   for(auto &s : list_column){
       if(s=="")
           continue;
       output1->append(s);
   }

   QStringList list_value = values.split(" ");
   for(auto &s : list_value){
       if(s=="")
           continue;
       output2->append(s);
   }
}

//sql语句预处理-表搜索
void SqlDebug::trim_select(QString input,QVector<QString>* output){
   input = input.replace(QRegExp(",")," ");
   QStringList list = input.split(" ");
   for(auto &s : list){
       if(s=="")
           continue;
       output->append(s);
   }
}

//sql语句预处理-表更改
void SqlDebug::trim_update(QString input,QVector<QString>* cnames,QVector<QString>* values){
   input = input.replace(QRegExp(",")," ");
   QStringList list = input.split(" ");
   int num=0;
   for(auto &s : list){
       if(s=="")
           continue;

       if(num%3==0){
           cnames->append(s);
       }
       else if(num%3==1){

       }
       else if(num%3==2){
           values->append(s);
       }
       num++;
   }

}

//sql语句预处理-where
void SqlDebug::trim_where(QString input,QVector<QString>* output){
   input = input.replace(QRegExp("=")," = ");
   input = input.replace(QRegExp(">")," > ");
   input = input.replace(QRegExp("<")," < ");
   input = input.replace(QRegExp(">=")," >= ");
   input = input.replace(QRegExp("<=")," <= ");
   QStringList list = input.split(" ");
   for(auto &s : list){
       if(s=="")
           continue;
       output->append(s);
   }
   return;
}

TYPE SqlDebug::get_type(QString input){
   Basic_Data* bd;
   if(input.contains(QRegExp("INT"))){
       bd = new Integer();
       return INTEGER;
   }
   else if(input.contains(QRegExp("BOOL"))){
       bd = new Bool();
       return BOOL;
   }
   else if(input.contains(QRegExp("VARCHAR"))){

       return VARCHAR;
   }
   else if(input.contains(QRegExp("DOUBLE"))){
       bd = new Double();
       return DOUBLE;
   }
   else if(input.contains(QRegExp("DATETIME"))){
       bd = new DateTime();
       return DATETIME;
   }
   return NULLDATA;
}

int SqlDebug::get_size(QString input){
   input.replace(QRegExp("VARCHAR"),"");
   input.replace('(',"");
   input.replace(')',"");
   bool isOk=false;
   int tmp = input.toInt(&isOk);
   if(isOk){
       return tmp;
   }
   else
       return 0;
}
