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