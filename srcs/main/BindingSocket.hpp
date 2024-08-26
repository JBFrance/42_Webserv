#ifndef BindingSocket_hpp
#define BindingSocket_hpp

#include "serverheaders.hpp"



class BindingSocket:public SimpleSocket {

	public:
		BindingSocket(int domain, int service, int protocol, int port, std::string ipAddress);
		int connect_to_network(int sock, struct sockaddr_in address);

};


#endif