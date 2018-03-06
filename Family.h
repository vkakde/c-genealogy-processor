#pragma once

#include <string>
#include <vector>
#include "Individual.h"

namespace Family {
class Family {
		/**** PUBLIC ATTRIBUTES	*****/
	public:
		std::string id;
		std::string husbandId;
		std::string wifeId;
		std::string marriageDate;
		std::string divorceDate;
		std::vector<std::string> childrenIds;
		std::string marriageEndDate;

		Individual::Individual husband;
		Individual::Individual wife;
		std::vector<Individual::Individual> children;

		/**** CONSTRUCTORS *****/
		Family(std::string);
	};
}