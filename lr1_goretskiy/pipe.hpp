#pragma once
#include <string>
#include <iostream>
#include <fstream>


class Pipe {
    private:
        static int next_id;
        std::string name = "";
        unsigned long int id = 0;
        double length = 0;
        int d = 0;
        bool work = false;
    public:
        Pipe() = default;
        Pipe(std::string new_name, double new_l, int new_d, bool new_status);

        void print() const;

        friend std::ofstream& operator<<(std::ofstream& out, const Pipe& p);
        friend std::ifstream& operator>>(std::ifstream& in, Pipe& p);
        
        std::string get_name() const {
            return name;
        }

        int get_id() const {
            return id;
        }

        bool is_in_repair() const {
            return !work;
        }

        void input();
        void edit(bool choice);
};