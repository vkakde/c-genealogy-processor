///brief This program parses and processes genealogy files in the GEDCOM format
///authors Matthew Josephs, Louis Rozencwajg-Hays, Gabrielle Mccormack, Vishwajeet Kakde
// Link to github: https://github.com/vkakde/sw555MLGV

// include headers
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

class Individual {
public:
    //No getters/setters for this many fields - it shouldn't be necessary
    std::string id;
    std::string name;
    std::string sex;
    std::string birthDay;
    std::string deathDate;
    std::string famcId;
    std::string famsId;

    Individual(std::string cId) {
        id = cId;
    }
};

class Family {
public:
    std::string id;
    std::string husb;
    std::string wife;
    std::string marrDate;
    std::string divorceDate;
    std::vector<std::string> children;

    Family(std::string cId) {
        id = cId;
    }    
};

class gedcom {
    std::map<std::string, int> supportedTagsAndLevels;
    std::string gedcomData;
    std::vector<Family> familyList = {};
    std::vector<Individual> individualList = {};
    std::string currentId;
    bool isInd;
    bool deathDate = false;
    bool birthDate = false;
    bool marrDate = false;
    bool divDate = false;

    // constructors
public:
    gedcom() {
        supportedTagsAndLevels = { { "INDI",0 },{ "NAME",1 },{ "SEX",1 },{ "BIRT",1 },{ "DEAT",1 },{ "FAMC",1 },{ "FAMS",1 },{ "FAM",0 },{ "MARR",1 },{ "HUSB",1 },{ "WIFE",1 },
        { "CHIL",1 },{ "DIV",1 },{ "DATE",2 },{ "HEAD",0 },{ "TRLR",0 },{ "NOTE",0 } };
    }

    // public functions
    void readGedFile() {
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

    void printGedcomData() {
        std::cout << gedcomData << "\n";
    }

    void printSupportedTags() {
        for (auto& i : supportedTagsAndLevels) {
            std::cout << "\n" << i.first << "  " << i.second;
        }
    }

    /*
    bool isIndiOrFam(std::vector<std::string> lineElements) {
    ///cite https://stackoverflow.com/a/19266247/2535979
    std::locale loc;
    std::string lineElement_index2;
    if (lineElements.size()>2) {
    for (auto i : lineElements[2]) {
    lineElement_index2 += std::tolower(i, loc);
    }

    if (lineElement_index2 == "indi" || lineElement_index2 == "fam") {
    return true;
    }
    }
    return false;
    }
    */

    std::string isIndiOrFam(std::vector<std::string> lineElements) {
        ///cite https://stackoverflow.com/a/19266247/2535979
        std::locale loc;
        std::string lineElement_index2;
        if (lineElements.size()>2) {
            for (auto i : lineElements[2]) {
                lineElement_index2 += std::tolower(i, loc);
            }

            if (lineElement_index2 == "indi" || lineElement_index2 == "fam") {
                return lineElement_index2;
            }
        }
        return "";
    }
    
    bool isTagValid(std::string tagLevel, std::string tagName, int tagIndex) {
        // convert tag under test to lower case
        ///cite https://stackoverflow.com/a/19266247/2535979
        std::locale loc;
        std::string testTagName_LowerCase;
        for (auto i : tagName) {
            testTagName_LowerCase += std::tolower(i, loc);
        }

        // convert all supported tags to lower case (for comparison) 
        std::map<std::string, std::string> supportedTagsAndLevels_LowerCase;
        for (auto tag : supportedTagsAndLevels) {
            std::string loweredTag = "";
            std::string level = std::to_string(tag.second);
            for (auto i : tag.first) {
                loweredTag += std::tolower(i, loc);
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
                    if(testTagName_LowerCase=="fam" || testTagName_LowerCase=="indi"){
                        if(tagIndex == 2){
                            return true;
                        }
                    }
                    else{
                        if(tagIndex == 1){
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    // parse and process a single line of gedcom data
    void parseLine(std::string unparsedGedcomLine) {
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

                //We know that there's a tag and a corresponding attribute, so add it:
                if (lineElements[1] == "DATE") {
					if(tagIsValid){
						if (deathDate) {
							addAttribute("DEAT", lineElements[2]);
						}
						if (birthDate) {
							addAttribute("BIRT", lineElements[2]);
						}
						if (marrDate) {
							addAttribute("MARR", lineElements[2]);
						}
						if (divDate) {
							addAttribute("DIV", lineElements[2]);
						}
					}
					
					deathDate = false;
					birthDate = false;
					marrDate = false;
					divDate = false;
                }
                else{
					if(tagIsValid){
						addAttribute(lineElements[1], lineElements[2]);
					}
                }
            }
            //These should only consist of the tags before dates. Save this tag so we can get its date on the next iteration 
            else {
                if (lineElements[1] == "DEAT") {
                    deathDate = true;
                }
                if (lineElements[1] == "BIRT") {
                    birthDate = true;
                }
                if (lineElements[1] == "MARR") {
                    marrDate = true;
                }
                if (lineElements[1] == "DIV") {
                    divDate = true;
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
					Family family(lineElements[1]);
					familyList.push_back(family);

					isInd = false;
				}
				else {
					Individual individual(lineElements[1]);
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

/*        // print unparsed line
        std::cout << "\n--> " << unparsedGedcomLine;

        // print parsed line
        std::cout << "\n<-- " << parsedGedcomLine;*/
    }

    // parse and iteratively process all lines of gedcom data
    void parseAll() {
        for (auto i = 0; i<gedcomData.length(); i++) {
            std::string gedcomLine = "";
            while (gedcomData[i] != '\n' && gedcomData[i] != '\0' && gedcomData[i] != '\r' && i<gedcomData.length()) {
                gedcomLine += gedcomData[i++];
            }
            parseLine(gedcomLine);
        }

        //Test to see if lists were created correctly:
/*        for(int i = 0; i < individualList.size(); i++){
            std::cout << "\n" << individualList[i].id;
            std::cout << "\n" << individualList[i].name;
            
        }
        for(int i = 0; i < familyList.size(); i++){
            std::cout << "\n" << familyList[i].id;
        }*/
        
    }

    void addAttribute(std::string tag, std::string attribute) {
        int index;
        if (isInd) {
            index = individualList.size()-1;
            if(index < 0){
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
            if(index < 0){
                return;
            }
            if (tag == "HUSB") {
                familyList[index].husb = attribute;
            }
            if (tag == "WIFE") {
                familyList[index].wife = attribute;
            }
            if (tag == "CHIL") {
                familyList[index].children.push_back(attribute);
            }
            if (tag == "MARR") {
                familyList[index].marrDate = attribute;
            }
            if (tag == "DIV") {
                familyList[index].divorceDate = attribute;
            }
        }
    }

    void printIndividualTable() {

    	std::cout <<"\n" << "Individuals" << std::endl;

    	std::cout << "------------------------------------------" << std::endl;
    	std::cout << "      ID       |" << "|         Name         " << std::endl;
    	std::cout << "------------------------------------------" << std::endl;


    	for (int i = 0; i < individualList.size(); ++i) {
    		std::cout << "     " << std::setw(10) << std::left << individualList[i].id << "||    " << individualList[i].name << "    " << std::endl;
    	}
        std::cout << "------------------------------------------" << std::endl;
    }

    void printFamilyTable() {
    	std::string husb_name;
    	std::string wife_name;

    	std::cout <<"\n" << "Families" << std::endl;

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
};

int main() {
    // create gedcom object instance
    gedcom gedcomInstance;

    // read gedcom file (file path+name will be prompted for; to be provided by user)
    gedcomInstance.readGedFile();

    // print input gedcom data
    // gedcomInstance.printGedcomData();

    // parse gedcom data
    gedcomInstance.parseAll();

    // print table of reords showing individuals' data
    gedcomInstance.printIndividualTable();

    // print table of reords showing families' data
    gedcomInstance.printFamilyTable();

    return 0;
}
