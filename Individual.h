#pragma once

#include <string>

namespace Individual {
	class Individual {
		/**** PUBLIC ATTRIBUTES	*****/
	public:
		///\remark all attributes public for now, getters/setters may be added later
		std::string id;
		std::string name;
		std::string sex;
		std::string birthDay;
		std::string deathDate;
		std::string famcId;
		std::string famsId;

		/**** CONSTRUCTORS *****/
		Individual(std::string);
		Individual();
	};
}