#pragma once

#include "Connection.h"

/**TCPConnection is the exacty same as Connection, except it is setup for TCP in the constructor*/
class TCPConnection : public Connection{

	public:
		TCPConnection();
		~TCPConnection();

	private:
};
