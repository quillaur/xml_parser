// Compilo command: g++ sql_functions.cpp -o sql_connect_test -std=c++17 -Wall -L/usr/lib/mariadb -lmariadbclient
#include <mariadb/mysql.h>
// #include "read_config.cpp"
#include <vector>
#include <iostream> // For std:: to be imported.

MYSQL * mysql_connection()
{
    // Take params from config.
    // Get configuration file
    std::ifstream my_config_file("config.cfg");

    // Check file opening
    if (!my_config_file.is_open())
    {
        std::cout << "config.cfg did not open !" << std::endl;
        return (nullptr);
    }

    const char * host = get_config_param(&my_config_file, "host").c_str();
    const char * user = get_config_param(&my_config_file, "user").c_str();
    const char * passwd = get_config_param(&my_config_file, "mdp").c_str();
    const char * db = get_config_param(&my_config_file, "database").c_str();
    const char * unix_socket = 0;
    unsigned long flags = 0;
    unsigned int port = 0;
    MYSQL * mysql = nullptr;
    MYSQL * mysql_con = nullptr;

    my_config_file.close();

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

    return (mysql);
}

bool get_tools_to_scan(MYSQL * mysql, std::vector<std::string> & results_vector)
{
    // Return list of tool names as a set
   
    const char * query = "SELECT name FROM Tool";
    int error = mysql_real_query(mysql, query, strlen(query));

    if (error) 
    {
        std::cout << mysql_error(mysql) << std::endl;
        return (false);
    }

    MYSQL_RES * result = mysql_store_result(mysql);
    if (result)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) 
        { 
            // std::cout << "Tool name: " << row[0] << std::endl;
            std::string tool_name(row[0]);
            results_vector.push_back(tool_name);
        }

        mysql_free_result(result);
    }

    return (true);
}

// Escape special characters function
std::string escape(const std::string & src) {
    std::stringstream dst;
    for (char ch : src) {
        switch (ch) {
            case '&': dst << "&amp;"; break;
            case '\'': dst << "&apos;"; break;
            case '"': dst << "&quot;"; break;
            case '<': dst << "&lt;"; break;
            case '>': dst << "&gt;"; break;
            case '?': dst << "\\?"; break;
            case '*': dst << "\\*"; break;
            case '[': dst << "\\["; break;
            case ']': dst << "\\]"; break;
            case '(': dst << "\\("; break;
            case ')': dst << "\\)"; break;
            case '^': dst << "\\^"; break;
            case '$': dst << "\\$"; break;
            case '.': dst << "\\."; break;
            case '|': dst << "\\|"; break;
            case '+': dst << "\\+"; break;
            case '@': dst << "\\@"; break;
            case '{': dst << "\\{"; break;
            case '}': dst << "\\}"; break;
            default: dst << ch; break;
        }
    }

    return dst.str();
}

bool build_entity_names_regex(std::vector<std::regex> & regex_vector)
{
    std::string entity_names_str = "";
    MYSQL * mysql = mysql_connection();

    if (mysql)
    {
        std::cout << "I am connected !" << std::endl;

        std::vector<std::string> entity_names;
        get_tools_to_scan(mysql, entity_names);

        int count = 0;

        if (!entity_names.empty())
        {
            std::cout << entity_names.size() << " tools fetched." << std::endl;

            for (auto it = entity_names.begin(); it != entity_names.end(); it++)
            {
                std::string name(*it);
                std::string entity_name_str(escape(name));

                std::regex entity_regex(entity_name_str, std::regex::icase);

                regex_vector.push_back(entity_regex);
                
                count += 1;
                if (count == 100)
                {
                    break;
                }
            }
        }
        // Test print
        // std::cout << "i: " << count << std::endl;
        mysql_close(mysql);
    }

    return (true);
}
