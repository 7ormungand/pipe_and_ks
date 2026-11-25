#include "gts.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>

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

void GTS::edit_pipes(const std::set<int>& ids, bool in_work) {
    for (int id : ids) {
        if (pipe_data.count(id) > 0) {
            pipe_data.at(id).edit(in_work);
        }
    }
}

void GTS::edit_css(const std::set<int>& ids, int action) {
    for (int id : ids) {
        if (cs_data.count(id) > 0) {
            cs_data.at(id).edit(action);
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

bool GTS::connect(int pipe_id, int Cs_out_id, int Cs_in_id) {
    if (!pipe_data.count(pipe_id)) {
        return false;
    }
    if (!cs_data.count(Cs_out_id) || !cs_data.count(Cs_in_id)) {
        return false;
    }
    if (Cs_out_id == Cs_in_id) {
        return false;
    }
    if (pipe_data.at(pipe_id).is_connected()) {
        return false;
    }

    pipe_data.at(pipe_id).connect(Cs_out_id, Cs_in_id);
    return true;
}

bool GTS::disconnect(int pipe_id) {
    if (!pipe_data.count(pipe_id)) return false;
    pipe_data.at(pipe_id).disconnect();
    return true;
}

int GTS::find_free_pipe(int diameter) const {
    int best_id = -1;
    for (const auto& [id, pipe] : pipe_data) {
        if (!pipe.is_connected() && !pipe.is_in_repair() && pipe.get_d() == diameter) {
            if (best_id == -1 || id < best_id) {
                best_id = id;
            }
        }
    }
    return best_id;
}

int GTS::add_pipe_interactive(int required_diameter) {
    Pipe pipe;
    pipe.input(required_diameter);
    int id = pipe.get_id();
    pipe_data.emplace(id, pipe);
    return id;
}

size_t GTS::remove_css(const std::set<int>& ids) {
    size_t removed = remove_impl(cs_data, ids);
    if (removed > 0) {
        for (auto& [id, pipe] : pipe_data) {
            if (pipe.is_connected() && (!cs_data.count(pipe.get_Cs_out_id()) || !cs_data.count(pipe.get_Cs_in_id()))) {
                pipe.disconnect();
            }
        }
    }
    return removed;
}

void GTS::dfs(int v,
              const std::map<int, std::set<int>>& adj,
              std::map<int, int>& color,
              std::vector<int>& order,
              std::vector<int>& path,
              bool& cycle_found,
              int& cycle_start,
              int& cycle_end) const {
    color[v] = GRAY;
    path.push_back(v);

    if (adj.count(v) > 0) {
        for (int w : adj.at(v)) {
            if (cycle_found) {
                return;
            }
            if (color[w] == GRAY) {
                cycle_found = true;
                cycle_start = w;
                cycle_end = v;
                return;
            }
            if (color[w] == WHITE) {
                dfs(w, adj, color, order, path, cycle_found, cycle_start, cycle_end);
                if (cycle_found) {
                    return;
                }
            }
        }
    }

    path.pop_back();
    color[v] = BLACK;
    order.push_back(v);
}

void GTS::topologic_sort() const {
    std::cout << "\n    Топологическая сортировка ГТС\n\n";

    std::map<int, std::set<int>> adj;
    std::set<int> vertices;

    for (const auto& [id, pipe] : pipe_data) {
        if (!pipe.is_connected() || pipe.is_in_repair()) {
            continue;
        }
        int out = pipe.get_Cs_out_id();
        int in = pipe.get_Cs_in_id();
        if (cs_data.count(out) == 0 || cs_data.count(in) == 0) {
            continue;
        }
        adj[out].insert(in);
        vertices.insert(out);
        vertices.insert(in);
    }

    if (vertices.empty()) {
        std::cout << "Граф пуст: нет соединённых труб в работе.\n";
        return;
    }

    std::cout << "Рёбра графа (КС выхода -> КС входа):\n";
    std::set<int> edge_pipes;
    for (const auto& [id, pipe] : pipe_data) {
        if (pipe.is_connected() && !pipe.is_in_repair()
            && cs_data.count(pipe.get_Cs_out_id()) > 0
            && cs_data.count(pipe.get_Cs_in_id()) > 0) {
            edge_pipes.insert(id);
        }
    }
    for (int id : edge_pipes) {
        const Pipe& pipe = pipe_data.at(id);
        std::cout << "  Труба " << id << " (d = " << pipe.get_d()
                  << " мм): КС " << pipe.get_Cs_out_id()
                  << " -> КС " << pipe.get_Cs_in_id() << "\n";
    }
    std::cout << "\n";

    std::map<int, int> color;
    for (int v : vertices) {
        color[v] = WHITE;
    }

    std::vector<int> order;
    std::vector<int> path;
    bool cycle_found = false;
    int cycle_start = -1;
    int cycle_end = -1;

    for (int v : vertices) {
        if (!cycle_found && color[v] == WHITE) {
            dfs(v, adj, color, order, path, cycle_found, cycle_start, cycle_end);
        }
    }

    if (cycle_found) {
        size_t start = path.size();
        for (size_t j = 0; j < path.size(); ++j) {
            if (path[j] == cycle_start) {
                start = j;
                break;
            }
        }
        
        std::cout << "Обнаружен цикл: ";
        for (size_t j = start; j < path.size() && path[j] != cycle_end; ++j) {
            std::cout << path[j] << " -> ";
        }
        std::cout << cycle_end << " -> " << cycle_start << "\n";
        std::cout << "Топологическая сортировка невозможна.\n";
        return;
    }

    std::reverse(order.begin(), order.end());
    std::cout << "Топологический порядок КС: ";
    for (size_t i = 0; i < order.size(); ++i) {
        std::cout << order[i];
        if (i + 1 < order.size()) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";
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