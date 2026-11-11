#include "pipe.hpp"

int Pipe::next_id = 1;

Pipe::Pipe(std::string new_name, double new_l, int new_d, bool new_status) : 
name(new_name), length(new_l), d(new_d), work(new_status) {
    id = next_id++;
}

void Pipe::print() const {
    std::cout << "Название (км отметка): " << name << "\n";
    std::cout << "Длина: " << length << "\n";
    std::cout << "Диаметр: " << d << "\n";
    std::cout << "Статус: " << (work ? "В работе" : "В ремонте" ) << "\n";
}

void Pipe::edit(bool choice) {
    work = choice;
}

std::ostream& operator<<(std::ostream& out, const Pipe& p) {
    out << p.name << "\n" << p.id << "\n" << p.length << "\n" << p.d << "\n" << p.work;
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& p) {
    std::getline(in, p.name);
    in >> p.id >> p.length >> p.d >> p.work;
    p.next_id = p.next_id > p.id + 1 ? p.next_id : p.id + 1;
    return in;
}