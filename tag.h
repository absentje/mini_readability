#ifndef _TAG__
#define _TAG__
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>

using std::string;
using std::set;
using std::list;

class Tag {
private:
	string& htmlFile;
	string fullName, shortName;
	int beginOfContent, endOfContent; // position of begin and pos. of end in htmlFile
	list<Tag> listOfChildren;
	Tag *parent;
public:
	void getTags(string& a, set<string>& tags);
	list<Tag>& getListOfChildren();
	string getFullName() const;
	string getShortName() const;
	string getContent() const;
	int getBeginOfContent() const;
	int getEndOfContent() const;
	Tag(string& file, int begOfCont, string fullN, string shortN, Tag * parentOfThis);
	int findNextOpenTag(int pos);
	int findNextCloseTag(int pos);
};
#endif //_TAG__