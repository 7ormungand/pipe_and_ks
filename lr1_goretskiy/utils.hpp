#pragma once
#include <iostream>

inline void INPUT_LINE(std::istream& in, std::string& str) {
	std::getline(in >> std::ws, str);
	std::cerr << str << std::endl;
}

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
	redirect_output_wrapper(std::ostream& src) : old_buf(src.rdbuf()), stream(src) {}

    ~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
    }
	void redirect (std::ostream& dest) {
		stream.rdbuf(dest.rdbuf());
	}
};



template <typename T>
T GetCorrectNumber(T min, T max) {
	T x;
	while ((std::cin >> x).fail()	// check type
		|| std::cin.peek() != '\n'	// is buffer empty (int/float check)
		|| x < min || x > max)		// check range
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "Неверно введите (" << min << "-" << max << "): ";
	}
	std::cerr << x << std::endl;
	std::cin.ignore(10000, '\n');
	return x;
}