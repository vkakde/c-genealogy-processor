#include "stdafx.h"
#include "Gedcom.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(US16_TC01)
{
	Gedcom::gedcom gedcomInstance("testFileError.ged");

	BOOST_TEST_MESSAGE("\nTesting US16 ...\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US16(), true);
}

// EOF