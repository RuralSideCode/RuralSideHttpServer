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

		int send(void* data, int size);
		void recive(void* data, int& size);

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
