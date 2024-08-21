#include "serverheaders.hpp"

// Default constructor

SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, std::string ipAddress)
{
	// !!!!! GET RID OF u_long interface, take in IP address as string
	// std::string ipAddress = "127.0.0.1";
	// Define address structure
	address.sin_family = domain;
	address.sin_port = htons(port);

	// address.sin_addr.s_addr = htonl(interface); // remove line
	inet_pton(AF_INET, ipAddress.c_str(), &(address.sin_addr)); // put fail check

	// Establish socket
	this->sock = socket(domain, service, protocol);
	test_connection(sock);
};

// Test connection virtual function

void SimpleSocket::test_connection(int item_to_test) {

	// Confirms socket or connection has been properly established
	if (item_to_test < 0)
	{
		perror("Failed to connect");
		exit(EXIT_FAILURE);
	}
};

// Getter functions

sockaddr_in SimpleSocket::get_address() {
	return this->address;
}

int SimpleSocket::get_sock() {
	return this->sock;
}

int SimpleSocket::get_connection() {
	return this->connection;
}

// Setter functions
void SimpleSocket::set_connection(int con) {
	this->connection = con;
}