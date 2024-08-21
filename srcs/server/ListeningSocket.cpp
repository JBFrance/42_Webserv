#include "serverheaders.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, std::string ipAddress, int bklog) :
	BindingSocket(domain, service, protocol, port, ipAddress) {
		this->backlog = bklog;
		start_listening();
		test_connection(listening);
		
};

void ListeningSocket::start_listening() {
	this->listening = listen(get_sock(), backlog);

}