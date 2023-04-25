#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
/* uncomment for applications that use vectors */
#include <vector>

//#include "mysql_connection.h"
//#include "mysql_driver.h"
#include <mariadb/conncpp.hpp>

//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>
#include "chatUserEntry.h"

#ifndef CHATUSERDB_H
#define CHATUSERDB_H

#define DB_URL "jdbc:mariadb://localhost:3306/restChatBestChat"
#define USER "root"
#define PASS "srabieh"

using namespace std;

class chatUserDB {
public:
    chatUserDB();
    vector<chatUserEntry> find(string search);
    vector<chatUserEntry> findByUsername(string username);
    //unnecessary?
    /*
    vector<contactEntry> findByLast(string last);
    vector<contactEntry> findByType(string type);
    */
    chatUserEntry fetchEntry(string id);
    void addEntry(string username,string password,string email);
    void editEntry(string idnum,string username,string password,string email);
    void deleteEntry(string idnum);
    
    int getNumEntries();
private:
    const string db_url=DB_URL;
    const string user=USER;
    const string pass=PASS;
    sql::Driver* driver;
    sql::Properties properties;
    std::unique_ptr<sql::Connection> conn;

};

#endif /* contactDB_H */
