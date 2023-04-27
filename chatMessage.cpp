#include "chatMessage.h"

chatMessage::chatMessage() {
}

chatMessage::chatMessage(sql::SQLString usernameOfSender, sql::SQLString text) {
	sender = usernameOfSender;
	message = text;
}

string chatMessage::getSender() {
	return sender;
}

string chatMessage::getMessage() {
	return message;
}