﻿/**
*  Name:SqlAnalysis
*  Author: Li Wenjie, Shi haoyuan
*  Description:Sql语句分析
*  Date: 2023-4-19
*/


#include "sqlanalysis.h"
#include <iostream>
#include <regex>
#include <string>
#include "control.h"

using namespace std;


SqlAnalysis::SqlAnalysis()
{

}

void SqlAnalysis::parse_sql(QString qsql) {
    string sql = qsql.toStdString();
    // CREATE TABLE 语句的正则表达式
    regex create_table_pattern(R"(CREATE\s+TABLE\s+(\w+)\s*\((.+)\))");

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

    if (regex_match(sql, match, create_table_pattern)) {
        // 匹配 CREATE TABLE 语句
        string table_name = match[1];
        string columns_str = match[2];

        cout << "CREATE TABLE statement" << "\ntable  name:" << table_name << "\ncolumn list:" <<" (" << columns_str << ")\n" << endl;

        //......调用CREATE函数操作
        db->createTable(QString(QString::fromLocal8Bit(table_name.data())));//根据表名创建表

    } else if (regex_match(sql, match, insert_into_pattern)) {
        // 匹配 INSERT INTO 语句
        string table_name = match[1];
        string columns_str = match[2];
        string values_str = match[3];

        cout << "INSERT statement " << "\ntable  name:" << table_name << "\ncolumn list:" <<" (" << columns_str << ") \nVALUES (" << values_str << ")\n" << endl;

        //db->

        //......调用 INSERT 函数操作



    } else if (regex_match(sql, match, delete_from_pattern)) {
        // 匹配 DELETE FROM 语句
        string table_name = match[1];
        string condition = match[2];

        cout << "DELETE statement " << "\ntable  name:" << table_name << " \nWHERE " << condition << "\n" <<endl;

        //......调用 DELETE 函数操作



    } else if (regex_match(sql, match, update_pattern)) {
        // 匹配 UPDATE 语句
        string table_name = match[1];
        string set_clause = match[2];
        string condition = match[3];

        cout << "UPDATE statement" << "\ntable  name:" << table_name << " \nSET clause:" << set_clause << " \nWHERE " << condition << "\n" << endl;

        //......调用 UPDATE 函数操作

    } else if (regex_match(sql, match, drop_table_pattern)) {
        // 匹配 DROP TABLE 语句
        string table_name = match[1];

        cout << "DROP TABLE statement " << "\ntable  name:" << table_name << "\n" << endl;

        //......调用 DROP 函数操作

        db->dropTable(QString(QString::fromLocal8Bit(table_name.data())));//根据表名删除表

    } else if (regex_match(sql, match, create_index_regex)) {
        cout << "CREATE INDEX statement" << "\nindex name:" << match[3] << "\ntable name:" << match[4] << "\ncolumn list:" << "(" << match[5] << ")\n" <<endl;
        if (match[1].matched) {
            cout << "\nUNIQUE 选项已启用" << endl;
        }
        if (match[2].matched) {
            cout << "\nCLUSTERED 选项已启用" << endl;
        }
        //......调用 CREATE INDEX 函数操作

    } else if (regex_match(sql, match, drop_index_regex)) {
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
            cout << " WHERE " << where_clause << endl;
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

    } else {
        cout << "Invalid SQL statement" << endl;
    }
}

//int main() {
//    string sql;
//    cout << "input SQL statement:";
//    getline(cin, sql);
//    parse_sql(sql);
//    // 测试 CREATE TABLE 语句
//    /*sql = "CREATE TABLE students (id INT, name VARCHAR(20), age INT)";
//    parse_sql(sql);

//    // 测试 INSERT INTO 语句
//    sql = "INSERT INTO students (id, name, age) VALUES (1, 'Alice', 18)";
//    parse_sql(sql);

//    // 测试 DELETE FROM 语句
//    sql = "DELETE FROM students WHERE id = 1";
//    parse_sql(sql);

//    // 测试 UPDATE 语句
//    sql = "UPDATE students SET name = 'Bob', age = 20 WHERE id = 2";
//    parse_sql(sql);

//    // 测试 DROP TABLE 语句
//    sql = "DROP TABLE students";
//    parse_sql(sql);

//    // 测试 CREATE INDEX 语句
//    sql = "CREATE INDEX idx_name ON table_name (column1, column2) INCLUDE (column3)";
//    parse_sql(sql);

//    // 测试 CREATE INDEX 语句
//    sql = "CREATE INDEX idx_name ON table_name (column1, column2)";
//    parse_sql(sql);

//    // 测试 DROP INDEX 语句
//    sql = "DROP INDEX idx_name ON table_name";
//    parse_sql(sql);

//    // 测试 SELECT 语句，包括 WHERE、GROUP BY、HAVING和ORDER BY 子句
//    sql = "SELECT name, age FROM students WHERE age >= 18 GROUP BY age HAVING COUNT(*) > 1 ORDER BY name";
//    parse_sql(sql);

//    // 测试 SELECT 语句，不包括 WHERE、GROUP BY、HAVING和ORDER BY 子句
//    sql = "SELECT * FROM students";
//    parse_sql(sql);

//    // 测试无效的 SQL 语句
//    sql = "INVALID SQL ";
//    parse_sql(sql);
//*/
//    return 0;
//}


//sql语句处理
void SqlAnalysis::trim_create(QString input,QVector<QString>* output){
    input = input.replace(QRegExp(",")," ");
    QStringList list = input.split(" ");
    for(auto &s : list){
        if(s=="")
            continue;
        output->append(s);
    }
}