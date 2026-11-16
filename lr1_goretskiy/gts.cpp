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
    Pipe p;
    p.input();
    pipe_data.emplace(p.get_id(), p);
}

void GTS::add_cs_in_gts() {
    Cs cs;
    cs.input();
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
        if (in >> count_pipes >> std::ws) {
            for (size_t i = 0; i < count_pipes; ++i) {
                Pipe p;
                in >> p >> std::ws;
                pipe_data.emplace(p.get_id(), p);
            }
        }

        size_t count_css = 0;
        if (in >> count_css >> std::ws) {
            for (size_t i = 0; i < count_css; ++i) {
                Cs cs;
                in >> cs >> std::ws;
                cs_data.emplace(cs.get_id(), cs);
            }
        }
        std::cout << "Данные успешно загружены из " << filename << "!\n";
    }