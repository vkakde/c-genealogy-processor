///brief This program parses and processes genealogy files in the GEDCOM format
///authors Matthew Josephs, Louis Rozencwajg-Hays, Gabrielle Mccormack, Vishwajeet Kakde
// Link to github: https://github.com/vkakde/sw555MLGV

// include standard headers
#include <time.h>

// include project header files
#include "Gedcom.h"

int main() {
//	// create gedcom object instance
	Gedcom::gedcom gedcomInstance;
//
	// read gedcom file (file path+name will be prompted for; to be provided by user)
	gedcomInstance.readGedFile();
	
	// print input gedcom data
	// gedcomInstance.printGedcomData();
	
	// parse gedcom data
	gedcomInstance.parseAll();

	//Adds individual objects to family objects
	gedcomInstance.addIndividualsToFamilies();
	
	// print table of reords showing individuals' data
	gedcomInstance.printIndividualTable();
	
	// print table of reords showing families' data
	gedcomInstance.printFamilyTable();

	gedcomInstance.US28();
	gedcomInstance.US29();
	gedcomInstance.US30();
	gedcomInstance.US31();
	gedcomInstance.US32();


	// testing all US
	bool check01 = gedcomInstance.US01();
	bool check02 = gedcomInstance.US02();
	bool check03 = gedcomInstance.US03();
	bool check04 = gedcomInstance.US04();
	bool check05 = gedcomInstance.US05();
	bool check06 = gedcomInstance.US06();
	bool check07 = gedcomInstance.US07();
	bool check08 = gedcomInstance.US08();
	bool check09 = gedcomInstance.US09();
	bool check10 = gedcomInstance.US10();
	bool check11 = gedcomInstance.US11();
	bool check12 = gedcomInstance.US12();
	bool check13 = gedcomInstance.US13();
	bool check14 = gedcomInstance.US14();
	bool check15 = gedcomInstance.US15();
	bool check16 = gedcomInstance.US16();
	bool check17 = gedcomInstance.US17();
	bool check18 = gedcomInstance.US18();
	bool check19 = gedcomInstance.US19();
	bool check20 = gedcomInstance.US20();
	bool check21 = gedcomInstance.US21();
	bool check22 = gedcomInstance.US22();
	bool check23 = gedcomInstance.US23();
	bool check24 = gedcomInstance.US24();
	bool check25 = gedcomInstance.US25();
	bool check26 = gedcomInstance.US26();
	if (!check01 || !check02 || !check03 || !check04 ||
		!check05 || !check06 || !check07 || !check08 ||
		!check09 || !check10 || !check11 || !check12 || 
		!check13 || !check14 || !check15 || !check16 ||
		!check17 || !check18 || !check19 || !check20 ||
		!check21 || !check22 || !check23 || !check24 ||
		!check25 || !check26) {
		std::cout << "Some errors found in input file. Please check error messages for details." << std::endl;
	} else {
		std::cout << "All tests passed." << std::endl;
	}

	system("pause");
	//boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
	return 0;
}