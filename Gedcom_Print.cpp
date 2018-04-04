#include "stdafx.h"
#include "Gedcom.h"

void Gedcom::gedcom::printGedcomData() {
	std::cout << gedcomData << "\n";
}

void Gedcom::gedcom::printSupportedTags() {
	for (auto& i : supportedTagsAndLevels) {
		std::cout << "\n" << i.first << "  " << i.second;
	}
}

void Gedcom::gedcom::printIndividualTable() {
	std::cout << "\n" << "Individuals" << std::endl;

	std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|            Name            |" << "|   Gender   |" << "|      Birthday      |" << "|     Death Date    " << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < individualList.size(); ++i) {
		std::cout << "     " << std::setw(10) << std::left << individualList[i].id 
			<< "||    " << std::setw(20) << std::left << individualList[i].name << "    "
			<< "||     " << std::setw(5) << std::left << individualList[i].sex << "  "
			<< "||     " << std::setw(15) << std::left << individualList[i].birthDay
			<< "||     " << std::setw(15) << std::left << individualList[i].deathDate << std::endl;
	}
	std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
}

void Gedcom::gedcom::printFamilyTable() {
	std::string husb_name;
	std::string wife_name;

	std::cout << "\n" << "Families" << std::endl;

	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|   Husband ID  |" << "|      Husband Name      |" << "|    Wife ID    |" << "|       Wife Name        |" << "|     Marriage Date      |" << "|      Divorce Date      |" << "|     Children IDs     " << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < familyList.size(); ++i) {
		std::string childrenIds = "";

		for (int j = 0; j < familyList[i].childrenIds.size(); j++) {
			std::string childId = familyList[i].childrenIds[j];
			if (j == familyList[i].childrenIds.size() - 1) {
				childrenIds += childId;
			}
			else {
				childrenIds += childId + ", ";
			}
		}
		std::cout << "     " << std::setw(10) << std::left << familyList[i].id
			<< "||     " << std::setw(10) << std::left << familyList[i].husbandId
			<< "||    " << std::setw(20) << std::left << familyList[i].husband.name
			<< "||     " << std::setw(10) << std::left << familyList[i].wifeId
			<< "||    " << std::setw(20) << std::left << familyList[i].wife.name
			<< "||    " << std::setw(20) << std::left << familyList[i].marriageDate
			<< "||    " << std::setw(20) << std::left << familyList[i].divorceDate
			<< "||    " << childrenIds <<std::endl;
	}
	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

///\author gmccorma
// List siblings in families by decreasing age, i.e. oldest siblings first
void Gedcom::gedcom::US28() {
	//const int count = 0;
	std::vector<std::string> childIds;
	
	for (auto it_family : familyList) {
		// get dates of birth of each child
		//std::cout << "Siblings in family " << it_family.id << ": " << it_family.childrenIds << std::endl;
		for (std::vector<std::string>::const_iterator i = it_family.childrenIds.begin(); i != it_family.childrenIds.end(); ++i) {
			for (auto it_individual : individualList) {
				// find child, put into vector list
				if (it_individual.id.compare(*i) == 0) {
					childIds.push_back(it_individual.birthDay);
					// count number of children in family
					for (auto elem : it_family.childrenIds) {
						const int count = std::count(it_family.childrenIds.begin(), it_family.childrenIds.end(), elem);
					}
					//std::string arr[count] = {};
					std::cout << "Siblings in family " << it_family.id << ": " << it_individual.id << std::endl;
				}
			}
		}

	}
}

///\author gmccorma
// List all deceased individuals in a GEDCOM file
void Gedcom::gedcom::US29() {
	for (auto it_individual : individualList) {
		if (it_individual.deathDate != "") {
			std::cout << "Deceased individual with ID: " << it_individual.id << std::endl;
		}
	}
}