#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
// include project files
#include "make_full_html_file.h"
#include "html_parser.h"

int main(int argc, char* argv[])
{
	try {
		setlocale(LC_CTYPE, "Russian");
		MakeFullHtmlFile a;
		set<string> tags;
		tags.insert(string("title"));
		tags.insert(string("p"));
		HtmlParser parser(a.getFile());
		std::ofstream out1("fileFromTags.txt");
		std::ofstream out2(parser.getNameOfFile());
		std::ifstream tags_txt("tags.txt");
		if (tags_txt.is_open())
			out1 << parser.getTagsFromFile("tags.txt");
		out2 << parser.getClearText();
		out1.close();
		out2.close();
	}
	catch (std::exception& e){
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}
