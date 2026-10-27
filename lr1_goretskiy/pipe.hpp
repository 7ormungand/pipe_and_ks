#pragma once
#include <iostream>
#include <string>
#include <fstream>


class Pipe {
    private:
        std::string name = "";
        int id = 1;
        double length = 0;
        int d = 0;
        bool work = false;
    public:
        void print() const {
            if (d == 0) {
                return;
            }
            
            std::cout << "Название (км отметка): " << (name.empty() ? "(пусто)" : name) << "\n";
            std::cout << "Длина: " << length << "\n";
            std::cout << "Диаметр: " << d << "\n";
            std::cout << "Статус: " << (work ? "В работе" : "В ремонте" ) << "\n";
        }

        friend std::ostream& operator<<(std::ostream& out, const Pipe& p) {
            out << p.name << "\n" << p.id << "\n" << p.length << "\n" << p.d << "\n" << p.work;
            return out;
        }

        friend std::istream& operator>>(std::istream& in, Pipe& p) {
            std::getline(in, p.name);
            in >> p.id >> p.length >> p.d >> p.work;
            return in;
        }
        
        std::string get_name() const {
            return name;
        }
        bool is_in_repair() const {
            return !work;
        }

        void edit() {
            work = !work;
        }
        
        void set_name(const std::string& new_name) {
            name = new_name;
        }

        void set_id(int new_id) {
            if (new_id > 0) {
                id = new_id;
            }
        }   

        void set_len(double new_len) {
            if (new_len > 0) {
                length = new_len;
            }
        }       
        
        void set_diametr(int new_d) {
            if (new_d > 0) {
                d = new_d;
            }
        }

        void set_work(bool status) {
            work = status;
        }

        int get_id() {
            return id;
        }


};