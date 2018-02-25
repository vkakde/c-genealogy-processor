#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>
#include "main.cpp"

BOOST_AUTO_TEST_CASE(test1)
{
	gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting User Story: Birth Before Marriage -- Same day and month, birth year < marriage year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.birthBeforeMarriage("18 MAY 1992", "18 MAY 2025"), true);
}

BOOST_AUTO_TEST_CASE(test2)
{
	gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting User Story: Birth Before Marriage -- Same month, birth day < marriage day and birth year < marriage year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.birthBeforeMarriage("12 MAY 1992", "18 DEC 2025"), true);
}

BOOST_AUTO_TEST_CASE(test3)
{
	gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting User Story: Birth Before Marriage -- Same day and month, marriage year < birth year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.birthBeforeMarriage("18 MAY 2025", "18 MAY 1990"), false);
}

BOOST_AUTO_TEST_CASE(test4)
{
	gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting User Story: Birth Before Marriage -- Same month, marriage day < birth day and marriage year < birth year\n");
	BOOST_CHECK_EQUAL(gedcomInstance.birthBeforeMarriage("18 MAY 2025", "12 MAY 1990"), false);
}

BOOST_AUTO_TEST_CASE(test5)
{
	gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting User Story: Birth Before Marriage -- birth date equals marriage date\n");
	BOOST_CHECK_EQUAL(gedcomInstance.birthBeforeMarriage("18 MAY 2025", "18 MAY 2025"), false);
}

// EOF