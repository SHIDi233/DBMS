/**
*  Name:SqlAnalysis
*  Author: Li Wenjie, Shi haoyuan
*  Description:Sql语句分析
*  Date: 2023-4-20
*/
#include "sqlanalysis.h"
#include <iostream>
#include <regex>
#include <string>
#include "Control.h"
#include "basic_data.h"
#include<qDebug>
#include<QList>

using namespace std;


SqlAnalysis::SqlAnalysis(DB* db)
{
   this->db = db;
   m = nullptr;
   //this->ns = ns;
}

SqlAnalysis::SqlAnalysis(DB* db,MainWindow* m)
{
   this->db = db;
   this->m = m;
   //this->ns =  nullptr;
}

QVector<QVector<QString>> SqlAnalysis::parse_sql(QString qsql) {
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
   qsql.replace(",","_");//满足正则化

   string sql = qsql.toStdString();

   // CREATE USER 语句的正则表达式
   regex create_user_pattern(R"(CREATE\s+USER\s+(.+))");

   regex commit_pattern(R"(COMMIT\s)");
   regex roll_pattern(R"(ROLLBACK\s)");

   // USE DATABASE 语句的正则表达式
   regex use_db_pattern(R"(USE\s+(\w+))");

   // CREATE DATABASE 语句的正则表达式
   regex create_db_pattern(R"(CREATE\s+DATABASE\s+(\w+))");

   // DROP DATABASE 语句的正则表达式
   regex drop_db_pattern(R"(DROP\s+DATABASE\s+(\w+))");

   regex grant_pattern(R"(GRANT\s+(\w+)\s+(\w+))");

   // CREATE TABLE 语句的正则表达式
   regex create_table_pattern(R"(CREATE\s+TABLE\s+(\w+)\s*\((.+)\))");

   // CREATE TABLE 语句的正则表达式
   regex alter_table_add_pattern(R"(ALTER\s+TABLE\s+(\w+)\s+ADD+(.+))");
   // CREATE TABLE 语句的正则表达式
   regex alter_table_modify_pattern(R"(ALTER\s+TABLE\s+(\w+)\s+MODIFY+(.+))");
   // CREATE TABLE 语句的正则表达式
   regex alter_table_drop_pattern(R"(ALTER\s+TABLE\s+(\w+)\s+DROP+(.+))");

   regex alter_table_add_primary_pattern(R"(ALTER\s+TABLE\s+(\w+)\s+ADD\s+PRIMARY\s+KEY\s+(.+))");

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



   // CREATE VIEW 语句的正则表达式
   //regex create_index_regex(R"(^\s*CREATE\s+(UNIQUE\s+)?(CLUSTERED|NONCLUSTERED\s+)?INDEX\s+(\w+)\s+ON\s+(\w+)\s*\((.*)\)\s*)");
   regex create_view_regex(R"(CREATE\s+VIEW\s+(\w+)\s+AS\s+SELECT\s+(.+)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.+))?(?:\s+GROUP\s+BY\s+(.+))?(?:\s+HAVING\s+(.+))?(?:\s+ORDER\s+BY\s+(.+))?)");

   // DROP INDEX 语句的正则表达式
   regex drop_index_regex(R"(^\s*DROP\s+INDEX\s+(\w+)\s+ON\s+(\w+)\s*)");

   // SELECT 语句的正则表达式，包括可选的GROUP BY、HAVING和ORDER BY子句
   regex select_pattern(
           R"(SELECT\s+(.+)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.+))?(?:\s+GROUP\s+BY\s+(.+))?(?:\s+HAVING\s+(.+))?(?:\s+ORDER\s+BY\s+(.+))?)");
   // SELECT 语句的正则表达式，包括可选的GROUP BY、HAVING和ORDER BY子句

   smatch match;
   if (regex_match(sql, match, use_db_pattern)) {
       // 匹配 USE 语句
       string db_name = match[1];
       QString name = QString(QString::fromLocal8Bit(db_name.data()));

//       if(ns!=nullptr){
//           ns->db_name = name;
//       }

   }
   else if (regex_match(sql, match, alter_table_add_primary_pattern)) {
       string table = match[1];
       string pk = match[2];
       if(m!=nullptr)
            m->appendText(db->addPK(QString(QString::fromLocal8Bit(table.data())),QString(QString::fromLocal8Bit(pk.data()))));
       else
           db->addPK(QString(QString::fromLocal8Bit(table.data())),QString(QString::fromLocal8Bit(pk.data())));
   }
   else if (regex_match(sql, match, create_user_pattern)) {
       // 匹配 CREATE DB 语句
       string data = match[1];
//       string user_name = match[1];
//       string user_pass = match[2];
       QStringList list = QString(QString::fromLocal8Bit(data.data())).split(" ");
       QString user_name;
       QString user_pass;
       int flag = 0;
       for(QString s:list)
       {
           if(s!=""){
               if(flag==0)
                user_name = s;
               if(flag==1)
                user_pass = s;
                flag++;
           }
       }

       //......调用CREATE函数操作
       if(m!=nullptr)
            m->appendText(User::getUser()->createUser(user_name,user_pass));
       else
           User::getUser()->createUser(user_name,user_pass);
   }
   else if (regex_match(sql, match, create_db_pattern)) {
       // 匹配 CREATE DB 语句
       string db_name = match[1];

       //......调用CREATE函数操作
       if(m!=nullptr)
            m->appendText(User::getUser()->createDb(QString(QString::fromLocal8Bit(db_name.data()))));
       else
           User::getUser()->createDb(QString(QString::fromLocal8Bit(db_name.data())));
       QString log_reverse = "DROP DATABASE "+QString(QString::fromLocal8Bit(db_name.data()));
        qDebug()<<log_reverse;//可用
   }
   else if (regex_match(sql, match, commit_pattern)) {
       // 匹配 CREATE DB 语句
       if(m!=nullptr)
            m->appendText(db->commit());
       else
           db->commit();
   }
   else if (regex_match(sql, match, create_table_pattern)) {
       // 匹配 CREATE TABLE 语句
       string table_name = match[1];
       string columns_str = match[2];

       cout << "CREATE TABLE statement" << "\ntable  name:" << table_name << "\ncolumn list:" <<" (" << columns_str << ")\n" << endl;

       //......调用CREATE函数操作
       if(m!=nullptr)
            m->appendText(db->createTable(QString(QString::fromLocal8Bit(table_name.data()))));//根据表名创建表
       else
           db->createTable(QString(QString::fromLocal8Bit(table_name.data())));
       QVector<QString>* output = new QVector<QString>;
       trim_create(QString(QString::fromLocal8Bit(columns_str.data())),output);
       qDebug()<<output;

       //重复调用添加列

       for(int i=0;i<output->count();i+=2){
           if(m!=nullptr)
                m->appendText(db->addColumn(QString(QString::fromLocal8Bit(table_name.data()))
                         ,(*output)[i],get_type((*output)[i+1]),get_size((*output)[i+1])));
           else
               db->addColumn(QString(QString::fromLocal8Bit(table_name.data()))
                                        ,(*output)[i],get_type((*output)[i+1]),get_size((*output)[i+1]));
       }

       QString log_reverse = "DROP TABLE "+QString(QString::fromLocal8Bit(table_name.data()));
        qDebug()<<log_reverse;//可用

   }else if(regex_match(sql, match, desc_table_pattern)){
       //匹配 Desc 语句
       string table_name = match[1];

   }else if(regex_match(sql, match, grant_pattern)){
       //匹配 Desc 语句
       string user_name = match[1];
       string permission = match[2];
       QString p =QString(QString::fromLocal8Bit(permission.data()));
        if(p=="DBA"){
            User::getUser()->grant(QString(QString::fromLocal8Bit(user_name.data())),DBA);
        }
        else if(p=="AD"){
            User::getUser()->grant(QString(QString::fromLocal8Bit(user_name.data())),AD);
        }
        else if(p=="USER"){
            User::getUser()->grant(QString(QString::fromLocal8Bit(user_name.data())),USER);
        }
        else if(p=="VISITOR"){
            User::getUser()->grant(QString(QString::fromLocal8Bit(user_name.data())),VISITOR);
        }



   }
   else if (regex_match(sql, match, insert_into_pattern)) {
       // 匹配 INSERT INTO 语句
       string table_name = match[1];
       string columns_str = match[2];
       string values_str = match[3];

       cout << "INSERT statement " << "\ntable  name:" << table_name << "\ncolumn list:" <<" (" << columns_str << ") \nVALUES (" << values_str << ")\n" << endl;

       //......调用 INSERT 函数操作
       QVector<QString>* columns = new QVector<QString>;
       QVector<QString>* values = new QVector<QString>;
       trim_insert(QString(QString::fromLocal8Bit(columns_str.data())),QString(QString::fromLocal8Bit(values_str.data())),columns,values);

       if(m!=nullptr)
            m->appendText(db->insertRecord(QString(QString::fromLocal8Bit(table_name.data())),*columns,*values));
       else
           db->insertRecord(QString(QString::fromLocal8Bit(table_name.data())),*columns,*values);

       QString log_reverse = "DELETE FROM "+QString(QString::fromLocal8Bit(table_name.data()))+" WHERE ";
       for(int i=0;i<(*columns).length()-1;i++){
           log_reverse+= (*columns)[i]+"="+(*values)[i]+" AND ";
       }
       log_reverse+= (*columns)[(*columns).length()-1]+"="+(*values)[(*columns).length()-1];
       qDebug()<<log_reverse;//可用

   } else if (regex_match(sql, match, delete_from_pattern)) {
       // 匹配 DELETE FROM 语句
       string table_name = match[1];
       string condition = match[2];

       cout << "DELETE statement " << "\ntable  name:" << table_name << " \nWHERE " << condition << "\n" <<endl;

       //......调用 DELETE 函数操作

       QVector<QString>* t=new QVector<QString>;
       trim_where(QString(QString::fromLocal8Bit(condition.data())),t);
       QVector<QString> s = *t;

       QVector<BoolStat*> bs;
       for(int i=0;i<s.count()-2;){
           Compare* c;
           if(i==0){
               c=new Compare(s[i],s[i+2],s[i+1]);
               i+=3;
           }
           else{
               bool isTrue;
               if(s[i]=="AND")
                   isTrue=true;
               else
                   isTrue=false;
               c=new Compare(s[i+1],s[i+3],s[i+2],isTrue);
               i+=4;
           }
           bs.push_back(c);
       }

       if(m!=nullptr)
            m->appendText(db->deleteRecord(QString(QString::fromLocal8Bit(table_name.data())),bs));
       else
           db->deleteRecord(QString(QString::fromLocal8Bit(table_name.data())),bs);


       QString log_reverse = "INSERT INTO "+QString(QString::fromLocal8Bit(table_name.data()))+"(";
       QVector<QVector<QString>> last = parse_sql("SELECT * FROM "+QString(QString::fromLocal8Bit(table_name.data()))+" WHERE "+QString(QString::fromLocal8Bit(condition.data())));
       for(int i=0;i<last.count();i++){
            if(i==0){
                for(int k=0;k<last[0].count();k++){
                    log_reverse+=last[0][k];
                    if(k!=last[0].count()-1)
                        log_reverse+=",";
                    else
                        log_reverse+=") VALUES(";
                }
                continue;
            }
            for(int k=0;k<last[i].count();k++){
                log_reverse+=last[i][k];
                if(k!=last[i].count()-1)
                    log_reverse+=",";
                else
                    log_reverse+=");";
            }
       }
       qDebug()<<log_reverse;//可用

   } else if (regex_match(sql, match, update_pattern)) {
       // 匹配 UPDATE 语句
       string table_name = match[1];
       string set_clause = match[2];
       string condition = match[3];

       cout << "UPDATE statement" << "\ntable  name:" << table_name << " \nSET clause:" << set_clause << " \nWHERE " << condition << "\n" << endl;

       QVector<QString>* columns = new QVector<QString>;
       trim_select(QString(QString::fromLocal8Bit(set_clause.data())),columns);
       QVector<QString>* ct=new QVector<QString>;
       trim_where(QString::fromLocal8Bit(set_clause.data()),ct);
       QVector<QString> setname;
       QVector<QString> setvalue;
       for(int i=0;i<(*ct).count();i++){
           if((i+1)%3==1){
                setname.push_back((*ct)[i]);
           }
           if((i+1)%3==2){

           }
           if((i+1)%3==0){
                setvalue.push_back((*ct)[i]);
           }
       }
//       QVector<QString>* ct=new QVector<QString>;
//       trim_where(QString(QString::fromLocal8Bit(condition.data())),ct);


       QVector<QString>* t=new QVector<QString>;
       trim_where(QString(QString::fromLocal8Bit(condition.data())),t);
       QVector<QString> s = *t;

       QVector<BoolStat*> bs;
       for(int i=0;i<s.count()-2;){
           Compare* c;
           if(i==0){
               c=new Compare(s[i],s[i+2],s[i+1]);
               i+=3;
           }
           else{
               bool isTrue;
               if(s[i]=="AND")
                   isTrue=true;
               else
                   isTrue=false;
               c=new Compare(s[i+1],s[i+3],s[i+2],isTrue);
               i+=4;
           }
           bs.push_back(c);
       }
       //......调用 UPDATE 函数操作

       db->updateRecord(QString(QString::fromLocal8Bit(table_name.data())),setname,setvalue,bs);


   } else if (regex_match(sql, match, drop_table_pattern)) {
       // 匹配 DROP TABLE 语句
       string table_name = match[1];

       cout << "DROP TABLE statement " << "\ntable  name:" << table_name << "\n" << endl;

       //......调用 DROP 函数操作

       db->dropTable(QString(QString::fromLocal8Bit(table_name.data())));//根据表名删除表

   } /*else if (regex_match(sql, match, create_index_regex)) {
       cout << "CREATE INDEX statement" << "\nindex name:" << match[3] << "\ntable name:" << match[4] << "\ncolumn list:" << "(" << match[5] << ")\n" <<endl;
       if (match[1].matched) {
           cout << "\nUNIQUE 选项已启用" << endl;
       }
       if (match[2].matched) {
           cout << "\nCLUSTERED 选项已启用" << endl;
       }
       //......调用 CREATE INDEX 函数操作*/

     else if (regex_match(sql, match, drop_index_regex)) {
       cout << "DROP INDEX statement" << "\nindex name:" << match[1] << "\ntable name:" << match[2] << endl;

       //......调用 DROP INDEX 函数操作

   } else if (regex_match(sql, match, select_pattern)) {
       // 匹配 SELECT 语句
       string columns_str = match[1];
       string table_name = match[2];
       string where_clause = match[3];
       string group_by_clause = match[4];
       string having_clause = match[5];
       string order_by_clause = match[6];


       cout << "SELECT statement\n" << "select columns:(" << columns_str << ")\nFROM " << table_name << endl;

       // 检查是否存在 WHERE 子句
       if (!where_clause.empty()) {
           cout << "WHERE " << where_clause << endl;


       }

       // 检查是否存在 GROUP BY 子句
       if (!group_by_clause.empty()) {
           cout << "GROUP BY " << group_by_clause << endl;
       }

       // 检查是否存在 HAVING 子句
       if (!having_clause.empty()) {
           cout << "HAVING " << having_clause << endl;
       }

       // 检查是否存在 ORDER BY 子句
       if (!order_by_clause.empty()) {
           cout << "ORDER BY " << order_by_clause << endl;
       }

       //......调用 SELECT 函数操作
       QVector<QString>* columns = new QVector<QString>;
       trim_select(QString(QString::fromLocal8Bit(columns_str.data())),columns);

       QVector<QString>* t=new QVector<QString>;
       trim_where(QString(QString::fromLocal8Bit(where_clause.data())),t);
       QVector<QString> s = *t;

       QVector<BoolStat*> bs;
       for(int i=0;i<s.count()-2;){
           Compare* c;
           if(i==0){
               c=new Compare(s[i],s[i+2],s[i+1]);
               i+=3;
           }
           else{
               bool isTrue;
               if(s[i]=="AND")
                   isTrue=true;
               else
                   isTrue=false;
               c=new Compare(s[i+1],s[i+3],s[i+2],isTrue);
               i+=4;
           }
           bs.push_back(c);
       }

       //表名语义分割
       QVector<QString> tin;
       QString tem = QString(QString::fromLocal8Bit(table_name.data()));
       tem = tem.replace(" ","");
       QStringList res = tem.split("_");
       for(QString s : res){
           tin.push_back(s);
       }
       //暂时测试表搜索功能语句，非最终版本
       if((*columns)[0]=="*"){
           for(Table* tb : db->getTable()){
                   return db->select(true,QVector<QString>(),tin,bs);
           }
       }
       else{
           return db->select(false,*columns,tin,bs);
       }


   }else if (regex_match(sql, match, create_view_regex)){
        //匹配view
       string view_name = match[1];
       string columns_str = match[2];
       string table_name = match[3];
       string where_clause = match[4];
       string group_by_clause = match[5];
       string having_clause = match[6];
       string order_by_clause = match[7];

       QString vn=QString(QString::fromLocal8Bit(view_name.data()));

       QVector<QString>* columns = new QVector<QString>;
       trim_select(QString(QString::fromLocal8Bit(columns_str.data())),columns);

       QVector<QString>* t=new QVector<QString>;
       trim_where(QString(QString::fromLocal8Bit(where_clause.data())),t);
       QVector<QString> s = *t;

       QVector<BoolStat*> bs;
       for(int i=0;i<s.count()-2;){
           Compare* c;
           if(i==0){
               c=new Compare(s[i],s[i+2],s[i+1]);
               i+=3;
           }
           else{
               bool isTrue;
               if(s[i]=="AND")
                   isTrue=true;
               else
                   isTrue=false;
               c=new Compare(s[i+1],s[i+3],s[i+2],isTrue);
               i+=4;
           }
           bs.push_back(c);
       }

       //表名语义分割
       QVector<QString> tin;
       QString tem = QString(QString::fromLocal8Bit(table_name.data()));
       tem = tem.replace(" ","");
       QStringList res = tem.split("_");
       for(QString s : res){
           tin.push_back(s);
       }

       //暂时测试表搜索功能语句，非最终版本
       if((*columns)[0]=="*"){
           //for(Table* tb : db->getTable()){
               db->createView(vn,true,QVector<QString>(),tin,bs);
           //}
       }
       else{
           db->createView(vn,false,*columns,tin,bs);
       }

   }
   else if (regex_match(sql, match, alter_table_add_pattern)){
       string table_name = match[1];
       string columns_str = match[2];
       string type = match[3];

       if(QString(QString::fromLocal8Bit(columns_str.data())).contains("PRIMARY")){
            QString res = QString(QString::fromLocal8Bit(columns_str.data())).replace("KEY(","");
            res = res.replace(")","");
            res= res.replace("PRIMARY","");
            res= res.replace(" ","");
            if(m!=nullptr)
                m->appendText(db->addPK(QString(QString::fromLocal8Bit(table_name.data())),res));
            else
                db->addPK(QString(QString::fromLocal8Bit(table_name.data())),res);
       }
       else{
           if(m!=nullptr){
               QStringList tel= QString(QString::fromLocal8Bit(columns_str.data())).split(" ");
                m->appendText(db->addColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                       tel[1],
                                       get_type(QString::fromLocal8Bit(columns_str.data())),
                                       get_size(QString::fromLocal8Bit(columns_str.data()))));
           }
           else{
               QStringList tel= QString(QString::fromLocal8Bit(columns_str.data())).split(" ");
               db->addColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                                  tel[1],
                                                  get_type(QString::fromLocal8Bit(columns_str.data())),
                                                  get_size(QString::fromLocal8Bit(columns_str.data())));
           }
       }


   }
   else if (regex_match(sql, match, alter_table_modify_pattern)){
       string table_name = match[1];
       string columns_str = match[2];
       string type = match[3];
       if(m!=nullptr)
            m->appendText(db->modifyColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                      QString(QString::fromLocal8Bit(columns_str.data())),
                                      get_type(QString::fromLocal8Bit(columns_str.data())),
                                      get_size(QString::fromLocal8Bit(columns_str.data()))));
       else
           db->modifyColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                                 QString(QString::fromLocal8Bit(columns_str.data())),
                                                 get_type(QString::fromLocal8Bit(columns_str.data())),
                                                 get_size(QString::fromLocal8Bit(columns_str.data())));
   }
   else if (regex_match(sql, match, alter_table_drop_pattern)){
       string table_name = match[1];
       string columns_str = match[2];
       if(m!=nullptr)
            m->appendText(db->dropColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                    QString(QString::fromLocal8Bit(columns_str.data()))));
       else
           db->dropColumn(QString(QString::fromLocal8Bit(table_name.data())),
                                               QString(QString::fromLocal8Bit(columns_str.data())));
   }
   else if(QString(QString::fromLocal8Bit(sql.data()))=="COMMIT"){
       if(m!=nullptr)
            m->appendText(db->commit());
       else
           db->commit();
   }
   else if(QString(QString::fromLocal8Bit(sql.data()))=="ROLLBACK"){
       db = User::getUser()->getDB(db->getName());
       User::getUser()->loadDB();
   }
   else {
       cout << "Invalid SQL statement" << endl;
   }
   return QVector<QVector<QString>>();
}



//sql语句预处理-表建立
void SqlAnalysis::trim_create(QString input,QVector<QString>* output){
   input = input.replace(QRegExp("_")," ");
   QStringList list = input.split(" ");
   for(auto &s : list){
       if(s=="")
           continue;
       output->append(s);
   }
}

//sql语句预处理-表插入
void SqlAnalysis::trim_insert(QString columns,QString values,QVector<QString>* output1,QVector<QString>* output2){
   columns = columns.replace(QRegExp("_")," ");
   values = values.replace(QRegExp("_")," ");
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
void SqlAnalysis::trim_select(QString input,QVector<QString>* output){
   input = input.replace(QRegExp("_")," ");
   QStringList list = input.split(" ");
   for(auto &s : list){
       if(s=="")
           continue;
       output->append(s);
   }
}

//sql语句预处理-表更改
void SqlAnalysis::trim_update(QString input,QVector<QString>* cnames,QVector<QString>* values){
//   input = input.replace(QRegExp("_")," ");
//   QStringList list = input.split(" ");
//   int num=0;
//   for(auto &s : list){
//       if(s=="")
//           continue;

//       if(num%3==0){
//           cnames->append(s);
//       }
//       else if(num%3==1){

//       }
//       else if(num%3==2){
//           values->append(s);
//       }
//       num++;
//   }

}

//sql语句预处理-where
void SqlAnalysis::trim_where(QString input,QVector<QString>* output){
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

TYPE SqlAnalysis::get_type(QString input){
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

int SqlAnalysis::get_size(QString input){
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
