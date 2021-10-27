#include <iostream>
#include "Connection.h" 

void callback(ConnectionDetails cd){
	Connection conn(cd);



	conn.closeConnection();
}

int main(){

	BoundConnection bc;
	bc.setPort(80);
	bc.setConnectionCallback(callback);

	if(int rc = bc.createSocket() != 0){
		std::cout << "Error Creating Socket" << std::endl;
		std::cout << "Return code " << rc << std::endl;
		return 1;
	}	

	if(bc.bindConnection() != 0){
		std::cout << "Error Binding Connection" << std::endl;
		return 2;
	}

	bc.listenToConnection();

	bc.closeConnection();
	
	return 0;
}
