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

	std::cout << "------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|         Name         " << std::endl;
	std::cout << "------------------------------------------" << std::endl;

	for (int i = 0; i < individualList.size(); ++i) {
		std::cout << "     " << std::setw(10) << std::left << individualList[i].id << "||    " << individualList[i].name << "    " << std::endl;
	}
	std::cout << "------------------------------------------" << std::endl;
}

void Gedcom::gedcom::printFamilyTable() {
	std::string husb_name;
	std::string wife_name;

	std::cout << "\n" << "Families" << std::endl;

	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|   Husband ID  |" << "|      Husband Name      |" << "|    Wife ID    |" << "|       Wife Name       " << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < familyList.size(); ++i) {
		// find husb name based on id
		for (int j = 0; j < individualList.size(); j++) {
			if (familyList[i].husb == individualList[j].id) {
				husb_name = individualList[j].name;
				break;
			}
		}
		// find wife name based on id
		for (int k = 0; k < individualList.size(); k++) {
			if (familyList[i].wife == individualList[k].id) {
				wife_name = individualList[k].name;
				break;
			}
		}
		std::cout << "     " << std::setw(10) << std::left << familyList[i].id
			<< "||     " << std::setw(10) << std::left << familyList[i].husb
			<< "||    " << std::setw(20) << std::left << husb_name
			<< "||     " << std::setw(10) << std::left << familyList[i].wife
			<< "||    " << std::setw(20) << std::left << wife_name << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}