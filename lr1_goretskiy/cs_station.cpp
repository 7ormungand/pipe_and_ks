#include "cs_station.hpp"


Cs::Cs(std::string new_name, int new_id, int new_c_shop, int new_c_shop_w, char new_cl) : name(new_name), id(new_id), c_shop(new_c_shop), c_shop_w(new_c_shop_w), cl(new_cl) {}

void Cs::print() const {
    if (c_shop == 0) {
        return;
    }

    std::cout << "Название: " << (name.empty() ? "(пусто)" : name) << "\n";
    std::cout << "Всего цехов: " << c_shop << "\n";
    std::cout << "Цехов в работе: " << c_shop_w << "\n";
    std::cout << "Класс станции: " << cl << "\n";
    std::cout << "Процент неработающих цехов: " << get_percentage() << "%\n";
}

void Cs::edit(int choice) {
    if (choice == 1) {
        if (c_shop_w < c_shop) {
            c_shop_w++;
            std::cout << "Цех запущен. Цехов в работе: " << c_shop_w << "\n";
        } else {
            std::cout << "Все цеха уже работают!\n";
        }
    } else if (choice == 2) {
        if (c_shop_w > 0) {
            c_shop_w--;
            std::cout << "Цех остановлен. Цехов в работе: " << c_shop_w << "\n";
        } else {
            std::cout << "Все цеха уже остановлены!\n";
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Cs& cs) {
    out << cs. name << "\n" << cs.id << "\n" << cs.c_shop << "\n" << cs.c_shop_w << "\n" << cs.cl;
    return out;
}

std::istream& operator>>(std::istream& in, Cs& cs) {
    std::getline(in, cs.name);
    in >> cs.id >> cs.c_shop >> cs.c_shop_w >> cs.cl;
    return in;
}