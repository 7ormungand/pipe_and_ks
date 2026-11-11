#pragma once
#include <unordered_map>
#include <string>
#include "pipe.hpp"
#include "cs_station.hpp"

class GTS {
private:
    std::unordered_map<int, Pipe> pipe_data;
    std::unordered_map<int, Cs> cs_data;
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

    void add_pipe_in_gts();
    void add_cs_in_gts();

    void clear();

    void print() const;
    void save_f(const std::string& filename) const;
    void load_f(const std::string& filename);
};