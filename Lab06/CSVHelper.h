#pragma once
#include <string>
#include <vector>

namespace CSVHelper
{
	inline std::vector<std::string> Split(const std::string& str, char delim = ',')
	{
		std::vector<std::string> retVal;

		size_t start = 0;
		size_t delimLoc = str.find_first_of(delim, start);
		while (delimLoc != std::string::npos)
		{
			retVal.emplace_back(str.substr(start, delimLoc - start));

			start = delimLoc + 1;
			delimLoc = str.find_first_of(delim, start);
		}

		retVal.emplace_back(str.substr(start));
		return retVal;
	}
} // namespace CSVHelper
