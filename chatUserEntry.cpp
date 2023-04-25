#include "chatUserEntry.h"

chatUserEntry::chatUserEntry() {
}

chatUserEntry::chatUserEntry(sql::SQLString u, sql::SQLString p, sql::SQLString e, sql::SQLString I) {
    username = u;
    password = p;
    email = e;
    ID=I;
}

string chatUserEntry::text() {
	string result = ID + ". ";
	result += username + " ";
	result += password + " ";
	result += email + " ";
	return result;

}

string chatUserEntry::getUsername() {
	return username;
}

string chatUserEntry::getEmail() {
	return email;
}

string chatUserEntry::getPassword() {
	return password;
}

string chatUserEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"username\":\"" + username + "\",";
	result += "\"password\":\"" + password + "\",";
	result += "\"email\":\"" + email + "\"}";
	return result;

}
