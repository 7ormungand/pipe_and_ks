#include <iostream>
#include <string>
#include <fstream>
#include "clear_in.hpp"


class Cs {
    private:
        std::string name = "";
        int c_shop = 0;
        int c_shop_w = 0;
        char cl = 'E';
    public:
        void add_cs() {
            std::cout << "      Добавление КС\n";
            std::cout << "Введите название КС: ";
            while (!(std::getline(std::cin, name)) || name.empty()) {
                std::cout << "Некорректная длина. Повторите ввод: ";
            }

            std::cout << "Введите общее количество цехов: ";
            while (!(std::cin >> c_shop) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || c_shop <= 0) {
                std::cout << "Количество цехов должно быть > 0. Повторите: ";
                clear_input();
            }

            std::cout << "Введите количество цехов в работе: ";
            while (!(std::cin >> c_shop_w) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || c_shop_w > c_shop || c_shop_w < 0) {
                std::cout << "Цехов в работе не может быть больше, чем всего (" << c_shop << "). Повторите: ";
                clear_input();
            }

            std::string input_cl;
            std::cout << "Введите Класс станции (A, B, C, D, E): ";
            while (true) {
                std::cin >> input_cl;
                if (input_cl.length() == 1) {
                    char c = toupper(input_cl[0]);
                    if (c == 'A' || c == 'B' ||c == 'C' || c == 'D' || c == 'E') {
                        cl = c;
                        break;
                    }
                }
                std::cout << "Некорректный класс! Введите одну из букв (A, B, C, D, E): ";
                clear_input();
            }
            clear_input();//!!!
            std::cout << "КС добавлена!\n";
        }

        void print() const {
            if (c_shop == 0) {
                return;
            }
        
            std::cout << "Название: " << (name.empty() ? "(пусто)" : name) << "\n";
            std::cout << "Всего цехов: " << c_shop << "\n";
            std::cout << "Цехов в работе: " << c_shop_w << "\n";
            std::cout << "Класс станции: " << cl << "\n";
            std::cout << "Процент неработающих цехов: " << get_percentage() << "%\n";
        }

        void edit_cs(int choice) {
            if (choice == 1) {
                if (c_shop_w < c_shop) {
                    c_shop_w++;
                    std::cout << "Цех запущен. Цехов в работе: " << c_shop_w << "\n";
                } else {
                    std::cout << "Все цеха уже работают!\n";
                }
            } else if (choice == 2) {
                if (c_shop_w > 0) {
                    c_shop_w--;
                    std::cout << "Цех остановлен. Цехов в работе: " << c_shop_w << "\n";
                } else {
                    std::cout << "Все цеха уже остановлены!\n";
                }
            }
        }

        void save_cs(std::ofstream& out) const {
            out << name << "\n" << c_shop << "\n" << c_shop_w << "\n" << cl << "\n";
        }

        void load_cs(std::ifstream& in) {
            in.ignore();
            std::getline(in, name);
            in >> c_shop >> c_shop_w >> cl;
            
        }

        std::string get_name() const {
            return name;
        }

        double get_percentage() const {
            if (c_shop == 0) {
                return 0.0;
            }
            return ((double)(c_shop - c_shop_w) / c_shop) * 100.0;
        }
};