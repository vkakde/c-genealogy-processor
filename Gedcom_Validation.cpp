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
				std::cout << "\nUS01 Fail (Date before Current Date) for Individual with ID : " << it_individual.id << "!\n";
				result = false;
			}
		}
		if (it_individual.deathDate.length() != 0) {
			if (boost::gregorian::from_uk_string(it_individual.deathDate) >= today) {
				std::cout << "\nUS01 Fail (Date before Current Date) for Individual with ID : " << it_individual.id << "!\n";
				result = false;
			}
		}
	}
	for (auto fam : familyList) {
		//Check date entry exists
		if (fam.divorceDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.divorceDate) >= today) {
				std::cout << "\nUS01 Fail (Date before Current Date) for Family with ID : " << fam.id << "!\n";
				result = false;
			}
		}
		if (fam.marrDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.marrDate) >= today) {
				std::cout << "\nUS01 Fail (Date before Current Date) for Family with ID : " << fam.id << "!\n";
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

///\author gmccorma
bool Gedcom::gedcom::US03() {
	bool result = true;
	for (auto it_individual : individualList) {
		// Tests to see if the birth date is after the death date.  Returns false if that is the case, true otherwise.
		if (it_individual.deathDate != "") {
			std::cout << "Birth: " << boost::gregorian::from_uk_string(it_individual.birthDay).year() << std::endl;
			std::cout << "Death: " << boost::gregorian::from_uk_string(it_individual.deathDate).year() << std::endl;
			// if birth and death year same, and birth and death month same, and birth day greater than death day
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() == 0) {
				if (boost::gregorian::from_uk_string(it_individual.deathDate).month() - boost::gregorian::from_uk_string(it_individual.birthDay).month() == 0) {
					if (boost::gregorian::from_uk_string(it_individual.deathDate).day() - boost::gregorian::from_uk_string(it_individual.birthDay).day() < 0) {
						std::cout << "\nUS03 Fail (Birth before death) for Individual with ID : " << it_individual.id << "!\n";
						result = false;
					}
				}
			}
			// if birth and death year same, and birth month greater than death month
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() == 0) {
				if (boost::gregorian::from_uk_string(it_individual.deathDate).month() - boost::gregorian::from_uk_string(it_individual.birthDay).month() < 0) {
					std::cout << "\nUS03 Fail (Birth before death) for Individual with ID : " << it_individual.id << "!\n";
					result = false;
				}
			}
			// if birth year greater than death year
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() < 0) {
				std::cout << "\nUS03 Fail (Birth before death) for Individual with ID : " << it_individual.id << "!\n";
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
		if (fam.marrDate.length() != 0 && fam.divorceDate.length() != 0) {
			if (boost::gregorian::from_uk_string(fam.marrDate) >= boost::gregorian::from_uk_string(fam.divorceDate)) {
				std::cout << "\nUS04 Fail (Marriage before Divorce) for Family with ID : " << fam.id << "!\n";
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
				if (it_individual.deathDate.length() != 0 && it_family.marrDate.length() != 0) {
					///\cite http://thispointer.com/how-to-convert-string-to-date-in-c-using-boost-library/
					if (boost::gregorian::from_uk_string(it_family.marrDate) > boost::gregorian::from_uk_string(it_individual.deathDate)) {
						std::cout << "\nUS05 Fail (Marriage before death) for Individual with ID : " << it_individual.id << "!\n";
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
						std::cout << "\nUS06 Fail (Divorce before death) for Individual with ID : " << it_individual.id << "!\n";
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
	for (auto it_individual : individualList) {
		if (it_individual.deathDate != "") {
			// Death should be less than 150 years after birth for dead people
			if (boost::gregorian::from_uk_string(it_individual.deathDate).year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() > 150) {
				result = false;
				std::cout << "\nUS07 Fail (Less than 150 yeards old) for Individual with ID : " << it_individual.id << "!\n";
			}
		}
		// Current date should be less than 150 years after birth for all living people
		boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
		boost::gregorian::date dateObj = timeLocal.date();
		if (dateObj.year() - boost::gregorian::from_uk_string(it_individual.birthDay).year() > 150) {
			result = false;
			std::cout << "\nUS07 Fail (Less than 150 yeards old) for Individual with ID : " << it_individual.id << "!\n";
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