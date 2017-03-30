#ifndef _HTML_PARSER__
#define _HTML_PARSER__
#include "tag.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

class HtmlParser {
private:
	shared_ptr<Tag> file;
	string fullHtmlFile;
	string fileName;
public:
	HtmlParser(string nameOfFile);
	string getNameOfFile() const;
	string getTags(set<string>& tags);
	string getTagsFromFile(string nameOfFile);
	string getClearText();
private:
	void removeTagP(string&);
	void convertLink(string&);
	void makeReadyLine(string& fullLine, string& readyLine);
	void makeLineBreaks(string&);
	void deleteCharacterEntity(string&);
	void deleteRaquo(string&);
	void deleteLaquo(string&);
	void deleteMdash(string&);
	void deleteNdash(string&);
};
#endif  //_HTML_PARSER__