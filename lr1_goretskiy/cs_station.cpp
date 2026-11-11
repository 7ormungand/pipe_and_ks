#include "cs_station.hpp"

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

void Cs::edit(int choice) {
    if (choice == 1 && c_shop_w < c_shop) {
        c_shop_w++;
    } else if (choice == 2 && c_shop_w > 0) {
        c_shop_w--;
    }
}

std::ostream& operator<<(std::ostream& out, const Cs& cs) {
    out << cs. name << "\n" << cs.id << "\n" << cs.c_shop << "\n" << cs.c_shop_w << "\n" << cs.cl;
    return out;
}

std::istream& operator>>(std::istream& in, Cs& cs) {
    std::getline(in, cs.name);
    in >> cs.id >> cs.c_shop >> cs.c_shop_w >> cs.cl;
    cs.next_id = cs.next_id > cs.id + 1 ? cs.next_id : cs.id + 1;
    return in;
}