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
  map<string, string> allUsers;
  map<string, string> allUsersEmails;
  
  allUsers["ooniTest"] = "cornflower";
  allUsers["samTest"] = "yes";
  allUsers["jimTest"] = "skon";
  
  
	
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
	string password = req.matches[2];
    string result;
    vector<string> empty;
    cout << username << " joins" << endl;
    
    // Check if user with this name exists
    if (password == allUsers[username]) {
		// Add user to messages map
			messageMap[username]=empty;
			onlineUsers[username]="user logged in";
			result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
    	
    } else {
		// if username or password incorrect, do not login user
    	result = "{\"status\":\"failure\"}";
    }
    res.set_content(result, "text/json");
  });
  
  svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
	res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
	string password = req.matches[2];
	string email = req.matches[3];
	cout << username << " " << password << " " << email << endl;
	string result;
	
	// ADD: make it so you can't register as a user that already exists
	
	bool isEmailTaken = false;
	
	for (auto checkEmails: allUsersEmails) {
		string currEmail = allUsersEmails[checkEmails.first];
		if (email==currEmail) {
			isEmailTaken=true;
			break;
		}
	}
	
	
	if (allUsers.count(username)) {
    	result = "{\"status\":\"name taken\"}";
	} else if (isEmailTaken){
		result= "{\"status\":\"email taken\"}";
	} else if (password.length() < 6) {
		result = "{\"status\":\"password short\"}";
	} else {
		allUsers[username] = password;
		allUsersEmails[username] = email;
		result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
	}
    res.set_content(result, "text/json");
	
  });

   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string message = req.matches[2];
	string result; 
	
    if (!messageMap.count(username)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		addMessage(username,message,messageMap);
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
  
  
  //back to skon
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
