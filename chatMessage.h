#include <mariadb/conncpp.hpp>
#include <string>
#include <ctime>

using namespace std;

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

class chatMessage {
public:
	chatMessage();
	chatMessage(sql::SQLString usernameOfSender, sql::SQLString text);
	
	string sender;
	string message;
	string getSender();
	string getMessage();
	
private:

};

#endif