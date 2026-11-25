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

        int Cs_out_id = -1;
        int Cs_in_id = -1;
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

        int get_d() const {
            return d;
        }

        bool is_in_repair() const {
            return !work;
        }

        int get_Cs_out_id() const {
            return Cs_out_id;
        }

        int get_Cs_in_id() const {
            return Cs_in_id;
        }

        bool is_connected() const {
            return Cs_out_id != -1;
        }

        static int max_id();

        void connect(int CsNew_out_id, int CsNew_in_id) {
            Cs_out_id = CsNew_out_id;
            Cs_in_id = CsNew_in_id;
        }
        void disconnect() {
            Cs_out_id = -1;
            Cs_in_id = -1;
        }


        void input(int required_diameter = 0);
        void edit(bool choice);
};