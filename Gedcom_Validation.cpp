#include "Gedcom.h"

bool Gedcom::gedcom::isTagValid(std::string tagLevel, std::string tagName, int tagIndex) {
	// convert tag under test to lower case
	///cite https://stackoverflow.com/a/19266247/2535979
	std::locale loc;
	std::string testTagName_LowerCase;
	for (auto i : tagName) {
		testTagName_LowerCase += std::tolower(i);
	}

	// convert all supported tags to lower case (for comparison) 
	std::map<std::string, std::string> supportedTagsAndLevels_LowerCase;
	for (auto tag : supportedTagsAndLevels) {
		std::string loweredTag = "";
		std::string level = std::to_string(tag.second);
		for (auto i : tag.first) {
			loweredTag += std::tolower(i);
		}
		supportedTagsAndLevels_LowerCase.insert(std::make_pair(loweredTag, level));
	}

	// if tag under test matches any supported tag, tag's level and index is correct - it is valid. Return true.
	for (auto i : supportedTagsAndLevels_LowerCase) {
		if (i.first == testTagName_LowerCase) {
			// Okay, tag is supported. But is the level valid?
			if (i.second == tagLevel) {
				// Okay, tag is supported and level is valid. But is tag index okay?
				///remark In line "0 INDI id", tag name "INDI" and level "0" are valid. 
				///       But tag placement at index 1 is invalid, it should be "0 id INDI"
				if (testTagName_LowerCase == "fam" || testTagName_LowerCase == "indi") {
					if (tagIndex == 2) {
						return true;
					}
				}
				else {
					if (tagIndex == 1) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

///\author vkakde
bool Gedcom::gedcom::US02() {
	//boost::gregorian::date birthDate = { 2001, 10, 9 };
	//boost::gregorian::date marrDate = { 2020, 10, 9 };
	//std::cout << gedcomInstance.marriageAfter14(birthDate, marrDate) << "\n";
	return true;
}

///\author vkakde
bool Gedcom::gedcom::US08() {
	return true;
}