#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <boost/asio.hpp>
#include <string>

#define MQ_NAME "ProReversing"
#define PORT "1245"

//#include <boost/array.hpp>

using namespace std;
using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socket_ptr;


class AsioServer
{
public:
	AsioServer(boost::asio::io_service& io_service, string (*message_handler) (const string&));	

private:

	tcp::acceptor acceptor_;
	socket_ptr sock_;
};


class AsioClient
{

public:
	AsioClient(boost::asio::io_service& io_service);



	string remoteExecute(const string& data);
	

private:


	 tcp::socket sock_;
};
