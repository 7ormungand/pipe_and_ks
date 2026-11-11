#include "gts.hpp"
#include <iostream>
#include <fstream>

#include "utils.hpp"


void GTS::clear() {
    pipe_data.clear();
    cs_data.clear();
}

void GTS::print() const {
    std::cout << "\n    Трубы    \n";
    if (pipe_data.empty()) {
        std::cout << "Труб пока нет.\n";
    } else {
        for (const auto& [id, pipe] : pipe_data) {
            std::cout << "ID: " << id << "\n";
            pipe.print();
            std::cout << "------------------\n";
        }
    }
    std::cout << "\n    КС    \n";
    if (cs_data.empty()) {
        std::cout << "КС пока нет.\n";
    } else {
        for (const auto& [id, cs] : cs_data) {
            std::cout << "ID: " << id << "\n";
            cs.print();
            std::cout << "------------------\n";
        }
    }
}

void GTS::add_pipe_in_gts() {
    std::cout << "      Добавление трубы\n";
 
    std::cout << "Введите километровую отметку (название): ";
    std::string name;
    INPUT_LINE(std::cin, name);

    std::cout << "Введите длину (км): ";
    double length = GetCorrectNumber(1.0, 100000000000000000.0);  

    std::cout << "Введите диаметр (мм): ";
    unsigned long int d = GetCorrectNumber(1ULL, 100000000000000000ULL);

    std::cout << "Труба добавлена!\n";

    Pipe p(name, length, d, true);
    pipe_data.emplace(p.get_id(), p);
}

void GTS::add_cs_in_gts() {
    std::cout << "      Добавление КС\n";

    std::cout << "Введите название КС: ";
    std::string name;
    INPUT_LINE(std::cin, name);

    std::cout << "Введите общее количество цехов: ";
    unsigned long long int c_shop = GetCorrectNumber(1ULL, 10000000ULL);

    std::cout << "Введите количество цехов в работе: ";
    unsigned long long int c_shop_w = GetCorrectNumber(1ULL, c_shop);

    std::string input_cl;
    char cl = 'E';
    std::cout << "Введите Класс станции (A, B, C, D, E): ";
    while (true) {
        std::cin >> input_cl;
        if (input_cl.length() == 1) {
            char c = std::toupper(input_cl[0]);
            if (c >= 'A' && c <= 'E') {
                cl = c;
                break;
            }
        }
        std::cout << "Некорректный класс! Введите одну из букв (A, B, C, D, E): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    std::cerr << cl << "\n";
    std::cout << "КС добавлена!\n";
    
    Cs cs(name, c_shop, c_shop_w, cl);
    cs_data.emplace(cs.get_id(), cs);
}

void GTS::save_f(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Ошибка открытия файла!\n";
        return;
    }

    out << pipe_data.size() << "\n";
    for (const auto& [id, pipe] : pipe_data) {
        out << pipe << "\n";
    }

    out << cs_data.size() << "\n";
    for (const auto& [id, cs] : cs_data) {
        out << cs << "\n";
    }
    std::cout << "Все данные успешно сохранены в файл: " << filename << "\n";
}

void GTS::load_f(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) {
            std::cout << "Файл '" << filename << "' с данными не найден!\n";
            return;
        }
        clear();

        size_t count_pipes = 0;
        if (in >> count_pipes) {
            in.ignore(10000, '\n');
            for (size_t i = 0; i < count_pipes; ++i) {
                Pipe p;
                in >> p;
                in.ignore(10000, '\n');
                pipe_data.emplace(p.get_id(), p);
            }
        }

        size_t count_css = 0;
        if (in >> count_css) {
            in.ignore(10000, '\n');
            for (size_t i = 0; i < count_css; ++i) {
                Cs cs;
                in >> cs;
                in.ignore(10000, '\n');
                cs_data.emplace(cs.get_id(), cs);
            }
        }
        std::cout << "Данные успешно загружены из " << filename << "!\n";
    }