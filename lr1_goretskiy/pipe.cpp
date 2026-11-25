#include "pipe.hpp"

#include "utils.hpp"

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
    std::cout << "Соединение: ";
    if (is_connected()) {
        std::cout << "КС " << Cs_out_id << " -> КС " << Cs_in_id << "\n";
    } else {
        std::cout << "не подключена\n";
    }
}

void Pipe::input(int required_diameter) {
    std::cout << "      Добавление трубы\n";

    std::cout << "Введите километровую отметку (название): ";
    INPUT_LINE(std::cin, name);

    std::cout << "Введите длину (км): ";
    length = GetCorrectNumber(1.0, 100000000000.0);

    while (true) {
        std::cout << "Введите диаметр (мм):\n";
        d = choose_diameter();
        if (required_diameter == 0 || d == required_diameter) {
            break;
        }
        std::cout << "Для соединения нужна труба диаметром " << required_diameter
                  << " мм. Выберите этот диаметр.\n";
    }

    work = true;
    id = next_id++;

    std::cout << "Труба добавлена!\n";
}

void Pipe::edit(bool choice) {
    work = choice;
}

std::ofstream& operator<<(std::ofstream& out, const Pipe& p) {
    out << p.name << "\n" << p.id << "\n" << p.length << "\n" << p.d << "\n" << p.work << "\n" 
        << p.Cs_out_id << "\n" << p.Cs_in_id;
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Pipe& p) {
    std::getline(in, p.name);
    in >> p.id >> p.length >> p.d >> p.work >> p.Cs_out_id >> p.Cs_in_id;
    p.next_id = p.next_id > p.id + 1 ? p.next_id : p.id + 1;
    return in;
}