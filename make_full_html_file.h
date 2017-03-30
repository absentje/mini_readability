#ifndef _MAKE_FULL_HTML_FILE__
#define _MAKE_FULL_HTML_FILE__
#include <string>
using std::string;
using std::exception;

class MakeFullHtmlFile {
private:
	std::string file;
public:
	MakeFullHtmlFile();
	string getFile() const;
private:
	void httpConnect(string &host, string &path, string &nameOfFile);
	void httpsConnect(string &host, string &path, string &nameOfFile);
	void parseUrl(const string& url, string &host, string &path);
	void urlToNameOfFile(string& url);
};

#endif //_MAKE_FULL_HTML_FILE__