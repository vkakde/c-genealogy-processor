#pragma once

#include <string>
#include <vector>

namespace Family {
class Family {
		/**** PUBLIC ATTRIBUTES	*****/
	public:
		std::string id;
		std::string husb;
		std::string wife;
		std::string marrDate;
		std::string divorceDate;
		std::vector<std::string> children;

		/**** CONSTRUCTORS *****/
		Family(std::string);
	};
}