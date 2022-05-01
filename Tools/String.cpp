#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <sstream>

#include "String.h"

namespace maoutch
{
	namespace string
	{
		template <typename ... Args>
		std::string Format(const std::string& format, Args ... args)
		{
			size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
			if (size <= 0) { throw std::runtime_error("Error during formatting."); }
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1);
		}

		std::vector<std::string> Split(const std::string& string, const char& separator)
		{
			std::vector<std::string> words = {};

			std::string word;
			for (char character : string)
			{
				if (character == separator)
				{
					words.push_back(word);
					word = "";
					continue;;
				}
				word += character;
			}
			if (word != "") words.push_back(word);

			return words;
		}

		std::string Replace(std::string subject, const std::string& search, const std::string& replace)
		{
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
		}

		std::string ToLower(const std::string& string)
		{
			std::string res = string;
			std::transform(res.begin(), res.end(), res.begin(), std::tolower);

			return res;
		}
		std::string ToUpper(const std::string& string)
		{
			std::string res = string;
			std::transform(res.begin(), res.end(), res.begin(), std::toupper);

			return res;
		}

		std::string ToString(const float& value, const int& precision)
		{
			std::stringstream sstream;
			sstream << value;
			return sstream.str();
		}
	}
}
