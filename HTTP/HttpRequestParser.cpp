#include "HttpRequestParser.h"

#include <string>

#include <iostream>

HttpHeader* HttpRequestParser::parse(const char* header, void* data, int* dataSize){
	std::stringstream stream(header);

	return HttpRequestParser::parse(stream, data, dataSize);
}

HttpHeader* HttpRequestParser::parse(std::stringstream& header, void* data, int* dataSize){

	std::string line;

	while(std::getline(header, line, '\n')){
		std::cout << line << std::endl; 
		std::stringstream ss_line(line);

		std::string request;
		std::string request_param;
		std::getline(ss_line, request, ':');
		std::getline(ss_line, request_param);

		std::cout << request << std::endl;
		std::cout << request_param << std::endl;
		std::cout << std::endl;
		
	}

	return nullptr;
}

void HttpRequestParser::addFieldToHeader(struct HttpHeader& header, std::stringstream& ss){
	std::string fieldName;
	std::string fieldValue;

	//Get fieldName
	std::getline(ss, fieldName, ':');

	//Get fieldValue
	std::getline(ss, fieldValue);

	int index = getFieldIndex(fieldName);

	if(index == -1)
		std::cout << "getFieldIndex() in HttpRequestParser returned a negative index!" << std::endl;
	
	reinterpret_cast<char*>(((char*)&header) + index * 4) = fieldValue.c_str();
}

int HttpRequestParser::getFieldIndex(std::string& fieldName){
	for(int i = 0; i < sizeof(httpHeaderKey); i++){
		if(fieldName == httpHeaderKey[i])
			return i;
	}

	return -1;
}

const char* HttpRequestParser::httpHeaderKey[215] = {	
	"A-IM",
	"Accept",
	"Accept-Additions",
	"Accept-CH",
	"Accept-Charset",
	"Accept-Datetime",
	"Accept-Encoding",
	"Accept-Features",
	"Accept-Language",
	"Accept-Patch",
	"Accept-Post",
	"Accept-Ranges",
	"Access-Control",
	"Access-Control-Allow-Credentials",
	"Access-Control-Allow-Headers",
	"Access-Control-Allow-Methods",
	"Access-Control-Allow-Origin",
	"Access-Control-Max-Age",
	"Access-Control-Request-Headers",
	"Access-Control-Request-Method",
	"Age",
	"Allow",
	"ALPN",
	"Alt-Svc",
	"Alt-Used",
	"Alternates",
	"AMP-Cache-Transform",
	"Apply-To-Redirect-Ref",
	"Authentication-Control",
	"Authentication-Info",
	"Authorization",
	"C-Ext",
	"C-Man",
	"C-Opt",
	"C-PEP",
	"C-PEP-Info",
	"Cache-Control",
	"Cache-Status",
	"Cal-Managed-ID",
	"CalDAV-Timezones",
	"CDN-Loop",
	"Cert-Not-After",
	"Cert-Not-Before",
	"Close",
	"Compliance",
	"Connection",
	"Content-Base",
	"Content-Disposition",
	"Content-Encoding",
	"Content-ID",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-MD5",
	"Content-Range",
	"Content-Script-Type",
	"Content-Style-Type",
	"Content-Transfer-Encoding",
	"Content-Type",
	"Content-Version",
	"Cookie",
	"Cookie2",
	"Cost",
	"DASL",
	"Date",
	"DAV",
	"Default-Style",
	"Delta-Base",
	"Depth",
	"Derived-From",
	"Destination",
	"Differential-ID",
	"Digest",
	"Early-Data",
	"EDIINT-Features",
	"ETag",
	"Expect",
	"Expect-CT",
	"Expires",
	"Ext",
	"Forwarded",
	"From",
	"GetProfile",
	"Hobareg",
	"Host",
	"HTTP2-Settings",
	"If",
	"If-Match",
	"If-Modified-Since",
	"If-None-Match",
	"If-Range",
	"If-Schedule-Tag-Match",
	"If-Unmodified-Since",
	"IM",
	"Include-Referred-Token-Binding-ID",
	"Isolation",
	"Keep-Alive",
	"Label",
	"Last-Modified",
	"Link",
	"Location",
	"Lock-Token",
	"Man",
	"Max-Forwards",
	"Memento-Datetime",
	"Message-ID",
	"Meter",
	"Method-Check",
	"Method-Check-Expires",
	"MIME-Version",
	"Negotiate",
	"Non-Compliance",
	"OData-EntityId",
	"OData-Isolation",
	"OData-MaxVersion",
	"OData-Version",
	"Opt",
	"Optional",
	"Optional-WWW-Authenticate",
	"Ordering-Type",
	"Origin",
	"OSCORE",
	"OSLC-Core-Version",
	"Overwrite",
	"P3P",
	"PEP",
	"Pep-Info",
	"PICS-Label",
	"Position",
	"Pragma",
	"Prefer",
	"Preference-Applied",
	"ProfileObject",
	"Protocol",
	"Protocol-Info",
	"Protocol-Query",
	"Protocol-Request",
	"Proxy-Authenticate",
	"Proxy-Authentication-Info",
	"Proxy-Authorization",
	"Proxy-Features",
	"Proxy-Instruction",
	"Proxy-Status",
	"Public",
	"Public-Key-Pins",
	"Public-Key-Pins-Report-Only",
	"Range",
	"Redirect-Ref",
	"Referer",
	"Referer-Root",
	"Repeatability-Client-ID",
	"Repeatability-First-Sent",
	"Repeatability-Request-ID",
	"Repeatability-Result",
	"Replay-Nonce",
	"Resolution-Hint",
	"Resolver-Location",
	"Retry-After",
	"Safe",
	"Schedule-Reply",
	"Schedule-Tag",
	"Sec-Token-Binding",
	"Sec-WebSocket-Accept",
	"Sec-WebSocket-Extensions",
	"Sec-WebSocket-Key",
	"Sec-WebSocket-Protocol",
	"Sec-WebSocket-Version",
	"Security-Scheme",
	"Server",
	"Set-Cookie",
	"Set-Cookie2",
	"SetProfile",
	"SLUG",
	"SoapAction",
	"Status-URI",
	"Strict-Transport-Security",
	"SubOK",
	"Subst",
	"Sunset",
	"Surrogate-Capability",
	"Surrogate-Control",
	"TCN",
	"TE",
	"Timeout",
	"Timing-Allow-Origin",
	"Title",
	"Topic",
	"Traceparent",
	"Tracestate",
	"Trailer",
	"Transfer-Encoding",
	"TTL",
	"UA-Color",
	"UA-Media",
	"UA-Pixels",
	"UA-Resolution",
	"UA-Windowpixels",
	"Upgrade",
	"Urgency",
	"URI",
	"User-Agent",
	"Variant-Vary",
	"Vary",
	"Version",
	"Via",
	"Want-Digest",
	"Warning",
	"WWW-Authenticate",
	"X-Content-Type-Options",
	"X-Device-Accept",
	"X-Device-Accept-Charset",
	"X-Device-Accept-Encoding",
	"X-Device-Accept-Language",
	"X-Device-User-Agent",
	"X-Frame-Options"
};
