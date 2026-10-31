#pragma once
#include <unordered_map>
#include <string>
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

    int get_num_pipe() {
        return ++num_pipe;
    }

    int get_num_cs() {
        return ++num_cs;
    }

    void add_pipes(Pipe& pip) {
        pipe_data[num_pipe] = pip; 
    }

    void add_css(Cs& css) {
        cs_data[num_cs] = css;
    }
    
    void clear();
    void recalculate_ids();

    void save_f(const std::string& filename) const;
    void load_f(const std::string& filename);
};