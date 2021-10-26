#include <iostream>
#include "Connection.h" 

int main(){

	Connection conn;
	conn.setPort("80");
	conn.setAddress("example.com");
	conn.setProtocol(PROTOCOL_TCP);

	if(conn.createSocket() != 0){
		return 1;
	}
	if(conn.createConnection() != 0){
		return 2;
	}
	conn.closeConnection();
	return 0;
}
