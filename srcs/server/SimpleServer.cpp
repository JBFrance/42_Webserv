#include "serverheaders.hpp"


SimpleServer::SimpleServer(int domain, int service, int protocol, int port, std::string ipAddress, int bklg) {
	this->socket = new ListeningSocket(domain, service, protocol, port, ipAddress, bklg);
	
	bzero(buffer, 30000);
	//launch();
}

void SimpleServer::accepter() {
	struct sockaddr_in address = socket->get_address();
	int addrlen = sizeof(address);
	new_socket = accept(socket->get_sock(), (struct sockaddr *) &address, (socklen_t *)&addrlen);

	// Make non blocking
	fcntl(new_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC); // Close automatically (even though manual closing below)

	// std::cout << "Accepting..." << std::endl;
	read(new_socket, buffer, 30000);
};

// maybe delete this if unused
void SimpleServer::handler() {
	// std::cout << "here!!!!!!!!!" << buffer << std::endl;
};

SimpleServer::~SimpleServer() {
	std::cout << "Server destroyed!" << std::endl;
}

void SimpleServer::responder() {
	// Create a HTTP handler here
	std::string response_html = "Hello from the server!!!";
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	// response += "Content-Length: " + std::to_string(response_html.size()) + "\r\n";
	response += "\r\n";
	// response += "<!DOCTYPE html>\n<html><body>"; //CHECK WHY DOESNT WORK IF WRAPPED(seems to wrap automatically)
	response += response_html;
	// response += "</body></html>"; 
	response += "\r\n\n";
	write(new_socket, response.c_str(), strlen(response.c_str()));
	std::cout << response << std::endl;
	close(new_socket);
};

void SimpleServer::launch() {
	while (1) {
		std::cout << "Waiting..." << SOMAXCONN << std::endl;
		accepter();
		handler();
		responder();
		std::cout << "Done" << std::endl;
	}
}

ListeningSocket* SimpleServer::get_socket() {
	return this->socket;
}
