#include "stdafx.h"
#include "Gedcom.h"

#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
	Gedcom::gedcom gedcomInstance;
	gedcomInstance.readGedFile();

	BOOST_TEST_MESSAGE("\nTesting US02: Birth Before Marriage -- Same day and month, birth year < marriage year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US07(), true);		//("18 MAY 1992", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test2)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US02: Birth Before Marriage -- Same month, birth day < marriage day and birth year < marriage year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US02(), true);		//("12 MAY 1992", "18 DEC 2025")
}

BOOST_AUTO_TEST_CASE(test3)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US02: Birth Before Marriage -- Same day and month, marriage year < birth year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US02(), true); // ("18 MAY 2025", "18 MAY 1990")
}

BOOST_AUTO_TEST_CASE(test4)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US02: Before Marriage -- Same month, marriage day < birth day and marriage year < birth year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US02(), true);  // ("18 MAY 2025", "12 MAY 1990")
}

BOOST_AUTO_TEST_CASE(test5)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US02: Birth Before Marriage -- birth date equals marriage date\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US02(), true); // ("18 MAY 2025", "18 MAY 2025")
}

// EOF