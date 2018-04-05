#include "stdafx.h"
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

// Author: mjosephs
bool Gedcom::gedcom::US01() {
	bool result = true;
	boost::gregorian::date today = boost::gregorian::day_clock::local_day();

	//Go through individual list first, then family list because we need to check all dates
	for (auto it_individual : individualList) {
		//Check date entry exists
		if (it_individual.birthDay.length() != 0) {
			if (boost::gregorian::from_uk_string(it_individual.birthDay) >= today) {
				std::cout << "US01 Fail (Date before Current Date) for Individual with ID : " << it_individual.id << std::endl;
				result = false;
			}
		}
		if (it_individual.deathDate.length() != 0) {
			if (boost::gregorian::from_uk_string(it_individual.deathDate) >= today) {
				std::cout << "US01 Fail (Date before Current Date) for Individual with ID : " << it_individual.id << std::endl;
				result = false;
			}
		}
	}
	for (auto fam : familyList) {
		//Check date entry exists
		if (fam.divorceDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.divorceDate) >= today) {
				std::cout << "US01 Fail (Date before Current Date) for Family with ID : " << fam.id << std::endl;
				result = false;
			}
		}
		if (fam.marriageDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.marriageDate) >= today) {
				std::cout << "US01 Fail (Date before Current Date) for Family with ID : " << fam.id << std::endl;
				result = false;
			}
		}
	}
	return result;
}

///\author vkakde
bool Gedcom::gedcom::US02() {
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famsId == it_family.id) {
				///\remark Check date entry exists (individual may be un-married)
				if(it_individual.birthDay.length()!=0 && it_family.marriageDate.length()!=0){
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) >= boost::gregorian::from_uk_string(it_family.marriageDate)) {
						std::cout << "US02 Fail (Birth before marriage) for Individual with ID : " << it_individual.id << std::endl;
						result = false;
					}
				}
			}
		}
	}
	return result;
}

///\author gmccorma
bool Gedcom::gedcom::US03() {
	bool result = true;
	for (auto it_individual : individualList) {
		// Tests to see if the birth date is after the death date.  Returns false if that is the case, true otherwise.
		if (it_individual.deathDate != "") {
			//std::cout << "Birth: " << boost::gregorian::from_uk_string(it_individual.birthDay).year() << std::endl;
			//std::cout << "Death: " << boost::gregorian::from_uk_string(it_individual.deathDate).year() << std::endl;
			// if birth and death year same, and birth and death month same, and birth day greater than death day
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() == 0) {
				if (boost::gregorian::from_uk_string(it_individual.deathDate).month() - boost::gregorian::from_uk_string(it_individual.birthDay).month() == 0) {
					if (boost::gregorian::from_uk_string(it_individual.deathDate).day() - boost::gregorian::from_uk_string(it_individual.birthDay).day() < 0) {
						std::cout << "US03 Fail (Birth before death) for Individual with ID : " << it_individual.id << std::endl;
						result = false;
					}
				}
			}
			// if birth and death year same, and birth month greater than death month
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() == 0) {
				if (boost::gregorian::from_uk_string(it_individual.deathDate).month() - boost::gregorian::from_uk_string(it_individual.birthDay).month() < 0) {
					std::cout << "US03 Fail (Birth before death) for Individual with ID : " << it_individual.id << std::endl;
					result = false;
				}
			}
			// if birth year greater than death year
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() < 0) {
				std::cout << "US03 Fail (Birth before death) for Individual with ID : " << it_individual.id << std::endl;
				result = false;
			}
			else {
				result = true;
			}
		}
		}
	return result;
}

// Author: mjosephs
bool Gedcom::gedcom::US04() {
	bool result = true;
	for (auto fam : familyList) {
			//Check if dates exist
		if (fam.marriageDate.length() != 0 && fam.divorceDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.marriageDate) >= boost::gregorian::from_uk_string(fam.divorceDate)) {
				std::cout << "US04 Fail (Marriage before Divorce) for Family with ID : " << fam.id << std::endl;
				result = false;
			}
		}
		
	}
	return result;
}

///\author LouisRH
bool Gedcom::gedcom::US05() {
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famsId == it_family.id) {
				///\remark Check date entry exists (individual may be un-married)
				if (it_individual.deathDate.length() != 0 && it_family.marriageDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_family.marriageDate) > boost::gregorian::from_uk_string(it_individual.deathDate)) {
						std::cout << "US05 Fail (Marriage before death) for Individual with ID : " << it_individual.id << std::endl;
						result = false;
					}
				}
			}
		}
	}
	return result;
}

///\author LouisRH
bool Gedcom::gedcom::US06() {
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famsId == it_family.id) {
				///\remark Check date entry exists (individual may be un-married)
				if (it_individual.deathDate.length() != 0 && it_family.divorceDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_family.divorceDate) > boost::gregorian::from_uk_string(it_individual.deathDate)) {
						std::cout << "US06 Fail (Divorce before death) for Individual with ID : " << it_individual.id << std::endl;
						result = false;
					}
				}
			}
		}
	}
	return result;
}

///\author gmccorma
bool Gedcom::gedcom::US07() {
	bool result = true;
	bool printed = false;
	for (auto it_individual : individualList) {
		if (it_individual.deathDate != "") {
			// Death should be less than 150 years after birth for dead people
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() > 150) {
				result = false;
				std::cout << "US07 Fail (Less than 150 yeards old) for Individual with ID : " << it_individual.id << std::endl;
				printed = true;
			}
		}

		if (!printed) {
			// Current date should be less than 150 years after birth for all living people
			boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
			boost::gregorian::date dateObj = timeLocal.date();
			if (dateObj.year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() > 150) {
				result = false;
				std::cout << "US07 Fail (Less than 150 yeards old) for Individual with ID : " << it_individual.id << std::endl;
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
				if (it_individual.birthDay.length() != 0 && it_family.marriageDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) <= boost::gregorian::from_uk_string(it_family.marriageDate)) {
						result = false;
						std::cout << "US08 Fail (Birth before marriage of parents) for Individual with ID : " << it_individual.id << std::endl;
					}
				}

				///\remark Check date entry exists (parents' divorce date may be un-available)
				if (it_individual.birthDay.length() != 0 && it_family.divorceDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_individual.birthDay) - boost::gregorian::from_uk_string(it_family.divorceDate) >= boost::gregorian::days(9*30)) {
						result = false;
						std::cout << "US08 Fail (Birth 9 months beyond parents' divorce) for Individual with ID : " << it_individual.id << std::endl;
					}
				}
			}
		}
	}
	return result;
}

///\author vkakde
bool Gedcom::gedcom::US09() {
	bool result = true;
	for (auto it_family : familyList) {
		std::string father_deathDate = it_family.husband.deathDate;
		std::string mother_deathDate = it_family.wife.deathDate;

		for (auto it_child : it_family.children) {
			std::string child_birthDate = it_child.birthDay;

			// check if child born before mother's death
			if (mother_deathDate.length() != 0 && child_birthDate.length() != 0 && boost::gregorian::from_uk_string(child_birthDate) > boost::gregorian::from_uk_string(mother_deathDate)) {
				result = false;
				std::cout << "US09 Fail (Birth before death of mother) for Individual with ID : " << it_child.id << std::endl;
			}

			// check if child born before 9 months after father's death
			if (father_deathDate.length() != 0 && child_birthDate.length() != 0 && boost::gregorian::from_uk_string(child_birthDate) - boost::gregorian::from_uk_string(father_deathDate) >= boost::gregorian::days(9*30)) {
				result = false;
				std::cout << "US09 Fail (Birth before 9 months after of father) for Individual with ID : " << it_child.id << std::endl;
			}
		}
	}
	return result;
}

///\author gmccorma
bool Gedcom::gedcom::US10() {
	// marriage should be at least 14 years after birth of both spouses (parents must be at least 14 years old).
	bool result = true;
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_individual.famsId == it_family.id) {
				if (it_individual.birthDay.length() != 0 && it_family.marriageDate.length() != 0) {
					if (boost::gregorian::from_uk_string(it_family.marriageDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year()
						< 14) {
						result = false;
						std::cout << "US10 Fail (Marriage after 14) for Individuals with IDs : " << it_family.husbandId << ", " << it_family.wifeId << std::endl;
					}
				}
			}
		}
	}
	return result;
}

/** Marriage should not occur during marriage to another spouse **/
bool Gedcom::gedcom::US11() {
	bool result = true;

	//Sets the end date of the marriage as whichever of the following dates is the oldest: divorceDate, huband death date, wife death date
	for (int i = 0; i < familyList.size(); i++) {
		std::string oldestDate = "";
		if (familyList[i].divorceDate == "") {
			if (familyList[i].husband.deathDate != "" && familyList[i].wife.deathDate != "") {
				if (boost::gregorian::from_uk_string(familyList[i].husband.deathDate) > boost::gregorian::from_uk_string(familyList[i].wife.deathDate)) {
					oldestDate = familyList[i].wife.deathDate;
				}
				else {
					oldestDate = familyList[i].husband.deathDate;
				}
			}
			else if (familyList[i].husband.deathDate != "") {
				oldestDate = familyList[i].husband.deathDate;
			}
			else if (familyList[i].wife.deathDate != "") {
				oldestDate = familyList[i].wife.deathDate;
			}
		}
		else {
			oldestDate = familyList[i].divorceDate;
		}
		familyList[i].marriageEndDate = oldestDate;
	}

	for (int i = 0; i < familyList.size(); i++) {
		Family::Family fam = familyList[i];
		std::string marrDate = fam.marriageDate;

		for (int j = i+1; j < familyList.size(); j++) {
			bool bigamyOccurs = false;
			Family::Family otherFam = familyList[j];
			std::string otherMarrDate = otherFam.marriageDate;
			if ((fam.wifeId == otherFam.wifeId) || (fam.husbandId == otherFam.husbandId)) {
				//Now we know that a spouse in one family is also thespouse in another
				//We have to check if: the marriage date of one is before the end date of another, or if both don't have end dates
				if (fam.marriageEndDate != "" && otherFam.marriageDate != "") {
					if (boost::gregorian::from_uk_string(fam.marriageEndDate) > boost::gregorian::from_uk_string(otherFam.marriageDate)) {
						bigamyOccurs = true;
					}
					
				}
				if (otherFam.marriageEndDate != "" && fam.marriageDate != "") {
					if (boost::gregorian::from_uk_string(otherFam.marriageEndDate) > boost::gregorian::from_uk_string(fam.marriageDate)) {
						bigamyOccurs = true;
					}
				}

				if (fam.marriageEndDate == "" && otherFam.marriageEndDate == "") {
					bigamyOccurs = true;
				}

			}
			if (bigamyOccurs) {
				std::cout << "US11 Fail (No Bigamy) for Families with IDs : " << fam.id  << " and " << otherFam.id << std::endl;
				result = false;
			}
		}
	}
	return result;
}

// Helper for US12 (remove eventually)
int stringToInt(std::string str) {
	int num;
	std::stringstream stream(str);
	stream >> num;
	return num;
}

///\author LouisRH
// This is a gross and outdated solution, and will be refactored before the end of sprint 4
bool Gedcom::gedcom::US12() {
	bool result = true;
	std::string fatherID = "";
	std::string motherID = "";
	std::vector<std::string> childrenIDs = {};
	std::string fatherBDay = "";
	std::string motherBDay = "";
	std::vector<std::string> childrenBDays = {};
	int parentYear = 0;
	int childYear = 0;

	for (int i = 0; i < familyList.size(); i++) {
		if (familyList[i].childrenIds.size() > 0) {
			fatherID = familyList[i].husbandId;
			motherID = familyList[i].wifeId;
			childrenIDs = familyList[i].childrenIds;
			for (int k = 0; k < individualList.size(); k++) {
				if (fatherID == individualList[k].id) {
					fatherBDay = individualList[k].birthDay;
				}
				if (motherID == individualList[k].id) {
					motherBDay = individualList[k].birthDay;
				}
				for (int j = 0; j < childrenIDs.size(); j++) {
					if (childrenIDs[j] == individualList[k].id) {
						childrenBDays.push_back(individualList[k].birthDay);
					}
				}
			}
			parentYear = stringToInt(fatherBDay.substr(fatherBDay.length() - 4, 4));
			for (int a = 0; a < childrenBDays.size(); a++) {
				childYear = stringToInt(childrenBDays[a].substr(childrenBDays[a].length() - 4, 4));
				if (parentYear - childYear > 80) {
					std::cout << "US12 Fail (Parents not too old) for Family with ID : " << familyList[i].id << std::endl;
					result = false;
				}
			}
			parentYear = stringToInt(motherBDay.substr(motherBDay.length() - 4, 4));
			for (int a = 0; a < childrenBDays.size(); a++) {
				childYear = stringToInt(childrenBDays[a].substr(childrenBDays[a].length() - 4, 4));
				if (parentYear - childYear > 80) {
					std::cout << "US12 Fail (Parents not too old) for Family with ID : " << familyList[i].id << std::endl;
					result = false;
				}
			}
		}
	}
	return result;
}

/*Birth dates of siblings should be more than 8 months apart or less than 2 days apart*/
bool Gedcom::gedcom::US13() {
	bool result = true;
	for (auto it_family : familyList) {
		if (it_family.children.size() > 1) {
			for (int i = 0; i < it_family.children.size(); i++) {
				std::string childBirthDay = it_family.children[i].birthDay;
				for (int j = i+1; j < it_family.children.size(); j++) {
					std::string otherChildBirthDay = it_family.children[j].birthDay;
					if (otherChildBirthDay != "" && childBirthDay != "") {
						if (boost::gregorian::from_uk_string(childBirthDay).year() == boost::gregorian::from_uk_string(otherChildBirthDay).year()) {
							if (boost::gregorian::from_uk_string(childBirthDay) > boost::gregorian::from_uk_string(otherChildBirthDay)) {
								if ((boost::gregorian::from_uk_string(childBirthDay) - boost::gregorian::from_uk_string(otherChildBirthDay) < boost::gregorian::days(8 * 30)) && (boost::gregorian::from_uk_string(childBirthDay) - boost::gregorian::from_uk_string(otherChildBirthDay) > boost::gregorian::days(2))) {
									std::cout << "US13 Fail (Sibiling Spacing) for children with IDs : " << it_family.children[i].id << " and " << it_family.children[j].id << std::endl;
									result = false;
								}
							}
							if (boost::gregorian::from_uk_string(childBirthDay) < boost::gregorian::from_uk_string(otherChildBirthDay)) {
								if ((boost::gregorian::from_uk_string(otherChildBirthDay) - boost::gregorian::from_uk_string(childBirthDay) < boost::gregorian::days(8 * 30)) && (boost::gregorian::from_uk_string(otherChildBirthDay) - boost::gregorian::from_uk_string(childBirthDay) > boost::gregorian::days(2))) {
									std::cout << "US13 Fail (Sibiling Spacing) for children with IDs : " << it_family.children[i].id << " and " << it_family.children[j].id << std::endl;
									result = false;
								}
							}
							
						}
					}
				}
			}
		}
	}
	return result;
}

///\author gmccorma
bool Gedcom::gedcom::US14() {
	bool result = true;
	int count = 0;
	//std::string fail_indiv_id;
	std::vector<std::string> multsIds;
	for (auto it_family : familyList) {
		// no more than 5 siblings should be born at the same time
		for (std::vector<std::string>::const_iterator i = it_family.childrenIds.begin(); i != it_family.childrenIds.end(); ++i) {
			for (auto it_individual : individualList) {
				// find child
				if (it_individual.id.compare(*i) == 0) {
					// put children birthdays into vector
					//fail_indiv_id = *i;
					multsIds.push_back(it_individual.birthDay);
				}
			}
		}
		for (auto elem : multsIds) {
			count = std::count(multsIds.begin(), multsIds.end(), elem);
		}
		if (count > 5) {
			std::cout << "US14 Fail (Multiple births <= 5) for Family with ID : " << it_family.id << std::endl;
			result = false;
		}
	}
	return result;
}

///\author LouisRH
bool Gedcom::gedcom::US15() {
	bool result = true;
		for (auto it_family : familyList) {
			if (it_family.childrenIds.size() > 15) {
				std::cout << "US15 Fail (Fewer than 15 siblings) for Family with ID : " << it_family.id << std::endl;
				result = false;
			}
		}
	return result;
}

///\author vkakde
bool Gedcom::gedcom::US16() {
	bool result = true;
	for (auto it_family : familyList) {
		std::string father_lastName = it_family.husband.name.substr(it_family.husband.name.find(' '));

		for (auto it_child : it_family.children) {
			std::string child_lastName = it_child.name.substr(it_child.name.find(' '));

			if (father_lastName.length()!=0 && child_lastName.length()!=0 && father_lastName != child_lastName) {
				result = false;
				std::cout << "US16 Fail (Same last name - all male members in family) for Individual with ID : " << it_child.id << std::endl;
			}
		}
	}
	return result;
}

// Helper for US17 and US19
std::vector<Family::Family> findFamilies(std::vector<Family::Family> familyList, std::string indiID) {
	std::vector<Family::Family> memberList = {};
	for (auto it_family : familyList) {
		for (auto child : it_family.children) {
			if (child.id == indiID) {
				memberList.push_back(it_family);
			}
		}
	}
	return memberList;
}

// Helper for US17
bool checkAncestors(std::vector<Family::Family> familyList, std::string originalID, std::string ancestorID) {
	std::vector<Family::Family> memberList = findFamilies(familyList, originalID);
	if (memberList.size() == 0) {
		return true;
	}

	for (auto it_family : memberList) {
		if (ancestorID == it_family.husbandId || ancestorID == it_family.wifeId) {
			return false;
		} else {
			if (!checkAncestors(familyList, it_family.husbandId, ancestorID) ||
				!checkAncestors(familyList, it_family.wifeId, ancestorID)) {
				return false;
			} else {
				return true;
			}
		}
	}

	return true;
}

///\author LouisRH
bool Gedcom::gedcom::US17() {
	bool result = true;
	for (auto it_family : familyList) {
		if (!checkAncestors(familyList, it_family.husbandId, it_family.wifeId) ||
			!checkAncestors(familyList, it_family.wifeId, it_family.husbandId)) {
			result = false;
			std::cout << "US17 Fail (No marriages to descendants) for Individuals with ID : " << it_family.husbandId << " " << it_family.wifeId << std::endl;
			return result;
		}
	}

	return result;
}

///\ author gmccorma
// Siblings should not marry one another
bool Gedcom::gedcom::US18() {
	bool result = true;
	for (auto it_family : familyList) {
		for (auto it_individual : individualList) {
			if (it_family.husband.famsId == it_family.wife.famsId && it_family.husband.famcId == it_family.wife.famcId) {
				result = false;
				std::cout << "US18 Fail (Siblings should not marry) for Individuals with ID : " << it_family.husbandId << " " << it_family.wifeId << std::endl;
				return result;
			}
		}
	}
	return result;
}

///\author LouisRH
// Time and space complexity are atrocious, refactor later
bool Gedcom::gedcom::US19() {
	bool result = true;
	std::vector<std::string> husbGrandfather = {};
	std::vector<std::string> husbGrandmother = {};
	std::vector<std::string> wifeGrandfather = {};
	std::vector<std::string> wifeGrandmother = {};
	std::vector<Family::Family> memberList = {};
	std::vector<Family::Family> memberListFather = {};
	std::vector<Family::Family> memberListMother = {};
	// For each family
	for (auto it_family : familyList) {
		// Husb: For each family in which the husband is a child
		memberList = findFamilies(familyList, it_family.husbandId);
		// For each of those families
		for (auto it_family2 : memberList) {
			// Find all families where the mother and father are children (separately)
			memberListFather = findFamilies(familyList, it_family2.husbandId);
			memberListMother = findFamilies(familyList, it_family2.wifeId);
			// For each family where the father is a child
			for (auto it_family3 : memberListFather) {
				// Record ID of parents
				husbGrandfather.push_back(it_family3.husbandId);
				husbGrandmother.push_back(it_family3.wifeId);
			}
			// For each family where the mother is a child
			for (auto it_family3 : memberListMother) {
				// Record ID of parents
				husbGrandfather.push_back(it_family3.husbandId);
				husbGrandmother.push_back(it_family3.wifeId);
			}
		}

		// Wife: For each family in which the wife is a child
		memberList = findFamilies(familyList, it_family.wifeId);
		// For each of those families
		for (auto it_family2 : memberList) {
			// Find all families where the mother and father are children (separately)
			memberListFather = findFamilies(familyList, it_family2.husbandId);
			memberListMother = findFamilies(familyList, it_family2.wifeId);
			// For each family where the father is a child
			for (auto it_family3 : memberListFather) {
				// Record ID of parents
				wifeGrandfather.push_back(it_family3.husbandId);
				wifeGrandmother.push_back(it_family3.wifeId);
			}
			// For each family where the mother is a child
			for (auto it_family3 : memberListMother) {
				// Record ID of parents
				wifeGrandfather.push_back(it_family3.husbandId);
				wifeGrandmother.push_back(it_family3.wifeId);
			}
		}

		for (auto it_ID : husbGrandfather) {
			for (auto it_ID2 : wifeGrandfather) {
				if (it_ID == it_ID2) {
					result = false;
					std::cout << "US19 Fail (First cousins should not marry) for Individuals with ID : " << it_family.husbandId << " " << it_family.wifeId << std::endl;
					return result;
				}
			}
		}

		for (auto it_ID : husbGrandmother) {
			for (auto it_ID2 : wifeGrandmother) {
				if (it_ID == it_ID2) {
					result = false;
					std::cout << "US19 Fail (First cousins should not marry) for Individuals with ID : " << it_family.husbandId << " " << it_family.wifeId << std::endl;
					return result;
				}
			}
		}
	}

	return result;
}

///\ author gmccorma
// Aunts and uncles should not marry their nieces or nephews
bool Gedcom::gedcom::US20() {
	bool result = true;
	std::string id;
	std::string ind_id;
	for (auto it_family : familyList) {
		// go through parent's family
		for (auto it_individual : individualList) {
			id = it_individual.famsId;
		}
		// go through child's family
		for (std::vector<std::string>::const_iterator i = it_family.childrenIds.begin(); i != it_family.childrenIds.end(); ++i) {
			for (auto it_individual : individualList) {
				// find child
				if (it_individual.id.compare(*i) == 0) {
					// if child spouse id 
					if (it_individual.famsId == id) {
						result = false;
						std::cout << "US20 Fail (Aunts and uncles should not marry their nieces or nephews) for Family with ID : " << id << std::endl;
						return result;
					}
				}
			}
		}
	}
	return result;
}

///\author mjosephs
bool Gedcom::gedcom::US21() {
	bool result = true;

	for (auto it_family : familyList) {
		if (it_family.husband.sex == "F") {
			std::cout << "US21 Fail (Correct gender for role) for Individual with ID : " << it_family.husband.id << std::endl;
			result = false;
		}
		if (it_family.wife.sex == "M") {
			std::cout << "US21 Fail (Correct gender for role) for Individual with ID : " << it_family.husband.id << std::endl;
			result = false;
		}
	}
	return result;
}

///\author mjosephs
bool Gedcom::gedcom::US22() {
	bool result = true;
	std::vector<std::string> famIds;
	std::vector<std::string> indiIds;

	for (auto it_family : familyList) {
		bool isRepeat = false;
		for (auto famId : famIds) {
			if (famId == it_family.id) {
				isRepeat = true;
				result = false;
				std::cout << "US22 Fail (Unique IDs) for Family with ID : " << it_family.id << std::endl;
			}
		}

		if (!isRepeat) {
			famIds.push_back(it_family.id);
		}
	}

	for (auto it_individual : individualList) {
		bool isRepeat = false;
		for (auto indiId : indiIds) {
			if (indiId == it_individual.id) {
				isRepeat = true;
				result = false;
				std::cout << "US22 Fail (Unique IDs) for Individual with ID : " << it_individual.id << std::endl;
			}
		}

		if (!isRepeat) {
			indiIds.push_back(it_individual.id);
		}
	}

	return result;
}

///\author vkakde
bool Gedcom::gedcom::US23() {
	bool result = true;
	for (int i = 0; i < individualList.size(); i++) {
		for (int j = 0; j < individualList.size(); j++) {
			if (i != j && individualList[i].name == individualList[j].name && individualList[i].birthDay == individualList[j].birthDay) {
				std::cout << "US23 Fail (More than 1 individual with same name and DoB) for Individuals with ID : " << individualList[i].id << " and " << individualList[j].id << std::endl;
			}
		}
	}
	return result;
}

///\author vkakde
///\remark Since 'GIVN' and 'MARNM' are not among supported tags for our project, we eliminate the concept of given and marriage names.
bool Gedcom::gedcom::US24() {
	bool result = true;
	for (int i = 0; i < familyList.size(); i++) {
		for (int j = 0; j < familyList.size(); j++) {
			if (i != j && familyList[i].husband.name == familyList[j].husband.name && familyList[i].wife.name == familyList[j].wife.name && familyList[i].marriageDate == familyList[j].marriageDate) {
				std::cout << "US24 Fail (More than 1 family with same spouse names and DoM) for Families with ID : " << familyList[i].id << " and " << familyList[j].id << std::endl;
			}
		}
	}
	return result;
}

///\author mjosephs
//No more than one child with the same name and birth date should appear in a family
bool Gedcom::gedcom::US25() {
	bool result = true;
	for (auto it_family : familyList) {
		std::vector<std::string> childrenNames = {};
		for (auto child : it_family.children) {
			childrenNames.push_back(child.name);
		}

		std::set<std::string> uniqueChildrenList(childrenNames.begin(), childrenNames.end());
		if (childrenNames.size() != uniqueChildrenList.size()) {
			std::cout << "US25 Fail (More than one child with the same name) for Family with ID : " << it_family.id << std::endl;
		}
	}

	return result;
}
