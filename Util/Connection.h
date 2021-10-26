#include "Util.h"

#include <functional>

/**
 * Connection Details is used to keep track of a connection.
 * It can be used to pass around a connection for other programs to use it
 */
struct ConnectionDetails{
	SFD_t socketfd; /**<The socket file descriptor used to identify a web socket*/
	struct addrinfo address; /**<The address to connect to. This is only needed in a bound socket to send and receive data*/
};

/** Connection is used to connect to any addresses and ports. Once connected to these ports you are then able to send and receive data. This class is useful when for when you are wanting to make request to a server. It can also be used by BoundConnection to create connections from a server to a client without working with a BoundConnection.
 */
class Connection{
	public:
	
		/** Default constructor for Connection*/
		Connection();

		/**Creates a Connection object with a already existing ConnectionDetails
		 * @param cd The ConnectionDetails to use when constructing this Connection
		 */
		Connection(ConnectionDetails cd);

		~Connection();

		/**This will set the protocol of a connection.
		 * This will also the the socket type automatically if PROTOCOL_TCP or PROTOCOL_UDP is used.
		 * @param protocol The protocol to use.*/
		void setProtocol(Protocol_t protocol);

		/**This will set the address that the connection should try to use.
		 * A url or ip address can be sent (through a cstring)
		 * @param address The url or ip address to attempt to connect to*/
		void setAddress(const char* address);

		/**This will set the socket type the connection should use when connecting.
		 * This should not be used if you are using a TCP or UDP protocol as it will be set automatically in setProtocol(Protocol_t protocol).
		 * @param socketType The socket type to use*/
		void setSocketType(SocketType_t socketType);

		/**This will set the port that the connection should attempt to connect to.
		 * @param _port The port to use. This can be a number (i.e 8080), or it can also be a well known port name (i.e http or ftp)*/
		void setPort(const char* _port);

		/**This will set the port that the connection should attempt to connect to.
		 * This function is another version of Connection::setPort(const char* _port), except this will only accept and integer value.
		 * @param _port The port to use*/
		void setPort(int _port);

		/**This will use the available data given to Connection, and will try to make a connection.
		 * Note that Connection::createSocket() should be used before this, otherwise it will not attempt to connect.
		 * @return A return code. 0 on success. 1 on invalid socket. 2 on failed attempt to connect*/
		int createConnection();

		/**This will close a connection*/
		void closeConnection();

		/**This will create a socket to connect to.
		 * All data should be set before calling this (port, address, protocol, etc.)
		 * @return Returns a return code. 0 on success. 1 on the unablity to find a valid address. 2 on failure to create a socket.*/
		int createSocket();

		/**This will return the ConnectionDetails of this Connection.
		 * These details can be used to create a copy of this Connection
		 * @return Returns the ConnectionDetails of this Connection*/
		ConnectionDetails getConnectionDetails();

		/**Sends buffer of data (of bufferSize)
		@return Amount of data sent in bytes */
		int sendData(const void* buffer, int bufferSize);

		/**Receives data and puts data in buffer (of bufferSize)
		@return Amount of data received in bytes*/
		int receiveData(void* buffer, int bufferSize);

	private:
		SFD_t socketfd = -1;
		struct addrinfo* addressInfo;

		struct addrinfo addressHints;

		const char* port = "";
		const char* address = "";
};

/**BoundConnection is used as a way for a machine to listen on a port. These can only listen to local ports (thus why there is no way to set address). Once you are listening you will be able to set callbacks for when connections are made. BoundConnections should be used when setting up a server (to serve clients that connect).
 */
class BoundConnection{
	public:
		
		/**Constructor for BoundConnection*/
		BoundConnection();
		~BoundConnection();

		/*Used to set the port listend to by a BoundConnection. Note that this method accepts both a url and a port number (ex. "8080").
		 * @param _port The port for the BoundConnection to listen to. Note that this can accept both a url and a port number (ex. "8080")*/
		void setPort(const char* _port);

		/*Same as BoundConnection::setPort(const char* _port) except this is able to take a true integer value
		 * @param _port The integer value of a port for BoundConnection to listen to*/
		void setPort(int _port);

		/**Sets the BoundConnection protocol to use (TCP, UDP, etc.)
		 * This will also automatically set the socket type if the connection is TCP or UDP
		 * @param protocol The protocol for BoundConnection to use while communicating*/
		void setProtocol(Protocol_t protocol);

		void setConnectionCallback(std::function<void(ConnectionDetails)> callback);

		/**Sends data accross a bound connection
		 * @param buffer The buffer of data to send
		 * @param bufferSize The size of const void* buffer
		 * @param connection The connection that we should send data through
		 * @return Returns the number of bytes that were successfully sent*/
		int sendData(const void* buffer, int bufferSize, ConnectionDetails connection);

		/**Receives data accross a bound connection
		 * @param buffer The buffer to receive data through
		 * @param bufferSize Size of void* buffer
		 * @param ConnectionDetails The connection that we should receive data through
		 * @return Returns the number of bytes that were received*/
		int receiveData(void* buffer, int bufferSize, ConnectionDetails connection);

		/**Creates a socket for the BoundConnection
		 * @return Returns and error code. 0 on success. 1 on an error of retreiving address information. 2 on error creating socket.*/
		int createSocket();

		/**Bind connection to the created socket.
		 * Note: You must call BoundConnection::createSocket() before trying to bind to it.
		 * @return Returns an error code. 0 on success. 1 on invalid socket. 2 on error binding*/
		int bindConnection();

		/**Will make the BoundConnection start listening to the port*/
		int listenToConnection();

		/**Closes the BoundConnection*/
		void closeConnection();

	private:
		SFD_t socketfd = -1;

		struct addrinfo* addressInfo;
		const char* port;

		Protocol_t protocol = PROTOCOL_TCP;
		SocketType_t socketType = SOCK_STREAM;

		int max_connections = 10;
		int current_connections = 0;

		struct ConnectionDetails* connections = nullptr;

		std::function<void(ConnectionDetails)> callback;

		struct addrinfo constructAddressHints();

		bool isRunning = true;

};
