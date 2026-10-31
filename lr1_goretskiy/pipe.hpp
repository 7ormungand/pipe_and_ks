#pragma once
#include <string>
#include <iostream>


class Pipe {
    private:
        std::string name = "";
        int id = 1;
        double length = 0;
        int d = 0;
        bool work = false;
    public:
        Pipe() = default;
        Pipe(std::string new_name, int new_id, double new_l, int new_d, bool new_status);

        void print() const;
        void edit(int choice);

        friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
        friend std::istream& operator>>(std::istream& in, Pipe& p);
        
        std::string get_name() const {
            return name;
        }

        int get_id() const {
            return id;
        }

        bool is_in_repair() const {
            return !work;
        }
};