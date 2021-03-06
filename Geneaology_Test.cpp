#include "stdafx.h"
#include "Gedcom.h"
//#include "../Project/Gedcom.h"

//
#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>

// Testing for US01

// Testing for US02

BOOST_AUTO_TEST_CASE(test1)
{
	Gedcom::gedcom gedcomInstance;
	gedcomInstance.readGedFile();

	BOOST_TEST_MESSAGE("\nTesting US02: Birth Before Marriage -- Same day and month, birth year < marriage year\n");
	std::cout << "\n Test 1 output: " << gedcomInstance.US07();
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

// Testing for US03

BOOST_AUTO_TEST_CASE(test6)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US03: Birth Before Death -- birth date before death date\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US03(), true); // ("17 MAY 2025", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test7)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US03: Birth Before Death -- birth date on death date\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US03(), true); // ("18 MAY 2025", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test8)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US03: Birth Before Death -- birth date after death date\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US03(), false); // ("19 MAY 2025", "18 MAY 2025")
}

// Testing for US04

// Testing for US05

// Testing for US06

// Testing for US07

BOOST_AUTO_TEST_CASE(test9)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US07: Less than 150 years old -- individual is more than 150 years old\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US07(), false); // ("18 MAY 2025", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test10)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US07: Less than 150 years old -- individual is less than 150 years old\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US07(), true); // ("18 MAY 2025", "18 MAY 2025")
}

// Testing for US08

// Testing for US09

// Testing for US10

BOOST_AUTO_TEST_CASE(test11)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US10: Marriage After 14 -- Inidividual is younger than 14 years old\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US10(), false); // ("18 MAY 2025", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test12)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US10: Marriage After 14 -- Inidividual is at last than 14 years old\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US10(), true); // ("18 MAY 2025", "18 MAY 2025")
}

// Testing for US11

// Testing for US12

// Testing for US13

// Testing for US14

BOOST_AUTO_TEST_CASE(test13)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US14: Multiple births <= 5 -- Family has <= 5 multiples\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US14(), false); // ("18 MAY 2025", "18 MAY 2025")
}

BOOST_AUTO_TEST_CASE(test14)
{
	Gedcom::gedcom gedcomInstance;

	BOOST_TEST_MESSAGE("\nTesting US14: Multiple births <= 5 -- Family has > 5 multiples\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US14(), false); // ("18 MAY 2025", "18 MAY 2025")
}

// Testing for US15

// Testing for US16



// EOF