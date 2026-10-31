#include "gts.hpp"
#include <iostream>
#include <fstream>


void GTS::clear() {
    pipe_data.clear();
    cs_data.clear();
    num_pipe = 0;
    num_cs = 0;
}

void GTS::recalculate_ids() {
    num_pipe = 0;
    for (const auto& [id, pipe] : pipe_data) {
        if (id > num_pipe) num_pipe = id;
    }
    num_cs = 0;
    for (const auto& [id, cs] : cs_data) {
        if (id > num_cs) num_cs = id;
    }
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
            in.ignore();
            for (size_t i = 0; i < count_pipes; ++i) {
                Pipe p;
                in >> p;
                in.ignore();
                pipe_data[p.get_id()] = p;
            }
        }

        size_t count_css = 0;
        if (in >> count_css) {
            in.ignore();
            for (size_t i = 0; i < count_css; ++i) {
                Cs cs;
                in >> cs;
                in.ignore();
                cs_data[cs.get_id()] = cs;
            }
        }
        recalculate_ids();
        std::cout << "Данные успешно загружены из " << filename << "!\n";
    }