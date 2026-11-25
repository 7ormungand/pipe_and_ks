#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <set>
#include "pipe.hpp"
#include "cs_station.hpp"

class GTS {
private:
    std::unordered_map<int, Pipe> pipe_data;
    std::unordered_map<int, Cs> cs_data;

    enum Color {
        WHITE,
        GRAY,
        BLACK
    };

    void dfs(int v,
            const std::map<int, std::set<int>>& adj,
            std::map<int, int>& color,
            std::vector<int>& order,
            std::vector<int>& path,
            bool& cycle_found,
            int& cycle_start,
            int& cycle_end) const;
            
    template<typename T>
    size_t remove_impl(std::unordered_map<int, T>& data, const std::set<int>& ids) {
        size_t removed = 0;
        for (int id : ids) {
            removed += data.erase(id);
        }
        return removed;
    }
public:
    
    const std::unordered_map<int, Pipe>& get_pipes() const { 
        return pipe_data; 
    }

    const std::unordered_map<int, Cs>& get_css() const {
        return cs_data;
    }

    bool is_empty_pipes() const {
        return pipe_data.empty();
    }
    bool is_empty_css() const {
        return cs_data.empty();
    }

    void edit_pipes(const std::set<int>& ids, bool in_work);
    void edit_css(const std::set<int>& ids, int action);
    size_t remove_pipes(const std::set<int>& ids) {
        return remove_impl(pipe_data, ids);
    }
    size_t remove_css(const std::set<int>& ids);

    void add_pipe_in_gts();
    void add_cs_in_gts();

    bool cs_exists(int id) const {
        return cs_data.count(id) > 0;
    }

    bool connect(int pipe_id, int Cs_out_id, int Cs_in_id);
    bool disconnect(int pipe_id);

    int find_free_pipe(int diameter) const;
    int add_pipe_interactive(int required_diameter);
    void topologic_sort() const;

    void clear();

    void print() const;
    void save_f(const std::string& filename) const;
    void load_f(const std::string& filename);
};