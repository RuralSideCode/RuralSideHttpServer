#include "Util.h"

class Connection{
	public:

		Connection();
		~Connection();

		void setProtocol(Protocol_t protocol);
		void setAddress(const char* address);
		void setSocketType(SocketType_t socketType);
		void setPort(const char* _port);
		void setPort(int _port);

		int createConnection();
		void closeConnection();
		int createSocket();

		//Sends buffer of data (of bufferSize)
		//Returns data sent in bytes
		int sendData(const void* buffer, int bufferSize);

		//Receives data and puts data in buffer (of bufferSize)
		//Returns data received in bytes
		int receiveData(void* buffer, int bufferSize);

		const struct addrinfo getAddressInfo();

	private:
		SFD_t socketfd = -1;
		struct addrinfo* addressInfo;

		struct addrinfo addressHints;

		const char* port = "";
		const char* address = "";
};

class BoundConnection{
	public:

	private:

};
