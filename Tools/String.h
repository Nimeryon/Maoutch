#pragma once
#include <string>
#include <vector>

namespace maoutch
{
	namespace string
	{
		template<typename ... Args>
		std::string Format(const std::string& format, Args ...args);

		std::vector<std::string> Split(const std::string& string, const char& separator);

		std::string Replace(std::string subject, const std::string& search, const std::string& replace);

		std::string ToLower(const std::string& string);
		std::string ToUpper(const std::string& string);

		std::string ToString(const float& value);
	}
}
