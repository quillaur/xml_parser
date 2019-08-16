// #include <mariadb/my_global.h>
#include <mariadb/mysql.h>
#include <iostream> // For std:: to be imported.

int main()
{
    const char * host = "localhost";
    const char * user = "****";
    const char * passwd = "******";
    const char * db = "******";
    const char * unix_socket = "/var/run/mysqld/mysqld.sock";
    unsigned long flags = 0;
    unsigned int port = 0;
    MYSQL * mysql = nullptr;
    MYSQL * mysql_con = nullptr;

    mysql = mysql_init(nullptr);
    mysql_con = mysql_real_connect(mysql,
                           host,
                           user,
                           passwd,
                           db,
                           port,
                           unix_socket,
                           flags);

    if (mysql_con == NULL) 
    {
        std::cout << mysql_error(mysql) << std::endl;
    }
    else
    {
        std::cout << "I am connected !" << std::endl;
    }

    mysql_close(mysql);

    return 0;
}