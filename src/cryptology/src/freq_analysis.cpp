#include <calgo/cr/freq_analysis.hpp> 

#include <fstream> 
#include <ios> 
#include <limits>
#include <cctype>

namespace ca::cr {

void freq::fill(const std::filesystem::path& p) {
	clear();
	std::ifstream f(p, std::ios::in);
	f.seekg(0, std::ifstream::seekdir::end);
	size_type count = f.tellg();
	f.seekg(0, std::ifstream::seekdir::beg);

	while (not f.eof()) 
		operator[](std::tolower(f.get()))++;
	f.close();

	for (auto& [c, f]: *this) {
		f /= count;
	}
}

void freq::fill(const std::string& s) {
	clear();
	size_type count = s.length();

	for (std::string::size_type i = 0; i < s.size(); i++)
		operator[](std::tolower(s[i]))++;

	for (auto& [c, f]: *this) {
		f /= count;
	}
}

std::ostream& operator<<(std::ostream& os, const freq& f) {
	for (const auto& [c, f]: f) {
		os << "{'";
		switch (c) {
			case '\n': {
				os << "\\n";
				break;
			}
			case '\t': {
				os << "\\t";
				break;
			}
			case '\r': {
				os << "\\r";
				break;
			}
			default: {
				os << c;
			}
		}
		os << "', " 
			<< std::setprecision(std::numeric_limits<double>::max_digits10)
			<< f << "},\n";
	}
	return os;
}

}
