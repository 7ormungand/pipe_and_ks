#pragma once
#include <iostream>
#include <string>
#include <fstream>


class Cs {
    private:
        std::string name = "";
        int id = 1;
        int c_shop = 0;
        int c_shop_w = 0;
        char cl = 'E';
    public:
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

        void set_name(const std::string& new_name) {
            name = new_name;
        }

        void set_id(int new_id) {
            if (new_id > 0) {
                id = new_id;
            }
        }  

        void set_shops(int total, int working) {
            if (total > 0 && working >= 0 && working <= total) {
                c_shop = total;
                c_shop_w = working;
            }
        }

        void set_class(char new_cl) {
            char c = toupper(new_cl);
            if (c >= 'A' && c <= 'E') {
                cl = c;
            }
        }

        void edit(int choice) {
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


        friend std::ostream& operator<<(std::ostream& out, const Cs& cs) {
            out <<cs. name << "\n" << cs.id << "\n" << cs.c_shop << "\n" << cs.c_shop_w << "\n" << cs.cl;
            return out;
        }

        friend std::istream& operator>>(std::istream& in, Cs& cs) {
            std::getline(in, cs.name);
            in >> cs.id >> cs.c_shop >> cs.c_shop_w >> cs.cl;
            return in;
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

        int get_id() {
            return id;
        }
};