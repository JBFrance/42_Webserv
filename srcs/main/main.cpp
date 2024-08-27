#include "Server.hpp"
#include "Location.hpp"

int main() 
{
    std::vector<Server> servers;
    parseConfigFile("default.conf", servers);

    if (servers.empty()) 
        std::cout << "No servers found or error parsing config file." << std::endl;
	// else 
	// {
    //     for (const auto& server : servers) 
	// 		printServerDetails(server);
    // }
    return 0;
}
// int	main(int argc, char **argv)
// {
// 	std::string	config;

// 	try
// 	{
// 		if (argc == 1)
// 			file_open_read(CONFIG_DEFAULT, config);
// 		else if (argc == 2)
// 			file_open_read(argv[1], config);
// 		else
// 			throw std::runtime_error("Error: Argument count must be 1 or 0.");
// 		start(config);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() <<  std::endl;
// 	}
// 	return (0);
// }
