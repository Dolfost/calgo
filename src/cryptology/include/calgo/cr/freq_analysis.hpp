#ifndef _CALGO_CRYPTOLOGY_FREQ_ANALYSIS_HPP_
#define _CALGO_CRYPTOLOGY_FREQ_ANALYSIS_HPP_

#include <calgo/calgo.hpp>

#include <unordered_map>
#include <filesystem>
#include <cstddef>
#include <iosfwd>

namespace ca::cr {

/**
 * @brief Character frequency
 *
 * Contains pairs of `char`-`double`
 */
class freq: public std::unordered_map<char, double> {
public:
	using size_type = std::size_t;
	using char_type = char;
	using prob_type = double;
public:
	using std::unordered_map<char_type, prob_type>::unordered_map;
	freq(const std::filesystem::path& p) {
		fill(p);
	};
	freq(const std::string& s) {
		fill(s);
	};

	/**
	 * @brief Build frequency field from file
	 *
	 * @param p input file path
	 */
	void fill(const std::filesystem::path& p);
	/**
	 * @brief Build frequency field from string
	 *
	 * @param s input string
	 */
	void fill(const std::string& s);

	friend std::ostream& operator<<(std::ostream& os, const freq& f);
};

namespace freqs {

static const freq en { // data from war and peace
	{'$', 6.2695747880335136e-07},
	{'%', 3.1347873940167568e-07},
	{'=', 6.2695747880335136e-07},
	{'9', 9.7178409214519455e-06},
	{'q', 0.00073009198406650264},
	{'z', 0.00071159673844180379},
	{')', 0.00020470161682929422},
	{'(', 0.00020720944674450763},
	{'!', 0.0012131627214844848},
	{'x', 0.0013742907935369462},
	{'/', 2.8213086546150809e-06},
	{'*', 8.2131429723239025e-05},
	{'#', 3.1347873940167568e-07},
	{'0', 5.3291385698284863e-05},
	{'1', 0.00012257018710605519},
	{'-', 0.0005526630175651542},
	{'[', 3.1347873940167568e-07},
	{',', 0.012460466412477207},
	{']', 3.1347873940167568e-07},
	{'.', 0.0095864933296426428},
	{'v', 0.0082670613155009908},
	{'l', 0.030129382080113253},
	{'m', 0.019246967641784084},
	{'s', 0.050707066970657452},
	{':', 0.00031473265435928238},
	{'i', 0.05383088260879515},
	{'5', 1.5673936970083783e-05},
	{'d', 0.036969174695118415},
	{'2', 4.5140938473841294e-05},
	{'a', 0.063270040930919008},
	{'w', 0.018540386563172705},
	{'h', 0.0523437394690736},
	{'k', 0.006352333175235556},
	{'\n', 0.024033474513708269},
	{'7', 1.1912192097263676e-05},
	{'f', 0.017200578430969945},
	{'y', 0.014427858980962122},
	{'3', 1.8495245624698866e-05},
	{'b', 0.010824420871539861},
	{'?', 0.00096457408113895607},
	{'n', 0.057444665516617664},
	{'4', 7.2100110062385405e-06},
	{'c', 0.019305901644791599},
	{'u', 0.020126902463284586},
	{';', 0.00035736576291791027},
	{'j', 0.00080658079648051153},
	{'o', 0.059404534595356939},
	{'8', 5.9874439225720054e-05},
	{'g', 0.01606641235181468},
	{'r', 0.046458176136807139},
	{' ', 0.16203245821563514},
	{'6', 1.6614373188288811e-05},
	{'e', 0.098142356338179609},
	{'p', 0.014232248247575477},
	{'t', 0.070711085768096579},
};

}

}

#endif // !_CALGO_CRYPTOLOGY_FREQ_ANALYSIS_HPP_
