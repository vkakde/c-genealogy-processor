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
		bool bigamyOccurs = false;

		for (int j = i+1; j < familyList.size(); j++) {
			Family::Family otherFam = familyList[j];
			std::string otherMarrDate = fam.marriageDate;
			if ((fam.wifeId == otherFam.wifeId) || (fam.husbandId == otherFam.husbandId)) {
				//Now we know that a spouse in one family is also thespouse in another
				//We have to check if: the marriage date of one is before the end date of another, or if both don't have end dates
				if (fam.marriageEndDate != "" && otherFam.marriageDate != "") {
					if (boost::gregorian::from_uk_string(fam.marriageEndDate) > boost::gregorian::from_uk_string(otherFam.marriageDate)) {
						bigamyOccurs = true;
					}
				}
				if (otherFam.marriageEndDate != "") {
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
// This is a gross and outdated solution, and will be refactored before the end of sprint 3
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
	for (auto it_individual : individualList) {
		for (auto it_family : familyList) {
			if (it_family.childrenIds.size() > 15) {
				std::cout << "US15 Fail (Fewer than 15 siblings) for Family with ID : " << it_family.id << std::endl;
				result = false;
			}
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