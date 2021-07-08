#pragma once
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

using namespace std;
using namespace sql;
using namespace sql::mysql;

class Mysql
{
private:
    /* data */
public:
    MySQL_Driver *driver = NULL;
    Connection *con = NULL;
    Statement *state = NULL;
    Mysql(string address, string username, string password);
    ~Mysql();
};