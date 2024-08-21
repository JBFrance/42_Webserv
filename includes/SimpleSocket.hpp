#ifndef SimpleSocket_hpp
#define SimpleSocket_hpp


#include "serverheaders.hpp"



// namespace hde

class SimpleSocket {

	private:
		int sock;
		struct sockaddr_in address;
		int connection;

	public:
		// Constructor
		SimpleSocket(int domain, int service, int protocol, int port, std::string ipAddress);
		
		// Virtual function to connect to network
		virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
		
		// Function to test sockets and connections
		void test_connection(int item_to_test);

		// Getter functions
		struct sockaddr_in get_address();
		int get_sock();
		int get_connection();

		// Setter functions
		void set_connection(int con);
};

#endif