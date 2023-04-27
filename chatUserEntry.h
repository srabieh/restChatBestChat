#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef CHATUSERENTRY_H
#define CHATUSERENTRY_H

class chatUserEntry {
public:
     chatUserEntry();
     chatUserEntry(sql::SQLString u, sql::SQLString p, sql::SQLString e, sql::SQLString I);
     string text();
     string json();
     string getUsername();
     string getEmail();
     string getPassword();
     
     string username;
     string password;
     string email;
     string ID;

private:

};

#endif