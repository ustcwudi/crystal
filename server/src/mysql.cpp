#include "mysql.h"
#include <iostream>

Mysql::Mysql(string address, string username, string password)
{
    try
    {
        cout << "connecting mysql" << endl;
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://" + address, username, password);
        state = con->createStatement();
        state->execute("use earth;");
        cout << "db connected" << endl;
    }
    catch (sql::SQLException &e)
    {
        cout << "connecting mysql error " << e.getSQLStateCStr() << endl;
    }
}

Mysql::~Mysql()
{
    delete state;
    delete con;
}
