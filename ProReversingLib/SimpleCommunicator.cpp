#include "SimpleCommunicator.h"



string readStringWithNewline(tcp::socket& sock)
{
	boost::asio::streambuf buffer;
	boost::system::error_code error;
	boost::asio::read_until(sock, buffer, '\n', error);
	if (error)
		return string();

	istream is(&buffer);
	string s;
	getline(is, s);

	return s;

}


void writeStringWithNewline(tcp::socket& sock, const string& data)
{

	boost::system::error_code error;

	string sendData = data + '\n';

	boost::asio::write(sock, boost::asio::buffer(sendData.data(), sendData.length()));
	if (error)
		throw boost::system::system_error(error);

}


AsioServer::AsioServer( boost::asio::io_service& io_service, string (*message_handler) (const string&) )
	:acceptor_(io_service, tcp::endpoint(tcp::v4(), atoi(PORT)))
{

	for (;;)
	{
		sock_ = socket_ptr(new tcp::socket(acceptor_.get_io_service()));
		acceptor_.accept(*sock_);
		for(;;)
		{

			string cmd = readStringWithNewline(*sock_);

			if (cmd.empty())
				break;

			if (cmd == "exit")
			{
				acceptor_.get_io_service().stop();
				exit(0);
			}

			string res = message_handler(cmd);

			writeStringWithNewline(*sock_, res);
		}
	}
}


AsioClient::AsioClient( boost::asio::io_service& io_service )
	:sock_(io_service)
{


	tcp::resolver resolver(sock_.get_io_service());
	tcp::resolver::query query(tcp::v4(), "localhost", PORT);
	tcp::resolver::iterator iterator = resolver.resolve(query);

	boost::asio::connect(sock_, iterator);

}


std::string AsioClient::remoteExecute( const string& data )
{
	writeStringWithNewline(sock_, data);
	return readStringWithNewline(sock_);
}