# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14  -Wno-deprecated-declarations

RM= /bin/rm -f

all: PutHTML restChat

chatUserEntry.o: chatUserEntry.cpp chatUserEntry.h
	$(CC) -c $(CFLAGS) chatUserEntry.cpp
	
chatMessage.o: chatMessage.cpp chatMessage.h
	$(CC) -c $(CFLAGS) chatMessage.cpp

chatUserDB.o: chatUserDB.cpp chatUserDB.h
	$(CC) -c $(CFLAGS) -I/usr/include/cppconn chatUserDB.cpp
	
restChat.o: restChat.cpp httplib.h
	$(CC) -c $(CFLAGS) restChat.cpp


PutHTML:
	cp restChat.html /var/www/html/restChat/
	cp restChat.css /var/www/html/restChat/
	cp restChat.js /var/www/html/restChat/

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/restChat/

restChat : restChat.o chatUserEntry.o chatUserDB.o chatMessage.o
	$(CC) restChat.o chatUserEntry.o chatUserDB.o chatMessage.o -o restChat -L/usr/local/lib -lmariadbcpp
	# $(CXX)  -o restChat $(CXXFLAGS) restChat.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) 

clean:
	rm restChat *.o
