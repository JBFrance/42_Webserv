// #include "../../includes/Server.hpp"

// Server::Server()
// {
// }

// Server::Server(t_str &body)
// {
// 	t_str	tmp;

// 	tmp = body.substr(1, body.length() - 2);
// 	if (tmp == "" || tmp.find_first_not_of(" \t\v\r\n\f") == t_str::npos)
// 		throw std::logic_error("Error: Config file: directive 'server' is empty.");
// 	_directiveList.push_back("server_name");
// 	_directiveList.push_back("listen");
// 	_directiveList.push_back("root");
// 	_directiveList.push_back("index");
// 	_directiveList.push_back("autoindex");
// 	_directiveList.push_back("error_page");
// 	_directiveList.push_back("client_max_body_size");
// 	_directiveList.push_back("allow_methods");
// 	_directiveList.push_back("location");
// 	_directiveList.push_back("cgi");
// 	_directiveList.push_back("upload_dir");
// 	_directiveList.push_back("return");
// 	this->_autoindex = false;
// 	this->_clientMaxBodySize = 0;
// 	parsingBody(tmp);

// 	// added status code assignment and created a listening server
// 	statusCodes["200"] = "OK";
// 	statusCodes["201"] = "Created";
// 	statusCodes["202"] = "Accepted";
// 	statusCodes["204"] = "No Content";
// 	statusCodes["301"] = "Moved Permanently";
// 	statusCodes["302"] = "Found";
// 	statusCodes["304"] = "Not Modified";
// 	statusCodes["400"] = "Bad Request";
// 	statusCodes["401"] = "Unauthorized";
// 	statusCodes["403"] = "Forbidden";
// 	statusCodes["404"] = "Not Found";
// 	statusCodes["405"] = "Method Not Allowed";
// 	statusCodes["408"] = "Request Timeout";
// 	statusCodes["412"] = "Precondition Failed";
// 	statusCodes["500"] = "Internal Server Error";
// 	statusCodes["501"] = "Not Implemented";
// 	statusCodes["502"] = "Bad Gateway";
// 	statusCodes["503"] = "Service Unavailable";
// 	// this->serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, this->port, this->ipAddress , 5);
        

// }

// Server::Server(const Server &other)
// {
// 	this->_directiveList = other._directiveList;
// 	this->_serverName = other._serverName;
// 	this->_listen = other._listen;
// 	this->_location = other._location;
// 	this->_root = other._root;
// 	this->_index = other._index;
// 	this->_autoindex = other._autoindex;
// 	this->_errorPage = other._errorPage;
// 	this->_location = other._location;
// 	this->_clientMaxBodySize = other._clientMaxBodySize;
// 	this->_allowedMethods = other._allowedMethods;
// 	this->_cgi = other._cgi;
// 	this->_uploadDir = other._uploadDir;
// 	this->_return = other._return;
// }

// Server	&Server::operator=(const Server &rhs)
// {
// 	if (this != &rhs)
// 	{
// 		this->_directiveList = rhs._directiveList;
// 		this->_serverName = rhs._serverName;
// 		this->_listen = rhs._listen;
// 		this->_location = rhs._location;
// 		this->_root = rhs._root;
// 		this->_index = rhs._index;
// 		this->_autoindex = rhs._autoindex;
// 		this->_errorPage = rhs._errorPage;
// 		this->_clientMaxBodySize = rhs._clientMaxBodySize;
// 		this->_allowedMethods = rhs._allowedMethods;
// 		this->_cgi = rhs._cgi;
// 		this->_uploadDir = rhs._uploadDir;
// 		this->_return = rhs._return;
// 	}
// 	return (*this);
// }

// Server::~Server()
// {
// }

// void	Server::parsingListen(t_str &value)
// {
// 	t_str::size_type	colon_pos;
// 	t_str::size_type	addr_end;
// 	t_str				addr;
// 	t_str::size_type	port_begin;
// 	t_str::size_type	port_end;
// 	t_str				port;

// 	colon_pos = value.find(':');
// 	if (colon_pos != t_str::npos)
// 	{
// 		if (colon_pos == 0)
// 		{
// 			addr = "*";
// 			port_begin = value.find_first_not_of(" \t\v\r\n\f", 1);
// 			port_end = value.find_last_not_of(" \t\v\r\n\f") + 1;
// 		}
// 		else
// 		{
// 			addr_end = value.find_last_not_of(" :\t\v\r\n\f", colon_pos) + 1;
// 			addr = value.substr(0, addr_end);
// 			port_begin = value.find_first_not_of(" \t\v\r\n\f", colon_pos + 1);
// 			port_end = value.find_last_not_of(" \t\v\r\n\f") + 1;
// 		}
// 		port = value.substr(port_begin, port_end - port_begin);
// 		port_begin = port.find_first_not_of('0');
// 		if (port_begin == t_str::npos)
// 			throw std::runtime_error("Error: Port is not valid.");
// 		port = port.substr(port_begin, port.length() - port_begin);
// 	}
// 	else
// 	{
// 		value = value.substr(0, value.find_last_not_of(" \t\v\r\n\f") + 1);
// 		port = value;
// 		port_begin = port.find_first_not_of('0');
// 		if (port_begin == t_str::npos)
// 			throw std::runtime_error("Error: Port is not valid.");
// 		port = port.substr(port_begin, port.length() - port_begin);
// 		addr = "*";
// 	}
// 	this->setListen(addr, port);

// 	// added this
// 	// this->port = std::atoi(port.c_str());
// 	// this->ipAddress = "127.0.0.1";
// 	// std::cout << "HERE doesnt register " << this->ipAddress << std::endl;
// }


// void	Server::parsingLocation(t_str &body, t_str::size_type &value_begin, \
// 							 t_str::size_type &value_end)
// {
// 	t_str::size_type	count = 1;

// 	value_end = body.find_first_of("{}", value_begin);
// 	if (value_end == t_str::npos || body[value_end] == '}')
// 		throw std::runtime_error("Error: Config file: directive " \
// 								 "'location' has no opening '{'.");
// 	value_end = body.find_first_of("{}", value_end + 1);
// 	while (value_end != t_str::npos && count != 0)
// 	{
// 		if (body[value_end] == '{')
// 			++count;
// 		else
// 			--count;
// 		if (count != 0)
// 			value_end = body.find_first_of("{}", value_end + 1);
// 	}
// 	if (value_end == t_str::npos)
// 		throw std::runtime_error("Error: Config file: directive " \
// 								 "'location' has no closing '}'.");
// 	++value_end;
// }

// void	Server::parsingBody(t_str &body)
// {
// 	t_str::size_type	name_begin;
// 	t_str::size_type	name_end;
// 	t_str				name;
// 	t_str::size_type	value_begin;
// 	t_str::size_type	value_end;
// 	t_str				value;

// 	while (body != "")
// 	{
// 		name_begin = body.find_first_not_of(" \t\v\r\n\f");
// 		if (name_begin == t_str::npos)
// 			break ;
// 		name_end = body.find_first_of(" \t\v\r\n\f", name_begin);
// 		if (name_end == t_str::npos)
// 			throw std::logic_error("Error: Config file: after directive name " \
// 								   "must be at least one whitespce and " \
// 								   "after that must be its value.");
// 		name = body.substr(name_begin, name_end - name_begin);
// 		// if (std::find(_directiveList.begin(), _directiveList.end(), name) == \
// 		// 											_directiveList.end())
// 		// 	throw std::logic_error("Error: Config file: directive name '" \
// 		// 		   					+ name + "' unknown.");
// 		value_begin = body.find_first_not_of(" \t\v\r\n\f", name_end);
// 		if (value_begin == t_str::npos)
// 			throw std::logic_error("Error: Config file: directive '" + name + \
// 									"' doesn't have a value.");
// 		if (body.compare(name_begin, 8, "location") != 0)
// 		{
// 			value_end = body.find_first_of(';', value_begin);
// 			if (value_end == t_str::npos)
// 				throw std::logic_error("Error: Config file: value of "
// 					"directive '" + name + "' must end with ';' symbol.");
// 		}
// 		else
// 			parsingLocation(body, value_begin, value_end);
// 		value = body.substr(value_begin, value_end - value_begin);
// 		this->setFildes(name, value);
// 		if (body.compare(name_begin, 8, "location") != 0)
// 			body.erase(0, value_end + 1);
// 		else
// 			body.erase(0, value_end);
// 	}
// 	this->setDefaults();
// 	std::map<t_str, Location>::iterator	it;
// 	for (it = this->_location.begin(); it != this->_location.end(); ++it)
// 		it->second.inherit(this->_root, this->_index, this->_autoindex, this->_clientMaxBodySize, this->_allowedMethods, this->_cgi, this->_uploadDir, it->first);
// }