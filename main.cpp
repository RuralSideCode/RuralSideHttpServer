#include <iostream>
#include "Connection.h" 

int main(){

	Connection conn;
	conn.setPort("80");
	conn.setAddress("example.com");
	conn.setProtocol(PROTOCOL_TCP);

	conn.createSocket();
	conn.createConnection();

	conn.sendData("Hello world!", 12);

	conn.closeConnection();

	return 0;
}
