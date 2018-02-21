#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include <iomanip>
#include <cctype>

#include "Individual.h"
#include "Family.h"

#include <boost\date_time\gregorian\gregorian.hpp>
#include <boost\thread\thread.hpp>

namespace Gedcom {
	class gedcom {
		/*******************************************************/
		/****************** MEMBER ATTRIBUTES ******************/
		/*******************************************************/
		std::map<std::string, int> supportedTagsAndLevels;
		std::string gedcomData;
		std::vector<Family::Family> familyList = {};
		std::vector<Individual::Individual> individualList = {};
		std::string currentId;
		bool isInd;
		bool deathDate;
		bool birthDate;
		bool marrDate;
		bool divDate;

		/*******************************************************/
		/******************** CONSTRUCTORS *********************/
		/*******************************************************/
	public:
		gedcom();

		/*******************************************************/
		/************* PUBLIC FUNCTIONS - GENERAL **************/
		/*******************************************************/
		/* Read .ged file */
		void readGedFile();									
		
		/* Print as read from .ged file */
		void printGedcomData();
		
		/* Print tags supported in "Project Overview" */
		void printSupportedTags();
		
		/* Identifies a tag as "INDI" or FAM" */
		std::string isIndiOrFam(std::vector<std::string>);
		
		/* parse and process a single line of gedcom data */
		void parseLine(std::string);
		
		/* parse and iteratively process all lines of gedcom data */
		void parseAll();
		
		/* set attribute value */
		void addAttribute(std::string, std::string);
		
		/*******************************************************/
		/************** PUBLIC FUNCTIONS - PRINT ***************/
		/*******************************************************/
		/* print individual data */
		void printIndividualTable();
		
		/* print family data */
		void printFamilyTable();

		///\brief Include individual ages
		/** Include person's current age when listing individuals **/
		void US27();

		///\brief Order siblings by age
		/** Order siblings by age **/
		void US28();

		///\brief List deceased
		/** List all deceased individuals in a GEDCOM file **/
		void US29();

		///\brief List living married
		/** List all living married people in a GEDCOM file **/
		void US30();

		///\brief List living single
		/** List all living people over 30 who have never been married in a GEDCOM file **/
		void US31();

		///\brief List multiple births
		/** List all multiple births in a GEDCOM file **/
		void US32();

		///\brief List orphans
		/** List all orphaned children (both parents dead and child < 18 years old) in a GEDCOM file **/
		void US33();

		///\brief List large age differences
		/** List all couples who were married when the older spouse was more than twice as old as the younger spouse **/
		void US34();

		///\brief List recent births
		/** List all people in a GEDCOM file who were born in the last 30 days **/
		void US35();

		///\brief List recent deaths
		/** List all people in a GEDCOM file who died in the last 30 days **/
		void US36();

		///\brief List recent survivors
		/** List all living spouses and descendants of people in a GEDCOM file who died in the last 30 days **/
		void US37();

		///\brief List upcoming birthdays
		/** List all living people in a GEDCOM file whose birthdays occur in the next 30 days **/
		void US38();

		///\brief List upcoming anniversaries
		/** List all living couples in a GEDCOM file whose marriage anniversaries occur in the next 30 days **/
		void US39();

		///\brief Include input line numbers
		/** List line numbers from GEDCOM source file when reporting errors **/
		void US40();

		/*******************************************************/
		/************ PUBLIC FUNCTIONS - VALIDATION ************/
		/*******************************************************/
		/* Is tag supported? i.e. part of "Project Overview"? */
		bool isTagValid(std::string, std::string, int);

		///\brief Dates before current date
		/** Dates (birth, marriage, divorce, death) should not be after the current date **/
		bool US01();		 

		///\brief Birth before marriage
		/** Birth should occur before marriage of an individual **/
		bool US02();

		///\brief Birth before death
		/** Birth should occur before death of an individual **/
		bool US03();

		///\brief Marriage before divorce
		/** Marriage should occur before divorce of spouses, and divorce can only occur after marriage **/
		bool US04();

		///\brief Marriage before death
		/** Marriage should occur before death of either spouse **/
		bool US05();

		///\brief Divorce before death
		/** Divorce can only occur before death of both spouses **/
		bool US06();

		///\brief Less then 150 years old
		/** Death should be less than 150 years after birth for dead people, and current date should be less than 150 years after birth for all living people **/
		bool US07();

		///\brief Birth before marriage of parents
		/** Children should be born after marriage of parents (and not more than 9 months after their divorce) **/
		bool US08();

		///\brief Birth before death of parents
		/** Child should be born before death of mother and before 9 months after death of father **/
		bool US09();

		///\brief Marriage after 14
		/** Marriage should be at least 14 years after birth of both spouses (parents must be at least 14 years old) **/
		bool US10();

		///\brief No bigamy
		/** Marriage should not occur during marriage to another spouse **/
		bool US11();

		///\brief Parents not too old
		/** Mother should be less than 60 years older than her children and father should be less than 80 years older than his children **/
		bool US12();

		///\brief Siblings spacing
		/** Birth dates of siblings should be more than 8 months apart or less than 2 days apart (twins may be born one day apart, e.g. 11:59 PM and 12:02 AM the following calendar day) **/
		bool US13();

		///\brief Multiple births <= 5
		/** No more than five siblings should be born at the same time **/
		bool US14();

		///\brief Fewer than 15 siblings
		/** There should be fewer than 15 siblings in a family **/
		bool US15();

		///\brief Male last names
		/** All male members of a family should have the same last name **/
		bool US16();

		///\brief No marriages to descendants
		/** Parents should not marry any of their descendants **/
		bool US17();

		///\brief Siblings should not marry
		/** Siblings should not marry one another **/
		bool US18();

		///\brief First cousins should not marry
		/** First cousins should not marry one another **/
		bool US19();

		///\brief Aunts and uncles
		/** Aunts and uncles should not marry their nieces or nephews **/
		bool US20();

		///\brief Correct gender for role
		/** Husband in family should be male and wife in family should be female **/
		bool US21();

		///\brief Unique IDs
		/** All individual IDs should be unique and all family IDs should be unique **/
		bool US22();

		///\brief Unique name and birth date
		/** No more than one individual with the same name and birth date should appear in a GEDCOM file **/
		bool US23();

		///\brief Unique families by spouses
		/** No more than one family with the same spouses by name and the same marriage date should appear in a GEDCOM file **/
		bool US24();

		///\brief Unique first names in families
		/** No more than one child with the same name and birth date should appear in a family **/
		bool US25();

		///\brief Corresponding entries
		/** All family roles (spouse, child) specified in an individual record should have corresponding entries */
		/*  in the corresponding family records. Likewise, all individual roles (spouse, child) specified in */
		/*  family records should have corresponding entries in the corresponding  individual's records.  */
		/*  I.e. the information in the individual and family records should be consistent. **/
		bool US26();

		///\brief Include partial dates
		/** Accept and use dates without days or without days and months **/
		bool US41();

		///\brief Reject illegitimate dates
		/** All dates should be legitimate dates for the months specified (e.g., 2/30/2015 is not legitimate) **/
		bool US42();							
	};
}