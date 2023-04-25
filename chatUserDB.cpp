#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "chatUserDB.h"
#include "chatUserEntry.h"



chatUserDB::chatUserDB() {
  	// Instantiate Driver
  	driver = sql::mariadb::get_driver_instance();
  	
 	// Configure Connection
  	// The URL or TCP connection string format is
  	// ``jdbc:mariadb://host:port/database``.
  	sql::SQLString url(db_url);

    // Use a properties map for the other connection options
  	sql::Properties my_properties({{"user", user}, {"password",pass}});
  	// Save properties in object
  	properties = my_properties;

    // Establish Connection
  	std::unique_ptr<sql::Connection> my_conn(driver->connect(db_url, properties));
    
    // Check success
    if (!my_conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
   	
   	// Save connection in object
   	conn = std::move(my_conn);
   	
}




vector<chatUserEntry> chatUserDB::find(string search) {

	vector<chatUserEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM Users WHERE ID = '" + search + "'");
    
    // Loop through and print results
    while (res->next()) {
    	chatUserEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Email"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}




vector<chatUserEntry> chatUserDB::findByUsername(string username) {

	vector<chatUserEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM Users WHERE Username like '%"+username+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	chatUserEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Email"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;
}



void chatUserDB::addEntry(string username,string password,string email){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
  	
  	stmnt->executeQuery("INSERT INTO Users(Username,Password,Email) VALUES ('"+username+"','"+password+"','"+email+"')");
}


chatUserEntry chatUserDB::fetchEntry(string id){

	chatUserEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM Users WHERE ID = '"+id+"'");
    
    // Get first entry
    if (res->next()) {
    	entry = chatUserEntry(res->getString("Username"),res->getString("Password"),
			res->getString("Email"),
	    	res->getString("ID"));
    }
    return entry;
}

int chatUserDB::getNumEntries () {
	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
	sql::ResultSet *res = stmnt->executeQuery("SELECT COUNT(ID) AS entry_count FROM Users");
	int entries = 0;
	if (res->next()) {
  		entries = res->getInt("entry_count");
	}
	
	return entries;
}


//leftovers
/*
void contactDB::editEntry(string idnum,string first,string last,string phone, string type){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
  	stmnt->executeQuery("UPDATE contacts SET First = '"+first+"', Last ='"+last+"', Phone ='"+phone+"', Type ='"+type+"' WHERE ID='"+idnum+"'");
  	
}


void contactDB::deleteEntry(string idnum){
  // Establish Connection
  std::unique_ptr<sql::Connection>  conn(driver->connect(db_url, properties));
    
  if (!conn) {
   	cerr << "Invalid database connection" << endl;
   	exit (EXIT_FAILURE);
  }

  std::auto_ptr<sql::Statement> stmt(conn->createStatement());

  stmt->execute("DELETE FROM contacts WHERE ID='"+idnum+"'");
}
*/
