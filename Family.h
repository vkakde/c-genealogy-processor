#pragma once

#include <string>
#include <vector>

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

		/**** CONSTRUCTORS *****/
		Family(std::string);
	};
}