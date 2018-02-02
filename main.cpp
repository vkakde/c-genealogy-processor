///brief This program parses and processes genealogy files in the GEDCOM format
///author Vishwajeet Kakde

// include headers
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

class gedcom{
    std::map<std::string, int> supportedTagsAndLevels;
    std::string gedcomData;

    // constructors
    public:
    gedcom(){
        supportedTagsAndLevels = {{"INDI",0}, {"NAME",1}, {"SEX",1}, {"BIRT",1}, {"DEAT",1}, {"FAMC",1}, {"FAMS",1}, {"FAM",0}, {"MARR",1}, {"HUSB",1}, {"WIFE",1},
                        {"CHIL",1}, {"DIV",1}, {"DATE",2}, {"HEAD",0}, {"TRLR",0}, {"NOTE",0}};
    }

    // public functions
    void readGedFile(){
        // prompt user input
        std::cout<<"\nEnter file path+name of .ged file relative to workspace (ex: gedcom/file.ged): ";
        std::string file;
        std::cin>>file;

        ///cite https://stackoverflow.com/a/19922123
        std::ifstream inFile;
        inFile.open(file);
        if(!inFile){              // open file
            std::cout<<"\nFile open failed. Restart program and try again.\n";
            std::cout<<"Possible reasons for failure: \n\t1. Does the file exist?\n";
            std::cout<<"\t2. Did you forget to add the .ged extension?\n";
            std::cout<<"\t3. Mind the forward and reverse slashes depending on your OS.";
            std::cout<<"\tFor simplicity - place input ged in same folder as the source file.\n";
        }
        std::stringstream strStream;
        strStream << inFile.rdbuf();        // read file
        gedcomData = strStream.str();       // read file contents into string
    }

    void printGedcomData(){
        std::cout << gedcomData << "\n";
    }

    void printSupportedTags(){
        for(auto& i : supportedTagsAndLevels){
            std::cout<<"\n"<<i.first<<"  "<<i.second;
        }
    }

    bool isIndiOrFam(std::vector<std::string> lineElements){
        ///cite https://stackoverflow.com/a/19266247/2535979
        std::locale loc;
        std::string lineElement_index2;
        if(lineElements.size()>2){
            for(auto i : lineElements[2]){
                lineElement_index2 += std::tolower(i,loc);
            }
        
            if(lineElement_index2 == "indi" || lineElement_index2 == "fam"){
                return true;
            }
        }
        return false;
    }

    bool isTagValid(std::string tagLevel, std::string tagName){
        // convert tag under test to lower case
        ///cite https://stackoverflow.com/a/19266247/2535979
        std::locale loc;
        std::string testTagName_LowerCase;
        for(auto i : tagName){
            testTagName_LowerCase += std::tolower(i,loc);
        }       

        // convert all supported tags to lower case (for comparison) 
        std::map<std::string, std::string> supportedTagsAndLevels_LowerCase;
        for(auto tag: supportedTagsAndLevels){
            std::string loweredTag = "";
            std::string level = std::to_string(tag.second);
            for(auto i : tag.first){
                loweredTag += std::tolower(i,loc);
            }
            supportedTagsAndLevels_LowerCase.insert(std::make_pair(loweredTag, level));       
        }

        // if tag under test matches any supported tag and the tag's level is correct, it is valid. Return true.
        for(auto i: supportedTagsAndLevels_LowerCase){
            if(i.first == testTagName_LowerCase){
                // Okay, tag is supported. But is the level valid?
                if(i.second == tagLevel){
                    return true;
                }
            }
        }
        return false;        
    }

    // parse and process a single line of gedcom data
    void parseLine(std::string unparsedGedcomLine){
        std::vector<std::string> lineElements;
        std::string parsedGedcomLine = "";
        
        /** break line into individual elements (id/tags/arguments) **/
        for(auto i=0; i<unparsedGedcomLine.length(); i++){
            std::string lineElement = "";
            ///remark gedcom lines can have up to 3 attributes. 
            ///       Thus, every literal past second attribute automatically belongs to 3rd attribute.
            if(lineElements.size()==2){
                while(i<unparsedGedcomLine.length()){
                    lineElement+=unparsedGedcomLine[i++];
                }
            }
            else{
                while(unparsedGedcomLine[i]!=' ' && i<unparsedGedcomLine.length()){
                    lineElement+=unparsedGedcomLine[i++];
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
        if(!isIndiOrFam(lineElements)){
            parsedGedcomLine += lineElements[1];

            // add delimiter
            parsedGedcomLine += '|';

            // is tag valid?
            ///remark A valid tag is one that is within the scope of cs555 project, refer doc "Project Overview"
            if(isTagValid(lineElements[0], lineElements[1])){
                parsedGedcomLine += "Y";
            }
            else{
                parsedGedcomLine += "N";
            }

            if(lineElements.size()>2){
                // add delimiter
                parsedGedcomLine += '|';

                // append arguments
                parsedGedcomLine += lineElements[2];
            }
        }
        else{
            parsedGedcomLine += lineElements[2];

            // add delimiter
            parsedGedcomLine += '|';

            // is tag valid?
            ///remark A valid tag is one that is within the scope of cs555 project, refer doc "Project Overview"
            if(isTagValid(lineElements[0], lineElements[2])){
                parsedGedcomLine += "Y";
            }
            else{
                parsedGedcomLine += "N";
            }

            if(lineElements.size()>2){
                // add delimiter
                parsedGedcomLine += '|';

                // append arguments
                parsedGedcomLine += lineElements[1];
            }
        }

        // print unparsed line
        std::cout<<"\n--> "<<unparsedGedcomLine;

        // print parsed line
        std::cout<<"\n<-- "<<parsedGedcomLine;
    }

    // parse and iteratively process all lines of gedcom data
    void parseAll(){
        for(auto i=0; i<gedcomData.length(); i++){
            std::string gedcomLine="";
            while(gedcomData[i]!='\n' && gedcomData[i]!='\0' && gedcomData[i]!='\r' && i<gedcomData.length()){
                gedcomLine+=gedcomData[i++];
            }
            parseLine(gedcomLine);
        }
    }
};

int main(){
    // create gedcom object instance
    gedcom gedcomInstance;

    // read gedcom file (file path+name will be prompted for; to be provided by user)
    gedcomInstance.readGedFile();

    // print input gedcom data
    // gedcomInstance.printGedcomData();

    // parse gedcom data
    gedcomInstance.parseAll();    

    return 0;
}