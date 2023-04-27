//
//  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "httplib.h"
#include <mariadb/conncpp.hpp>
#include "chatUserDB.h"
#include "chatUserEntry.h"
#include "chatMessage.h"
//IF EVERYTHING BREAKS: uncomment?
//#include <sstream>
//#include <stdexcept>
#define DB_URL "jdbc:mariadb://localhost:3306/restChatBestChat"
#define USER "root"
#define PASS "srabieh"

using namespace httplib;
using namespace std;

const int port = 5005;

void addMessage(string username, string message, map<string,vector<string>> &messageMap) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result += "]}";
	messageMap[username].clear();
	return result;
}




int main(void) {


  Server svr;
  int nextUser=0;
  map<string,vector<string>> messageMap;
  map<string, string> onlineUsers;
  
// DELETE AFTER PUSHING LAB 3 CHANGES
//   map<string, string> allUsers;
//   map<string, string> allUsersEmails;
//   
//   allUsers["ooniTest"] = "cornflower";
//   allUsers["samTest"] = "yes";
//   allUsers["jimTest"] = "skon";
  
  //to establish DB connection + send requests: add objects & pull data
  chatUserDB connChatDB;
  
	
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


	//logging in
  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
	string password = req.matches[2];
    string result;
    vector<string> empty;
    cout << username << " is trying to log in" << endl;
    
    
    vector<chatUserEntry> checkUsername = connChatDB.findByUsername(username);
    
    // Check if username & password match
    if (checkUsername[0].getPassword()==password) {
		// Add user to messages map
			messageMap[username]=empty;
			onlineUsers[username]="user logged in";
			result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
			cout<<username<<" logged in successfully\n";
    } else {
		// if username or password incorrect, do not login user
    	result = "{\"status\":\"failure\"}";
    }
    res.set_content(result, "text/json");
  });
  
  //registering
  svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
	res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
	string password = req.matches[2];
	string email = req.matches[3];
	cout << username << " " << password << " " << email << endl;
	string result = "{\"status\":\"failure, status not redefined\"}";
	cout<<"result is "<<result<<" at the beginning of the register function"<<endl;
	
	
	bool isUserTaken = false;
	bool isEmailTaken = false;

	bool passTooShort = false;
	if (password.length() < 6) {
		passTooShort = true;
	}
	
	
	cout<<"successfully reached line 121\n";
	int numUserEntries = connChatDB.getNumEntries();
	cout<<"numUserEntries is "<<numUserEntries<<endl;
	//int numUserEntriesInt = stoi(numUserEntries);
	//CHECK LATER: <= num?
	
	
	for (int i = 1; i < numUserEntries; i++) {
		if (passTooShort==true) {
			result = "{\"status\":\"password short\"}";
			break;
		}
		
		vector<chatUserEntry> currentEntry = connChatDB.find(to_string(i));
		if (currentEntry[0].getUsername()==username) {
			isUserTaken = true;
			result = "{\"status\":\"name taken\"}";
			break;
		} else if (currentEntry[0].getEmail()==email){
			isEmailTaken = true;
			result= "{\"status\":\"email taken\"}";
			break;
		}
	}
	
	if (passTooShort==false&&isUserTaken==false&&isEmailTaken==false) {
		connChatDB.addEntry(username, password, email);
		result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
	}
	
	cout<<"result is "<<result<<endl;
	
	res.set_content(result, "text/json");
	
  });

	//sending
   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string message = req.matches[2];
	string result; 
	
    if (!messageMap.count(username)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		addMessage(username,message,messageMap);
		connChatDB.addDBMessage(username, message);
		result = "{\"status\":\"success\"}";
	}
    res.set_content(result, "text/json");
  });
  
  
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string username = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = getMessagesJSON(username,messageMap);
    res.set_content(resultJSON, "text/json");
  });
  
  
//new stuff: Sam & Dylan
svr.Get(R"(/chat/list)", [&](const Request& req, Response& res) {
	res.set_header("Access-Control-Allow-Origin","*");
	bool start = true;
	string onlineUsersJSON = "{\"onlineUsers\":[";
	for (auto const &listUser: onlineUsers)
	{		
		if (not start) onlineUsersJSON += ",";
		onlineUsersJSON += "\"" + listUser.first + "\"";
		start = false;
	}
	onlineUsersJSON+="]}";
	res.set_content(onlineUsersJSON, "text/json");
});

svr.Get(R"(/chat/leave/(.*))", [&](const Request& req, Response& res) {
	string result;
	res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	cout << username << " is the username to be erased" << endl;
	onlineUsers.erase(username);
	if (!onlineUsers.count(username)) {
		result = "{\"status\":\"success\"}";
		cout << username << " leaves" << endl;
		for (auto checkForUsers: onlineUsers) {
			cout << checkForUsers.first << " logged in" << endl;
		}
	} else {
		result = "{\"status\":\"failure\"}";
	}
	res.set_content(result, "text/json");
	
});

svr.Get(R"(/chat/history)", [&](const Request& req, Response& res) {
	res.set_header("Access-Control-Allow-Origin","*");
	string result = "{\"messages\":[";
	
	vector<chatMessage> allDBMessages = connChatDB.getAllDBMessages();
	
	cout<<"allDBMessages.size() is "<<allDBMessages.size()<<endl;
	
	//CHECK LATER: <= numAll?
	for (int i = 0; i < allDBMessages.size(); i++) {
		string author = allDBMessages[i].getSender();
		string messageContent = allDBMessages[i].getMessage();
		
		cout<<"author is "<<author<<endl;
		cout<<"messageContent is "<<messageContent<<endl;
		
		result+="{\"user\":\""+author+"\",\"message\":\""+messageContent+"\"},";
		
		cout<<"i = "<<i<<", result = "<<result<<endl;
	}
	
	// Remove the trailing comma after the last message
    if (allDBMessages.size() > 0) {
        result.erase(result.size()-1);
    }
	
	result+="]}";
	cout<<"\nresult is:\n"<<result<<endl<<endl;
	res.set_content(result,"text/json");
});
  
  
  //back to skon
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
