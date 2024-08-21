// #include "../../includes/Location.hpp"

// /*=====================================*/
// /*     Constructors and Destructor     */
// /*=====================================*/

// Location::Location()
// {
// }

// Location::Location(t_str &body)
// {
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
// 	this->_isAutoindexed = false;
// 	this->_clientMaxBodySize = 0;
// 	parsingBody(body);
// }

// Location::Location(const Location &other)
// {
// 	this->_root = other._root;
// 	this->_index = other._index;
// 	this->_autoindex = other._autoindex;
// 	this->_isAutoindexed = other._isAutoindexed;
// 	this->_errorPage = other._errorPage;
// 	this->_clientMaxBodySize = other._clientMaxBodySize;
// 	this->_location = other._location;
// 	this->_allowedMethods = other._allowedMethods;
// 	this->_cgi = other._cgi;
// 	this->_uploadDir = other._uploadDir;
// 	this->_return = other._return;
// }

// Location	&Location::operator=(const Location &rhs)
// {
// 	if (this != &rhs)
// 	{
// 		this->_root = rhs._root;
// 		this->_index = rhs._index;
// 		this->_autoindex = rhs._autoindex;
// 		this->_isAutoindexed = rhs._isAutoindexed;
// 		this->_errorPage = rhs._errorPage;
// 		this->_clientMaxBodySize = rhs._clientMaxBodySize;
// 		this->_location = rhs._location;
// 		this->_allowedMethods = rhs._allowedMethods;
// 		this->_cgi = rhs._cgi;
// 		this->_uploadDir = rhs._uploadDir;
// 		this->_return = rhs._return;
// 	}
// 	return (*this);
// }

// Location::~Location()
// {
// }

// const std::string	&Location::getRoot() const
// {
// 	return (this->_root);
// }

// const std::map<std::string, Location>	&Location::getLocation() const
// {
// 	return (this->_location);
// }

// const std::vector<std::string>	&Location::getIndex() const
// {
// 	return (this->_index);
// }

// bool	Location::getAutoindex() const
// {
// 	return (this->_autoindex);
// }

// const std::map<int, std::string>	&Location::getErrorPage() const
// {
// 	return (this->_errorPage);
// }

// unsigned long int	Location::getClientMaxBodySize() const
// {
// 	return (this->_clientMaxBodySize);
// }

// const std::vector<std::string>	&Location::getAllowedMethods() const
// {
// 	return (this->_allowedMethods);
// }

// const std::map<std::string, std::string>	&Location::getCgi() const
// {
// 	return (this->_cgi);
// }

// const std::string	&Location::getUploadDir() const
// {
// 	return (this->_uploadDir);
// }

// const std::string	&Location::getReturn() const
// {
// 	return (this->_return);
// }

// void	Location::inherit(t_str root, std::vector<t_str> index, bool autoindex, unsigned long int clientMaxBodySize, std::vector<t_str> allowMethods, std::map<t_str, t_str> cgi, std::string uploadDir, t_str path)
// {
// 	std::map<t_str, Location>::iterator	it;
// 	std::map<t_str, t_str>::iterator	it_cgi;
// 	std::string							full_path;
// 	std::string							error_msg;

// 	if (this->_root == "")
// 		this->_root = root;
// 	if (this->_index.size() == 0)
// 		this->_index = index;
// 	if (this->_isAutoindexed == false)
// 		this->_autoindex = autoindex;
// 	if (this->_clientMaxBodySize == 0)
// 		this->_clientMaxBodySize = clientMaxBodySize;
// 	if (this->_allowedMethods.size() == 0)
// 		this->_allowedMethods = allowMethods;
// 	if (this->_uploadDir == "")
// 		this->_uploadDir = uploadDir;
// 	for (it_cgi = cgi.begin(); it_cgi != cgi.end(); ++it_cgi)
// 	{
// 		if (this->_cgi.find(it_cgi->first) != this->_cgi.end())
// 			this->_cgi[it_cgi->first] = it_cgi->second;
// 	}
// 	if (path[path.length() - 1] != '/')
// 		full_path = path + "/";
// 	else
// 		full_path = path;
// 	for (it = this->_location.begin(); it != this->_location.end(); ++it)
// 	{
// 		if (it->first.compare(0, full_path.length(), full_path) != 0)
// 		{
// 			error_msg = "Error: '" + it->first + "' is not in '" \
// 						 + full_path + "' location (i.e. it must start with '" \
// 						 + full_path + "' path).";
// 			throw std::runtime_error(error_msg);
// 		}
// 		if (it->first.length() <= full_path.length())
// 			throw std::runtime_error("Error: location paths are indentical.");
// 		it->second.inherit(this->_root, this->_index, this->_autoindex, this->_clientMaxBodySize, this->_allowedMethods, this->_cgi, this->_uploadDir, it->first);
// 	}
// }

// void	Location::setRoot(t_str &value)
// {
// 	std::string::size_type	pos;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: root value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value.find_first_of(" \t\v\r\n\f") != std::string::npos)
// 		throw std::runtime_error("Error: root isn't valid "
// 								 "(there are whitespaces).");
// 	if (value.compare(0, 4, "www/") != 0)
// 		throw std::runtime_error("Error: root must start with 'www/' path.");
// 	// if (access(value.c_str(), F_OK) != 0)
// 	// 	throw std::runtime_error("Error: root directory not found.");
// 	this->_root = value;
// }

// void	Location::setLocation(t_str &value)
// {
// 	std::string::size_type	pos;
// 	std::string::size_type	pos2;
// 	std::string				name;
// 	std::string				inner;

// 	pos = value.find_first_of("{");
// 	name = value.substr(0, pos);
// 	pos2 = name.find_last_not_of(" \t\v\r\n\f");
// 	if (pos2 == std::string::npos)
// 		throw std::runtime_error("Error: location is not valid.");
// 	++pos2;
// 	name = name.substr(0, pos2);
// 	++pos;
// 	inner = value.substr(pos, value.length() - 1 - pos);
// 	if (name[0] != '/')
// 		throw std::runtime_error("Error: Location must start with '/' symbol.");
// 	if (name[name.length() - 1] != '/')
// 		name += "/";
// 	this->_location.insert(std::make_pair(name, Location(inner)));
// }

// void	Location::setIndex(t_str &value)
// {
// 	char	*token;

// 	token = std::strtok(&value[0], " \t\v\r\n\f");
// 	if (token == NULL)
// 		throw std::runtime_error("Error: Config file: Directive value of index is empty.");
// 	while (token != NULL)
// 	{
// 		this->_index.push_back(token);
// 		token = std::strtok(NULL, " \t\v\r\n\f");
// 	}
// }

// void	Location::setAutoindex(t_str &value)
// {
// 	std::string::size_type	pos;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: autoindex value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value == "on")
// 		this->_autoindex = true;
// 	else if (value == "off")
// 		this->_autoindex = false;
// 	else
// 		throw std::runtime_error("Error: autoindex value can only be 'on' or 'off'.");
// 	this->_isAutoindexed = true;
// }

// void	Location::setErrorPage(t_str &value)
// {
// 	t_str::size_type	pos;
// 	char				*token;
// 	t_str				page;
// 	int					error_number;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: Config file: Directive value of error_page is empty.");
// 	value = value.substr(0, ++pos);
// 	pos = value.find_last_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: Config file: Directive value of error_page must contain an error number and after that an error page uri.");
// 	++pos;
// 	page = value.substr(pos, value.length() - pos);
// 	value = value.substr(0, pos - 1);
// 	token = std::strtok(&value[0], " \t\v\r\n\f");
// 	while (token != NULL)
// 	{
// 		error_number = std::atoi(token);
// 		if (!(error_number >= 300 && error_number <= 599))
// 			throw std::runtime_error("Error: Config file: Directive value of error_page must be between '300' and '599' range.");
// 		this->_errorPage.insert(std::make_pair(error_number, page));
// 		token = std::strtok(NULL, " \t\v\r\n\f");
// 	}
// }

// void	Location::setClientMaxBodySize(t_str &value)
// {
// 	std::string::size_type	pos;
// 	int						coefficient;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: client_max_body_size value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value.back() == 'k' || value.back() == 'K')
// 	{
// 		coefficient = 1024;
// 		value.pop_back();
// 	}
// 	else if (value.back() == 'm' || value.back() == 'M')
// 	{
// 		coefficient = 1024 * 1024;
// 		value.pop_back();
// 	}
// 	else if (value.back() >= '0' && value.back() <= '9')
// 		coefficient = 1;
// 	else
// 		throw std::runtime_error("Error: client_max_body_size is not valid.");
// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: client_max_body_size value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value == "")
// 		throw std::runtime_error("Error: client_max_body_size is not valid.");
// 	for (std::string::iterator it = value.begin(); it != value.end(); ++it)
// 	{
// 		if (!(*it >= '0' && *it <= '9'))
// 			throw std::runtime_error("Error: client_max_body_size is not valid.");
// 	}
// 	this->_clientMaxBodySize = coefficient * std::atol(value.c_str());
// }

// void	Location::setAllowedMethods(t_str &value)
// {
// 	char	*token;

// 	token = std::strtok(&value[0], " \t\v\r\n\f");
// 	if (token == NULL)
// 		throw std::runtime_error("Error: Config file: Directive value of allow_methods is empty.");
// 	while (token != NULL)
// 	{
// 		if (std::strncmp(token, "GET", std::strlen(token)) != 0 && \
// 			std::strncmp(token, "POST", std::strlen(token)) != 0 && \
// 			std::strncmp(token, "DELETE", std::strlen(token)) != 0)
// 			throw std::runtime_error("Error: The values of allow_methods must be one of these 'GET' 'POST' 'DELETE'.");
// 		this->_allowedMethods.push_back(token);
// 		token = std::strtok(NULL, " \t\v\r\n\f");
// 	}
// }

// void	Location::setCgi(t_str &value)
// {
// 	t_str::size_type	pos;
// 	t_str				uri;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: Config file: Directive "
// 								 "value of cgi is empty.");
// 	value = value.substr(0, ++pos);
// 	pos = value.find_last_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: Config file: Directive value of "
// 								 "cgi must contain a cgi extension"
// 								 "and after that the cgi uri.");
// 	++pos;
// 	uri = value.substr(pos, value.length() - pos);
// 	value = value.substr(0, pos - 1);
// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	value = value.substr(0, ++pos);
// 	if (value.find(" \t\v\r\n\f") != std::string::npos)
// 		throw std::runtime_error("Error: Config file: Directive value of cgi is not valid.");
// 	// if (access(uri.c_str(), F_OK) != 0)
// 	// 	throw std::runtime_error("Error: cgi file not found.");
// 	this->_cgi.insert(std::make_pair(value, uri));
// }

// void	Location::setUploadDir(t_str &value)
// {
// 	std::string::size_type	pos;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: upload_dir value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value.find_first_of(" \t\v\r\n\f") != std::string::npos)
// 		throw std::runtime_error("Error: upload_dir isn't valid (there are whitespaces).");
// 	if (value.compare(0, 4, "www/") != 0)
// 		throw std::runtime_error("Error: upload_dir must start with 'www/' path.");
// 	if (access(value.c_str(), F_OK) != 0)
// 		throw std::runtime_error("Error: upload_dir value does not exist.");
// 	this->_uploadDir = value;
// }

// void	Location::setReturn(t_str &value)
// {
// 	std::string::size_type	pos;

// 	pos = value.find_last_not_of(" \t\v\r\n\f");
// 	if (pos == std::string::npos)
// 		throw std::runtime_error("Error: return directive value is empty.");
// 	++pos;
// 	value = value.substr(0, pos);
// 	if (value.find_first_of(" \t\v\r\n\f") != std::string::npos)
// 		throw std::runtime_error("Error: return directive isn't valid (there are whitespaces).");
// 	if (value[0] != '/' && value.compare(0, 7, "http://") != 0 && \
// 			value.compare(0, 8, "https://") != 0)
// 		throw std::runtime_error("Error: return directive must start with one of this 'http://', 'https://', '/'.");
// 	this->_return = value;
// }

// void	Location::setFildes(const t_str &name, t_str &value)
// {
// 	if (name.compare("root") == 0)
// 		this->setRoot(value);
// 	else if (name.compare("location") == 0)
// 		this->setLocation(value);
// 	else if (name.compare("index") == 0)
// 		this->setIndex(value);
// 	else if (name.compare("autoindex") == 0)
// 		this->setAutoindex(value);
// 	else if (name.compare("error_page") == 0)
// 		this->setErrorPage(value);
// 	else if (name.compare("client_max_body_size") == 0)
// 		this->setClientMaxBodySize(value);
// 	else if (name.compare("allow_methods") == 0)
// 		this->setAllowedMethods(value);
// 	else if (name.compare("cgi") == 0)
// 		this->setCgi(value);
// 	else if (name.compare("upload_dir") == 0)
// 		this->setUploadDir(value);
// 	else if (name.compare("return") == 0)
// 		this->setReturn(value);
// }

// void	Location::parsingLocation(t_str &body, t_str::size_type &value_begin, t_str::size_type &value_end)
// {
// 	t_str::size_type	count = 1;

// 	value_end = body.find_first_of("{}", value_begin);
// 	if (value_end == t_str::npos || body[value_end] == '}')
// 		throw std::runtime_error("Error: Config file: directive 'location' has no opening '{'.");
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
// 		throw std::runtime_error("Error: Config file: directive 'location' has no closing '}'.");
// 	++value_end;
// }

// void	Location::parsingBody(t_str &body)
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
// 			throw std::logic_error("Error: Config file: after directive name must be at least one whitespce and after that must be its value.");
// 		name = body.substr(name_begin, name_end - name_begin);
// 		// if (std::find(_directiveList.begin(), _directiveList.end(), name) == _directiveList.end())
// 		// 	throw std::logic_error("Error: Config file: directive name '" + name + "' unknown.");
// 		value_begin = body.find_first_not_of(" \t\v\r\n\f", name_end);
// 		if (value_begin == t_str::npos)
// 			throw std::logic_error("Error: Config file: directive '" + name + "' doesn't have a value.");
// 		if (body.compare(name_begin, 8, "location") != 0)
// 		{
// 			value_end = body.find_first_of(';', value_begin);
// 			if (value_end == t_str::npos)
// 				throw std::logic_error("Error: Config file: value of directive '" + name + "' must end with ';' symbol.");
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
// }