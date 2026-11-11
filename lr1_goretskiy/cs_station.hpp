#pragma once
#include <string>
#include <iostream>


class Cs {
    private:
        static int next_id;
        std::string name = "";
        int id = 0;
        int c_shop = 0;
        int c_shop_w = 0;
        char cl = 'E';
    public:
        Cs() = default;
        Cs(std::string new_name, int new_c_shop, int new_c_shop_w, char new_cl);

        void print() const;
        
        friend std::ostream& operator<<(std::ostream& out, const Cs& cs);
        friend std::istream& operator>>(std::istream& in, Cs& cs);

        std::string get_name() const {
            return name;
        }

        int get_id() const {
            return id;
        }

        double get_percentage() const {
            if (c_shop == 0) {
                return 0.0;
            }
            return ((double)(c_shop - c_shop_w) / c_shop) * 100.0;
        }    

        void edit(int choice); 
};