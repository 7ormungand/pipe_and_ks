#include <iostream>
#include <string>
#include <fstream>
#include "clear_in.hpp"


class Pipe {
    private:
        std::string name = "";
        double length = 0;
        int d = 0;
        bool work = false;
    public:
        void add_p() {
            std::cout << "      Добавление трубы\n";
            std::cout << "Введите километровую отметку (название): ";
            while (!(std::getline(std::cin, name)) || name.empty()) {
                std::cout << "Некорректная длина. Повторите ввод: ";
            }

            std::cout << "Введите длину (км): ";
            while (!(std::cin >> length) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || length <= 0) {
                std::cout << "Некорректно. Повторите ввод: ";
                clear_input();
            }

            std::cout << "Введите диаметр (мм): ";
            while (!(std::cin >> d) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || d <= 0) {
                std::cout << "Некорректно. Повторите ввод: ";
                clear_input();
            }
            work = true;
            clear_input();
            std::cout << "Труба добавлена!\n";
        }

        void print() const {
            if (d == 0) {
                return;
            }
            
            std::cout << "Название (км отметка): " << (name.empty() ? "(пусто)" : name) << "\n";
            std::cout << "Длина: " << length << "\n";
            std::cout << "Диаметр: " << d << "\n";
            std::cout << "Статус: " << (work ? "В работе" : "В ремонте" ) << "\n";
        }

        void save_p(std::ofstream& out) const {
            out << name << "\n" << length << "\n" << d << "\n" << work << "\n";
        }

        void load_pipe(std::ifstream& in) {
            in.ignore();
            std::getline(in, name);
            in >> length >> d >> work;
        
        }

        std::string get_name() const {
            return name;
        }
        bool is_in_repair() const {
            return !work;
        }

        void edit_p() {
            work = !work;
        }

        void set_work(bool status) {
            work = status;
        }
    
};