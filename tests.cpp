#include <UnitTest++/UnitTest++.h>

#include <string>
#include <exception>

#include "Logging.h"

#include "Util/Util.h"
#include "Util/Connection.h"
#include "HTTP/HttpHeader.h"
#include "Util/Configuration.h"
#include "Util/Resource.h"

#include "HTTP/HttpHeader.h"
#include "HTTP/HttpRequestParser.h"

TEST(Sanity_TEST) {
	CHECK(true);

	CHECK_EQUAL(1,1);
	CHECK_EQUAL("123abc", std::string("123abc"));

	CHECK_CLOSE(1.0/25.0, 0.04, 0.01);
}

SUITE(Utility) {
	TEST(ResourceLoader_TEST) {
		ResourceLoader loader = ResourceLoader("../TestResources/");

		/* Basic file loading */
		Resource* newResource = loader.load("NewFile.txt");	
		REQUIRE CHECK(newResource != nullptr);

		delete newResource;

		/* Testing contents of a file that has been loaded */
		Resource* resource = loader.load("NewFile.txt");
		REQUIRE CHECK(resource != nullptr);
		resource->copyData("Hello World!", sizeof("Hello World!"));

		resource->save();
		
		delete resource;	
		resource = loader.load("NewFile.txt");
		REQUIRE CHECK(resource != nullptr);
		CHECK_EQUAL("Hello World!", resource->getData());
		delete resource;	
			
		/* Testing the size of a file */
		Resource* testImage = loader.load("Lenna.png");
		REQUIRE CHECK(testImage != nullptr);
		
		CHECK_EQUAL(473831, testImage->size());

		delete testImage;
	}

	/*
	TEST(Connection_TEST) {
		Connection conn;

		conn.setPort(7); //Echo service
		conn.setAddress("127.0.0.1");
		conn.setProtocol(PROTOCOL_TCP);

		REQUIRE CHECK_EQUAL(CONNECTION_OK, conn.createSocket());
		REQUIRE CHECK_EQUAL(CONNECTION_OK, conn.createConnection());

		conn.sendData("Hello World!", sizeof("Hello World!"));

		char buf[16] {0};
		conn.receiveData(buf, 16);	

		CHECK_EQUAL("Hello World!", buf);
	}
	*/
}
//"GET /hello.htm HTTP/1.1\User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n";
SUITE(HTTP) {
	TEST(HttpRequestParser_TEST) {
		std::string stringRequest = "GET /hello.html HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.example.com\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive";

		HttpHeader* httpRequest = HttpRequestParser::parse(stringRequest, nullptr, nullptr);

		/* Basic http request */
		CHECK_EQUAL("GET", httpRequest->getRequestMethod());
		CHECK_EQUAL("/hello.html", httpRequest->getRequestResource());
		CHECK_EQUAL("HTTP/1.1", httpRequest->getRequestVersion());

		CHECK_EQUAL("www.example.com", httpRequest->getField("Host"));
		CHECK_EQUAL("en-us", httpRequest->getField("Accept-Language"));
		CHECK_EQUAL("Keep-Alive", httpRequest->getField("Connection"));
	}
}

int main() {
	Log.init();

	ConsoleLoggingHandler consoleLogger;
	Log.addHandler(&consoleLogger);

	Log.info("Logger Initialized");

	return UnitTest::RunAllTests();
}
