#pragma once
#include <iostream>
#include <set>
#include <unordered_map>

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

template<typename Obj, typename T>
using Filter = bool(*)(const Obj& s, T param);

template<typename Obj, typename T>
std::set<int> FindObjByFilter(const std::unordered_map<int, Obj>& some_type, Filter<Obj, T> f, T param) {
	std::set<int> res;

	for (const auto& [id, val] : some_type) {
        if (f(val, param)) {
            res.insert(id);
        }
    }

	return res;
}

inline int choose_diameter() {
    std::cout << "Допустимые диаметры труб (мм):\n";
    std::cout << "1. 500\n";
    std::cout << "2. 700\n";
    std::cout << "3. 1000\n";
    std::cout << "4. 1400\n";
    std::cout << "Выберите диаметр: ";
    int choice = GetCorrectNumber(1, 4);
    const int diameters[] = { 500, 700, 1000, 1400 };
    return diameters[choice - 1];
}