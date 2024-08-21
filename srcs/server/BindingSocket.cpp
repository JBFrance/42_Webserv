#include "serverheaders.hpp"


BindingSocket::BindingSocket(int domain, int service, int protocol, int port, std::string ipAddress) : SimpleSocket(domain, service, protocol, port, ipAddress) {
	// Establiish connection
	set_connection(connect_to_network(get_sock(), get_address()));
	test_connection(get_connection());
}


int BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
	return bind(sock, (struct sockaddr *) &address, sizeof(address));
}

