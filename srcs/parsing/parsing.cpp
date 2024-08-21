// #include "../../includes/webserv.hpp"

// std::vector<Server>	*parsing(std::string &config)
// {
// 	std::vector<Server>		*server_set;
// 	std::string::size_type	found_server;
// 	std::string				body;

// 	server_set = new std::vector<Server>;
// 	try
// 	{
// 		delete_comments(config);
// 		while (config != "")
// 		{
// 			found_server = config.find_first_not_of(" \t\v\r\n\f");
// 			if (found_server == std::string::npos)
// 				break ;
// 			else if (config.compare(found_server, 6, "server") == 0)
// 			{
// 				body = get_inside(config, found_server + 6);
// 				(*server_set).push_back(Server(body)); //Check if constructors works for stucts, else use new
// 				// server_set<this>->serverptr = new simpleserver
				
				
// 			}
// 			else
// 				throw std::runtime_error("Error: Config file must contain only 'server' directive.");
// 		}
// 	}
// 	catch (...)
// 	{
// 		delete server_set;
// 		throw ;
// 	}


// 	//  test if being set properly...delete this afterwards
	
// 	std::vector<Server>::iterator it;
//     for (it = server_set->begin(); it != server_set->end(); ++it)
//     {
// 		it->ipAddress = "127.0.0.1";
// 		it->port = 8000;

//     }
// 	// 
	
// 	return (server_set);
// }


