#pragma once
#include <iostream>

inline void clear_input() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}