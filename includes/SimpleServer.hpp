#ifndef SimpleServer_hpp
#define SimpleServer_hpp

#include "serverheaders.hpp"

class ListeningSocket;

class SimpleServer {
	protected:
		char buffer[30000];
		int	new_socket;

	public:
		void accepter();
		void handler();
		void responder();
		ListeningSocket* socket;
		SimpleServer(int domain, int service, int protocol, int port, std::string ipAddress, int bklg);
		~SimpleServer();
		void launch();
		ListeningSocket* get_socket();
};

#endif