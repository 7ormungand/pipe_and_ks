#include "cs_station.hpp"

#include "utils.hpp"

int Cs::next_id = 1;

Cs::Cs(std::string new_name, int new_c_shop, int new_c_shop_w, char new_cl) : 
name(new_name), c_shop(new_c_shop), c_shop_w(new_c_shop_w), cl(new_cl) {
    id = next_id++;
}

void Cs::print() const {
    std::cout << "Название: " << name << "\n";
    std::cout << "Всего цехов: " << c_shop << "\n";
    std::cout << "Цехов в работе: " << c_shop_w << "\n";
    std::cout << "Класс станции: " << cl << "\n";
    std::cout << "Процент неработающих цехов: " << get_percentage() << "%\n";
}

void Cs::input() {
    std::cout << "      Добавление КС\n";

    std::cout << "Введите название КС: ";
    INPUT_LINE(std::cin, name);

    std::cout << "Введите общее количество цехов: ";
    c_shop = GetCorrectNumber(1, 100);

    std::cout << "Введите количество цехов в работе: ";
    c_shop_w = GetCorrectNumber(1, c_shop);

    std::cout << "Введите Класс станции (A, B, C, D, E): ";
    cl = GetCorrectNumber('A', 'E');

    id = next_id++;

    std::cout << "КС добавлена!\n";
}

void Cs::edit(int choice) {
    if (choice == 1 && c_shop_w < c_shop) {
        c_shop_w++;
    } else if (choice == 2 && c_shop_w > 0) {
        c_shop_w--;
    }
}

std::ofstream& operator<<(std::ofstream& out, const Cs& cs) {
    out << cs. name << "\n" << cs.id << "\n" << cs.c_shop << "\n" << cs.c_shop_w << "\n" << cs.cl;
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Cs& cs) {
    std::getline(in, cs.name);
    in >> cs.id >> cs.c_shop >> cs.c_shop_w >> cs.cl;
    cs.next_id = cs.next_id > cs.id + 1 ? cs.next_id : cs.id + 1;
    return in;
}