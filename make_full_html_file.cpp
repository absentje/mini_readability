#include <iostream>
#include <fstream>
// include project files
#include "make_full_html_file.h"
// Boost includes
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;
using namespace boost::asio;

MakeFullHtmlFile::MakeFullHtmlFile() {
	string url, host, path, protocol;
	std::cout << "Enter url (example: lenta.ru/news/2015/12/15/american)\n";
	std::getline(std::cin, url);
	parseUrl(url, host, path);
	std::cout << "Enter communication protocol(http or https)\n";
	std::getline(std::cin, protocol);
	string nameOfFile(url);
	urlToNameOfFile(nameOfFile);
	if (protocol == "https") {
		httpsConnect(host, path, nameOfFile);
	}
	else {
		if (protocol == "http") {
			httpConnect(host, path, nameOfFile);
		}
		else {
			throw std::exception("Invalid protocol\n");
		}
	}
	file = nameOfFile;
}

string MakeFullHtmlFile::getFile() const {
	return file;
}

void MakeFullHtmlFile::httpConnect(string &host, string &path, string &nameOfFile) {
	std::ofstream fout(nameOfFile);
	boost::asio::io_service io_service;
	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, "http");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	// Try each endpoint until we successfully establish a connection.
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	string req = "GET " + path + " HTTP/1.0\r\nHost: "
		+ host + "\r\nAccept: * /*\r\nConnection: close\r\n\r\n";
	write(socket, buffer(req.c_str(), req.length()));
	// Read until EOF, writing data to output as we go.
	char buff[512];
	boost::system::error_code ec;
	while (!ec) {
		int bytes = read(socket, buffer(buff), ec);
		fout << string(buff, bytes);
	}
	if (ec != error::eof)
		throw exception(ec.message().c_str());
}

void MakeFullHtmlFile::httpsConnect(string &host, string &path, string &nameOfFile) {
	std::ofstream fout(nameOfFile);
	typedef ssl::stream<ip::tcp::socket> ssl_socket;
	ssl::context ctx(ssl::context::sslv23);
	ctx.set_default_verify_paths();
	// Open an SSL socket to the given host
	io_service service;
	ssl_socket sock(service, ctx);
	ip::tcp::resolver resolver(service);
	ip::tcp::resolver::query query(host, "https");
	connect(sock.lowest_layer(), resolver.resolve(query));
	// The SSL handshake
	sock.set_verify_mode(ssl::verify_none);
	sock.set_verify_callback(ssl::rfc2818_verification(host));
	sock.handshake(ssl_socket::client);
	std::string req = "GET " + path + " HTTP/1.0\r\nHost: "
		+ host + "\r\nAccept: * /*\r\nConnection: close\r\n\r\n";
	write(sock, buffer(req.c_str(), req.length()));
	char buff[512];
	boost::system::error_code ec;
	while (!ec) {
		int bytes = read(sock, buffer(buff), ec);
		fout << string(buff, bytes);
	}
	if (ec != error::eof)
		throw exception(ec.message().c_str());
}

void MakeFullHtmlFile::parseUrl(const string& url, string &host, string &path) {
	if (url.size() == 0)
		throw exception("Invalid url\n");
	if (host.size())
		host.clear();
	if (path.size())
		path.clear();
	const int NUMBER_OF_PATH = (int)url.find('/');
	if (NUMBER_OF_PATH < 0) {
		host += url;
		path += '/';
	}
	else {
		string x(url);
		x[NUMBER_OF_PATH] = 0;
		host += x.c_str();
		path += &url[NUMBER_OF_PATH];
	}
}

void MakeFullHtmlFile::urlToNameOfFile(string& url) {
	if (url.size() == 0)
		throw exception("Invalid url\n");
	int findSymbol = (int)url.find('/');
	while (findSymbol >= 0) {
		url[findSymbol] = '+';
		findSymbol = (int)url.find('/');
	}
	findSymbol = (int)url.rfind('+');
	if ((findSymbol > 0) && ((int)url.rfind('.') > findSymbol)) {
		url.erase(url.rfind('.'));
	}
	url += "_FULL_HTML.txt";
}