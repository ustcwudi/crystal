
#include "tcp.h"
#include "config.h"
#include "mysql.h"

int main()
{
    Config config("config.txt");
    Mysql mysql(config.mysql, config.mysql_username, config.mysql_password);
    char sql[100];
    sprintf(sql, "select id,web,earth,merge from server where ip='%s' and port=%d;", config.ip.c_str(), config.port);
    ResultSet *result = mysql.state->executeQuery(sql);
    if (result->next())
    {
        long id = result->getInt64("id");
        char sql[100];
        sprintf(sql, "update server set user='', activetime=CURRENT_TIMESTAMP where id=%ld;", id);
        mysql.state->executeUpdate(sql);
    }
    else
    {
        char sql[100];
        sprintf(sql, "insert into server(ip,port) values ('%s',%d);", config.ip.c_str(), config.port);
        mysql.state->execute(sql);
    }
    Tcp tcp(&config);
    tcp.Listen();

    return 0;
}