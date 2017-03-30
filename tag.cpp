// include project files
#include "tag.h"

void Tag::getTags(string& a, set<string>& tags) {
	if (tags.find(shortName) != tags.end()) {
		a += ('<' + fullName + '>' + getContent() + "</" + shortName + '>');
	}
	else {
		for (auto it = listOfChildren.begin(); it != listOfChildren.end(); it++)
			it->getTags(a, tags);
	}

}

list<Tag>& Tag::getListOfChildren() {
	return listOfChildren;
}

string Tag::getFullName() const {
	return fullName;
}

string Tag::getShortName() const {
	return shortName;
}

string Tag::getContent() const {
	return string(&htmlFile[beginOfContent], endOfContent - beginOfContent + 1);
}

int Tag::getBeginOfContent() const {
	return beginOfContent;
}

int Tag::getEndOfContent() const {
	return endOfContent;
}

Tag::Tag(string& file, int begOfCont, string fullN, string shortN, Tag * parentOfThis) :
	htmlFile(file), beginOfContent(begOfCont), fullName(fullN), shortName(shortN), parent(parentOfThis)
{
	if (shortName == "script") {
		const int POS_CLOSE_TAG = findNextCloseTag(begOfCont);

		if (POS_CLOSE_TAG >= 0) {
			endOfContent = POS_CLOSE_TAG - 1;
		}
		else
			throw std::exception("Incorrect html 1\n");
	}
	else {
		int pos = begOfCont;
		while (1) {
			const int POS_OPEN_TAG = findNextOpenTag(pos);
			const int POS_CLOSE_TAG = findNextCloseTag(pos);
			if ((POS_OPEN_TAG < POS_CLOSE_TAG) && (POS_OPEN_TAG >= 0)) {
				int posCloseNameOfNewTag = POS_OPEN_TAG, beginOfNewCont;
				while ((htmlFile[posCloseNameOfNewTag] != '>') && (htmlFile[posCloseNameOfNewTag] != '\0')) {
					posCloseNameOfNewTag++;
				}
				if (htmlFile[posCloseNameOfNewTag] == '\0')
					throw std::exception("Incorrect html 2\n");
				string fullNameOfNewTag(&htmlFile[POS_OPEN_TAG + 1], posCloseNameOfNewTag - POS_OPEN_TAG - 1);
				string shortNameOfNewTag;
				const int FIND_SYMBOL_SPACE = (int)fullNameOfNewTag.find(' ');
				if (FIND_SYMBOL_SPACE == -1)
					shortNameOfNewTag = fullNameOfNewTag;
				else {
					shortNameOfNewTag = string(fullNameOfNewTag.c_str(), FIND_SYMBOL_SPACE);
				}
				if (fullNameOfNewTag[fullNameOfNewTag.size() - 1] == '/') {
					pos = POS_OPEN_TAG;
					pos += fullNameOfNewTag.size() + 2;
					continue;
				}
				Tag a(htmlFile, posCloseNameOfNewTag + 1, fullNameOfNewTag, shortNameOfNewTag, this);
				listOfChildren.push_back(a);
				pos = a.getEndOfContent();
				pos += (a.getShortName().size() + 2);
				continue;
			};
			if ((POS_CLOSE_TAG >= 0) && ((POS_OPEN_TAG > POS_CLOSE_TAG) || (POS_OPEN_TAG < 0))) {
				endOfContent = POS_CLOSE_TAG - 1;
				break;
			}

			if (POS_CLOSE_TAG < 0) {
				std::cout << fullName << ' ' << shortName << '\n';
				throw std::exception("Incorrect html 3\n");
			}
		}
	}
};
int Tag::findNextOpenTag(int pos) {
	while (1) {
		while ((htmlFile[pos] != '\0') && (htmlFile[pos] != '<')) {
			pos++;
		}
		if (htmlFile[pos] == '\0')
			return -1;
		const char SECOND_SYMBOL(htmlFile[pos + 1]);
		if (((SECOND_SYMBOL >= 'A') && (SECOND_SYMBOL <= 'Z')) || ((SECOND_SYMBOL >= 'a') && (SECOND_SYMBOL <= 'z')))
			return pos;
		pos++;
	}
}

int Tag::findNextCloseTag(int pos) {
	while (1) {
		while ((htmlFile[pos] != '\0') && (htmlFile[pos] != '<')) {
			pos++;
		}
		if (htmlFile[pos] == '\0')
			return -1;
		const char SECOND_SYMBOL(htmlFile[pos + 1]);
		if (SECOND_SYMBOL == '/')
			return pos;
		pos++;
	}
}