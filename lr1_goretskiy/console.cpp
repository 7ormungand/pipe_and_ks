#include "console.hpp"
#include <iostream>
#include <cctype>
#include <type_traits>
#include <unordered_set>
#include <sstream>

#include "utils.hpp"
#include "pipe.hpp"
#include "cs_station.hpp"


void Console::printMenu() const {
    for (size_t i = 0; i != mainMenu.size(); i++) {
        std::cout << mainMenu[i];
    }
}


bool search_case(const std::string& str, const std::string& substr) {
    if (substr.empty()) {
        return true;
    }
    if (substr.size() > str.size()) {
        return false;
    }

    for (size_t i = 0; i <= str.size() - substr.size(); ++i) {
        bool match = true;
        for (size_t j = 0; j < substr.size(); ++j) {
            char c1 = std::tolower(str[i+j]);
            char c2 = std::tolower(substr[j]);

            if (c1 != c2) {
                match = false;
                break;
            }
        }

        if (match) {
            return true;
        }
    }
    return false;
}

std::string get_f_name() {
    std::cout << "Введите наименование (или часть): ";
    std::string querry;
    INPUT_LINE(std::cin, querry);
    return querry;
}

bool get_status() {
    std::cout << "Искать трубы:\n1. В ремонте\n2. В работе\nВыбор: ";
    int rep_choice = GetCorrectNumber(1, 2);
    bool target_repair = (rep_choice == 1);
    return target_repair;
}

std::pair<double, double> get_range() {
    std::cout << "Нижняя граница процента (0..100): ";
    double lo = GetCorrectNumber(0.0, 100.0);
    std::cout << "Верхняя граница процента (" << lo << "..100): ";
    double hi = GetCorrectNumber(lo, 100.0);
    return {lo, hi};
}


template<typename T>
bool SearchByName(const T& s, std::string param){
	return search_case(s.get_name(), param);
}

bool SearchByStatus(const Pipe& s, bool param){
	return s.is_in_repair() == param;
}

bool SearchByPercent(const Cs& s, std::pair<double, double> param){
	return s.get_percentage() >= param.first && s.get_percentage() <= param.second;
}


template<typename T>
std::set<int> Console::search_p(const std::unordered_map<int, T>& some_type) {
    std::set<int> found_ids;
    if (some_type.empty()) {
        std::cout << "Список пуст!\n";
        return found_ids;
    }
    std::cout << "\nФильтр поиска:\n";
    std::cout << "1. По наименованию\n";
    std::is_same_v<T, Pipe> ? std::cout << "2. По признаку 'в ремонте'\n" : std::cout << "2. По проценту незадейственных цехов\n";
    std::cout << "Выберите вариант: ";

    switch (GetCorrectNumber(1, 2)) {
        case 1: {
            found_ids = FindObjByFilter(some_type, SearchByName, get_f_name());
            break;
        }
        case 2: {
            if constexpr(std::is_same_v<T, Pipe>) { 
                found_ids = FindObjByFilter(some_type, SearchByStatus, get_status());
            } else {
                found_ids = FindObjByFilter(some_type, SearchByPercent, get_range());
            }
            break;
        }   
    }

    std::cout << "\nРезультаты поиска (" << found_ids.size() << " найдено):\n";
    for (int id : found_ids) {
        std::cout << "ID: " << id << "\n";
        some_type.at(id).print();
        std::cout << "---------------------\n";
    }
    return found_ids;
}

void Console::redact_pipe(std::unordered_map<int, Pipe>& items, std::set<int>& target_ids) {
    std::cout << "Выберите новый статус для выбранных труб (0 - В ремонте, 1 - В работе): ";
    int new_status = GetCorrectNumber(0, 1);

    for (int id : target_ids) {
        items[id].edit(new_status == 1);
    }
    std::cout << "Статус успешно обновлен для " << target_ids.size() << " труб(ы).\n";
}

void Console::redact_cs(std::unordered_map<int, Cs>& items, std::set<int>& target_ids) {
    std::cout << "\nРедактирование КС:\n";
    std::cout << "1. Запустить цех\n";
    std::cout << "2. Остановить цех\n";
    std::cout << "Выберите действие: ";

    int cs_act = GetCorrectNumber(1, 2);

    for (int id : target_ids) {
        items[id].edit(cs_act);
    }
    std::cout << "Изменения успешно применены к " << target_ids.size() << " КС.\n";
}

template<typename T>
void Console::edit_p_batch(std::unordered_map<int, T>& items) {
    std::set<int> found_ids = search_p(items);
    if (found_ids.empty()) {
        return;
    }

    std::cout << "\nПакетное редактирование с найденными объектами:\n";
    std::cout << "1. Применить редактирование / изменение статуса\n";
    std::cout << "2. Удалить\n";
    std::cout << "0. Отмена\n";
    std::cout << "Выберите действие: ";

    int act = GetCorrectNumber(0, 2);
    
    if (act == 0) {
        return;
    }

    std::cout << "\nПрименить к:\n";
    std::cout << "1. Всем найденным объектам\n";
    std::cout << "2. Выбранному подмножеству ID\n";
    std::cout << "Выберите: ";

    std::set<int> target_ids;
    switch (GetCorrectNumber(1, 2)) {
        case 1: {
            target_ids = found_ids;
            break;
        }
        case 2: {
            std::cout << "Введите ID через пробел (завершите ввод элементом 0): ";
            
            std::set<int> alr_tk;

            std::string line;
            INPUT_LINE(std::cin, line);
            std::istringstream iss(line);
            std::string token;

            while (iss >> token && token != "0") {
                if (token.find_first_not_of("0123456789") != std::string::npos) {
                    std::cout << "Некорректный ввод:  " << token << "\n"; 
                    continue;
                }
            
                int id = std::stoi(token);
                if (found_ids.count(id) == 0) {
                    std::cout << "ID " << id << " отсутствует среди найденных!\n";
                } else if (!alr_tk.insert(id).second) {
                    std::cout << "ID " << id << " уже добавлен!\n";
                } else {
                    target_ids.insert(id);
                }
            }
            break;
        }
    }
    if (target_ids.empty()) {
        std::cout << "Список ID для обработки пуст.\n";
        return;
    }
    switch (act) {
        case 1: {
            if constexpr (std::is_same_v<T, Pipe>) {
                redact_pipe(items, target_ids);
            } else {
                redact_cs(items, target_ids);
            }
            break;
        } 
        case 2: {
            for (int id : target_ids) {
                items.erase(id);
            }
            std::cout << "Успешно удалено: " << target_ids.size() << "\n";
            break;
        }
    }
} 

void Console::batch_edit_pipes() {
    edit_p_batch(gts.get_pipes_mutable());
}

void Console::batch_edit_stations() {
    edit_p_batch(gts.get_css_mutable());
}