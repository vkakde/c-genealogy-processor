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
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famsId == it_family.id) {
				///\remark Check date entry exists (individual may be un-married)
				if(it_individual.birthDay.length()!=0 && it_family.marrDate.length()!=0){
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) >= boost::gregorian::from_uk_string(it_family.marrDate)) {
						std::cout << "\nUS02 Fail (Birth before marriage) for Individual with ID : " << it_individual.id << "!\n";
						result = false;
					}
				}
			}
		}
	}
	return result;
}

///\author vkakde
bool Gedcom::gedcom::US08() {
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famcId == it_family.id) {
				///\remark Check date entry exists (parents' marriage date may be un-available)
				if (it_individual.birthDay.length() != 0 && it_family.marrDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) <= boost::gregorian::from_uk_string(it_family.marrDate)) {
						result = false;
						std::cout << "\nUS08 Fail (Birth before marriage of parents) for Individual with ID : " << it_individual.id << "!\n";
					}
				}

				///\remark Check date entry exists (parents' divorce date may be un-available)
				if (it_individual.birthDay.length() != 0 && it_family.divorceDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) - boost::gregorian::from_uk_string(it_family.divorceDate) >= boost::gregorian::days(9*30)) {
						result = false;
						std::cout << "\nUS08 Fail (Birth 9 months beyond parents' divorce) for Individual with ID : " << it_individual.id << "!\n";
					}
				}
			}
		}
	}
	return result;
}