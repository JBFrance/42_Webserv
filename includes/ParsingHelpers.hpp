#ifndef __PARSING_HELPERS_HPP__
# define __PARSING_HELPERS_HPP__

    char	**ft_split(char const *s, char c);
    void    initialiseHeaders(std::map<std::string, std::string>& headers);
    void    getMethod(std::map<std::string, std::string>& headers, char** splitBuffer, int i);
    void    getPath(std::map<std::string, std::string>& headers, char** splitBuffer, int i);
    void    getPort(std::map<std::string, std::string>& headers, char** splitBuffer, int i);
    void    getFileToDelete(std::string& fileToDeleteName, char** splitBuffer, int i);
    void    getContentDisposition(std::map<std::string, std::string>& headers, char** splitBuffer, int i);
   
    char	*ft_itoa(int n);
    void clearBuffer(char** splitBuffer, int i);

#endif