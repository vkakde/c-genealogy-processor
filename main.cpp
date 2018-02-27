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
	
	// print table of reords showing individuals' data
	//gedcomInstance.printIndividualTable();
	
	// print table of reords showing families' data
	//gedcomInstance.printFamilyTable();

	// testing US08 (birth before marriage of parents)
	gedcomInstance.US03();

	system("pause");
	//boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
	return 0;
}