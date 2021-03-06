#include "stdafx.h"
#include "Gedcom.h"

Gedcom::gedcom::gedcom(std::string file) {
	supportedTagsAndLevels.insert(std::make_pair("INDI", 0));
	supportedTagsAndLevels.insert(std::make_pair("NAME", 1));
	supportedTagsAndLevels.insert(std::make_pair("SEX", 1));
	supportedTagsAndLevels.insert(std::make_pair("BIRT", 1));
	supportedTagsAndLevels.insert(std::make_pair("DEAT", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAMC", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAMS", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAM", 0));
	supportedTagsAndLevels.insert(std::make_pair("MARR", 1));
	supportedTagsAndLevels.insert(std::make_pair("HUSB", 1));
	supportedTagsAndLevels.insert(std::make_pair("WIFE", 1));
	supportedTagsAndLevels.insert(std::make_pair("CHIL", 1));
	supportedTagsAndLevels.insert(std::make_pair("DIV", 1));
	supportedTagsAndLevels.insert(std::make_pair("DATE", 2));
	supportedTagsAndLevels.insert(std::make_pair("HEAD", 0));
	supportedTagsAndLevels.insert(std::make_pair("TRLR", 0));
	supportedTagsAndLevels.insert(std::make_pair("NOTE", 0));

	deathDateFlag = false;
	birthDateFlag = false;
	marrDateFlag = false;
	divDateFlag = false;

	readGedFile(file);
}

Gedcom::gedcom::gedcom() {
	//supportedTagsAndLevels = { { "INDI",0 },{ "NAME",1 },{ "SEX",1 },{ "BIRT",1 },{ "DEAT",1 },{ "FAMC",1 },{ "FAMS",1 },{ "FAM",0 },{ "MARR",1 },{ "HUSB",1 },{ "WIFE",1 },
	//{ "CHIL",1 },{ "DIV",1 },{ "DATE",2 },{ "HEAD",0 },{ "TRLR",0 },{ "NOTE",0 } };

	supportedTagsAndLevels.insert(std::make_pair("INDI", 0));
	supportedTagsAndLevels.insert(std::make_pair("NAME", 1));
	supportedTagsAndLevels.insert(std::make_pair("SEX", 1));
	supportedTagsAndLevels.insert(std::make_pair("BIRT", 1));
	supportedTagsAndLevels.insert(std::make_pair("DEAT", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAMC", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAMS", 1));
	supportedTagsAndLevels.insert(std::make_pair("FAM", 0));
	supportedTagsAndLevels.insert(std::make_pair("MARR", 1));
	supportedTagsAndLevels.insert(std::make_pair("HUSB", 1));
	supportedTagsAndLevels.insert(std::make_pair("WIFE", 1));
	supportedTagsAndLevels.insert(std::make_pair("CHIL", 1));
	supportedTagsAndLevels.insert(std::make_pair("DIV", 1));
	supportedTagsAndLevels.insert(std::make_pair("DATE", 2));
	supportedTagsAndLevels.insert(std::make_pair("HEAD", 0));
	supportedTagsAndLevels.insert(std::make_pair("TRLR", 0));
	supportedTagsAndLevels.insert(std::make_pair("NOTE", 0));

	deathDateFlag = false;
	birthDateFlag = false;
	marrDateFlag = false;
	divDateFlag = false;
}

void Gedcom::gedcom::readGedFile(std::string file) {
	///cite https://stackoverflow.com/a/19922123
	std::ifstream inFile;
	inFile.open(file);
	if (!inFile) {              // open file
		std::cout << "\nFile open failed. Restart program and try again.\n";
		std::cout << "Possible reasons for failure: \n\t1. Does the file exist?\n";
		std::cout << "\t2. Did you forget to add the .ged extension?\n";
		std::cout << "\t3. Mind the forward and reverse slashes depending on your OS.";
		std::cout << "\tFor simplicity - place input ged in same folder as the source file.\n";
	}
	std::stringstream strStream;
	strStream << inFile.rdbuf();        // read file
	gedcomData = strStream.str();       // read file contents into string
}



	// public functions
void Gedcom::gedcom::readGedFile() {
	// prompt user input
	std::cout << "\nEnter file path+name of .ged file relative to workspace (ex: gedcom/file.ged): ";
	std::string file;
	std::cin >> file;

	///cite https://stackoverflow.com/a/19922123
	std::ifstream inFile;
	inFile.open(file);
	if (!inFile) {              // open file
		std::cout << "\nFile open failed. Restart program and try again.\n";
		std::cout << "Possible reasons for failure: \n\t1. Does the file exist?\n";
		std::cout << "\t2. Did you forget to add the .ged extension?\n";
		std::cout << "\t3. Mind the forward and reverse slashes depending on your OS.";
		std::cout << "\tFor simplicity - place input ged in same folder as the source file.\n";
	}
	std::stringstream strStream;
	strStream << inFile.rdbuf();        // read file
	gedcomData = strStream.str();       // read file contents into string
}

std::string Gedcom::gedcom::isIndiOrFam(std::vector<std::string> lineElements) {
	///cite https://stackoverflow.com/a/19266247/2535979
	std::locale loc;
	std::string lineElement_index2;
	if (lineElements.size()>2) {
		for (auto i : lineElements[2]) {
			lineElement_index2 += std::tolower(i);
		}

		if (lineElement_index2 == "indi" || lineElement_index2 == "fam") {
			return lineElement_index2;
		}
	}
	return "";
}

void Gedcom::gedcom::parseLine(std::string unparsedGedcomLine) {
	std::vector<std::string> lineElements;
	std::string parsedGedcomLine = "";

	/** break line into individual elements (id/tags/arguments) **/
	for (auto i = 0; i<unparsedGedcomLine.length(); i++) {
		std::string lineElement = "";
		///remark gedcom lines can have up to 3 attributes. 
		///       Thus, every literal past second attribute automatically belongs to 3rd attribute.
		if (lineElements.size() == 2) {
			while (i<unparsedGedcomLine.length()) {
				lineElement += unparsedGedcomLine[i++];
			}
		}
		else {
			while (unparsedGedcomLine[i] != ' ' && i<unparsedGedcomLine.length()) {
				lineElement += unparsedGedcomLine[i++];
			}
		}

		lineElements.push_back(lineElement);
	}

	/** generate parsed gedcom line **/
	// copy first element as is
	parsedGedcomLine += lineElements[0];
	// add delimiter
	parsedGedcomLine += '|';

	// process second and third elements depending on tag (is it INDI/FAM)
	std::string indiOrFam = isIndiOrFam(lineElements);
	bool tagIsValid = false;
	if (indiOrFam == "") {
		parsedGedcomLine += lineElements[1];

		// add delimiter
		parsedGedcomLine += '|';

		// is tag valid?
		///remark A valid tag is one that is within the scope of cs555 project, refer doc "Project Overview"
		if (isTagValid(lineElements[0], lineElements[1], 1)) {
			parsedGedcomLine += "Y";
			tagIsValid = true;

		}
		else {
			parsedGedcomLine += "N";
		}

		if (lineElements.size()>2) {
			// add delimiter
			parsedGedcomLine += '|';

			// append arguments
			parsedGedcomLine += lineElements[2];

			if (lineElements[1] == "DEAT") {
				deathDateFlag = true;
			}
			if (lineElements[1] == "BIRT") {
				birthDateFlag = true;
			}
			if (lineElements[1] == "MARR") {
				marrDateFlag = true;
			}
			if (lineElements[1] == "DIV") {
				divDateFlag = true;
			}

			//We know that there's a tag and a corresponding attribute, so add it:
			if (lineElements[1] == "DATE") {
				if (tagIsValid) {
					if (deathDateFlag) {
						addAttribute("DEAT", lineElements[2]);
					}
					if (birthDateFlag) {
						addAttribute("BIRT", lineElements[2]);
					}
					if (marrDateFlag) {
						addAttribute("MARR", lineElements[2]);
					}
					if (divDateFlag) {
						addAttribute("DIV", lineElements[2]);
					}
				}

				deathDateFlag = false;
				birthDateFlag = false;
				marrDateFlag = false;
				divDateFlag = false;
			}
			else {
				if (tagIsValid) {
					addAttribute(lineElements[1], lineElements[2]);
				}
			}
		}
		//These should only consist of the tags before dates. Save this tag so we can get its date on the next iteration 
		
		else {
			if (lineElements[1] == "DEAT") {
				deathDateFlag = true;
			}
			if (lineElements[1] == "BIRT") {
				birthDateFlag = true;
			}
			if (lineElements[1] == "MARR") {
				marrDateFlag = true;
			}
			if (lineElements[1] == "DIV") {
				divDateFlag = true;
			}
		}
		
	}
	else {
		parsedGedcomLine += lineElements[2];

		// add delimiter
		parsedGedcomLine += '|';

		// is tag valid?
		///remark A valid tag is one that is within the scope of cs555 project, refer doc "Project Overview"
		if (isTagValid(lineElements[0], lineElements[2], 2)) {
			parsedGedcomLine += "Y";
			if (indiOrFam == "fam") {
				Family::Family family(lineElements[1]);
				familyList.push_back(family);

				isInd = false;
			}
			else {
				Individual::Individual individual(lineElements[1]);
				individualList.push_back(individual);

				isInd = true;
			}
		}
		else {
			parsedGedcomLine += "N";
		}

		if (lineElements.size()>2) {
			// add delimiter
			parsedGedcomLine += '|';

			// append arguments
			parsedGedcomLine += lineElements[1];
		}
	}
}

void Gedcom::gedcom::parseAll() {
	for (auto i = 0; i<gedcomData.length(); i++) {
		std::string gedcomLine = "";
		while (gedcomData[i] != '\n' && gedcomData[i] != '\0' && gedcomData[i] != '\r' && i<gedcomData.length()) {
			gedcomLine += gedcomData[i++];
		}
		parseLine(gedcomLine);
	}
}

void Gedcom::gedcom::addAttribute(std::string tag, std::string attribute) {
	int index;
	if (isInd) {
		index = individualList.size() - 1;
		if (index < 0) {
			return;
		}
		//Add the attribute to the last individual in the vector (individualList)
		if (tag == "NAME") {
			individualList[index].name = attribute;
		}
		if (tag == "SEX") {
			individualList[index].sex = attribute;
		}
		if (tag == "FAMS") {
			individualList[index].famsId = attribute;
		}
		if (tag == "FAMC") {
			individualList[index].famcId = attribute;
		}
		if (tag == "BIRT") {
			individualList[index].birthDay = attribute;
		}
		if (tag == "DEAT") {
			individualList[index].deathDate = attribute;
		}
	}
	else {
		index = familyList.size() - 1;
		if (index < 0) {
			return;
		}
		if (tag == "HUSB") {
			familyList[index].husbandId = attribute;
		}
		if (tag == "WIFE") {
			familyList[index].wifeId = attribute;
		}
		if (tag == "CHIL") {
			familyList[index].childrenIds.push_back(attribute);
		}
		if (tag == "MARR") {
			familyList[index].marriageDate = attribute;
		}
		if (tag == "DIV") {
			familyList[index].divorceDate = attribute;
		}
	}
}

void Gedcom::gedcom::addIndividualsToFamilies() {
	for (int i = 0; i < familyList.size(); i++) {
		for (auto it_individual : individualList) {
			if (familyList[i].husbandId == it_individual.id) {
				familyList[i].husband = it_individual;
			}
			if (familyList[i].wifeId == it_individual.id) {
				familyList[i].wife = it_individual;
			}
			for (auto child_id : familyList[i].childrenIds) {
				if (child_id == it_individual.id) {
					familyList[i].children.push_back(it_individual);
				}
			}
		}
	}
}