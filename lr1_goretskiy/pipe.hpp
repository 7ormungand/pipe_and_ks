#pragma once
#include <string>
#include <iostream>


class Pipe {
    private:
        static int next_id;
        std::string name = "";
        int id = 0;
        double length = 0;
        int d = 0;
        bool work = false;
    public:
        Pipe() = default;
        Pipe(std::string new_name, double new_l, int new_d, bool new_status);

        void print() const;

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

        void edit(bool choice);
};