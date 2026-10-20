#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include "pipe.hpp"
#include "cs_station.hpp"

class GTS {
private:
    std::unordered_map<int, Pipe> pipe_data;
    std::unordered_map<int, Cs> cs_data;
    int num_pipe = 0;
    int num_cs = 0;

public:
    
    const std::unordered_map<int, Pipe>& get_pipes() const { 
        return pipe_data; 
    }
    std::unordered_map<int, Pipe>& get_pipes_mutable() {
        return pipe_data;
    }

    const std::unordered_map<int, Cs>& get_css() const {
        return cs_data;
    }
    std::unordered_map<int, Cs>& get_css_mutable() {
        return cs_data;
    }

    bool is_empty_pipes() const {
        return pipe_data.empty();
    }
    bool is_empty_css() const {
        return cs_data.empty();
    }

    void add_pipes(const Pipe& pip) {
        pipe_data[++num_pipe] = pip;
    }

    void add_css(const Cs& css) {
        cs_data[++num_cs] = css;
    }

    void clear() {
        pipe_data.clear();
        cs_data.clear();
        num_pipe = 0;
        num_cs = 0;
    }

    void recalculate_ids() {
        num_pipe = 0;
        for (const auto& [id, pipe] : pipe_data) {
            if (id > num_pipe) num_pipe = id;
        }
        num_cs = 0;
        for (const auto& [id, cs] : cs_data) {
            if (id > num_cs) num_cs = id;
        }
    }

    void save_f(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cout << "Ошибка открытия файла!\n";
            return;
        }
        out << pipe_data.size() << "\n";
        for (const auto& [id, pipe] : pipe_data) {
            out << id << "\n";
            pipe.save_p(out);
        }
        out << cs_data.size() << "\n";
        for (const auto& [id, cs] : cs_data) {
            out << id << "\n";
            cs.save_cs(out);
        }
        std::cout << "Все данные успешно сохранены в файл: " << filename << "\n";
    }

    void load_f(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) {
            std::cout << "Файл '" << filename << "' с данными не найден!\n";
            return;
        }
        clear();

        size_t count_pipes = 0;
        if (in >> count_pipes) {
            for (size_t i = 0; i < count_pipes; ++i) {
                int id;
                in >> id;
                Pipe p;
                p.load_pipe(in);
                pipe_data[id] = p;
            }
        }

        size_t count_css = 0;
        if (in >> count_css) {
            for (size_t i = 0; i < count_css; ++i) {
                int id;
                in >> id;
                Cs cs;
                cs.load_cs(in);
                cs_data[id] = cs;
            }
        }
        recalculate_ids();
        std::cout << "Данные успешно загружены из " << filename << "!\n";
    }
};