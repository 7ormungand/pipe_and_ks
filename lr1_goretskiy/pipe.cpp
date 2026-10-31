#include "pipe.hpp"


Pipe::Pipe(std::string new_name, int new_id, double new_l, int new_d, bool new_status)  : name(new_name), id(new_id), length(new_l), d(new_d), work(new_status) {}

void Pipe::print() const {
    if (d == 0) {
        return;
    }
    
    std::cout << "Название (км отметка): " << (name.empty() ? "(пусто)" : name) << "\n";
    std::cout << "Длина: " << length << "\n";
    std::cout << "Диаметр: " << d << "\n";
    std::cout << "Статус: " << (work ? "В работе" : "В ремонте" ) << "\n";
}

void Pipe::edit(int choice) {
    if (choice == 1) {
        work = true;
    } else if (choice == 0) {
        work = false;
    }
}

std::ostream& operator<<(std::ostream& out, const Pipe& p) {
    out << p.name << "\n" << p.id << "\n" << p.length << "\n" << p.d << "\n" << p.work;
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& p) {
    std::getline(in, p.name);
    in >> p.id >> p.length >> p.d >> p.work;
    return in;
}