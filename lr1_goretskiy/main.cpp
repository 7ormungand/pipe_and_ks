#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "gts.hpp"
#include "clear_in.hpp"
#include "pipe.hpp"
#include "cs_station.hpp"


void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void pause() {
    std::cout << "\nНажмите Enter, чтобы продолжить...";
    std::cin.get();
}


void log_action(const std::string& action) {
    std::ofstream log_file("log.txt", std::ios::app);
    if (log_file.is_open()) {
        log_file << action << "\n";
    }
}


bool search_case(const std::string& str, const std::string& substr) {
    if (substr.empty()) return true;
    if (substr.size() > str.size()) return false;

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



template<typename T>
std::vector<int> search_p(const std::unordered_map<int, T>& some_type) {
    std::vector<int> found_ids;
    if (some_type.empty()) {
        std::cout << "Список пуст!\n";
        return found_ids;
    }
    std::cout << "\nФильтр поиска:\n";
    std::cout << "1. По наименованию\n";
    std::is_same_v<T, Pipe> ? std::cout << "2. По признаку 'в ремонте'\n" : std::cout << "2. По проценту незадейственных цехов\n";
    std::cout << "Выберите вариант: ";

    int choice;
    while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        clear_input();
        std::cout << "Введите 1 или 2: ";
    }
    clear_input();
    switch (choice) {
        case 1: {
            std::cout << "Введите наименование (или часть): ";
            std::string querry;
            std::getline(std::cin, querry);
            for (const auto& [id, val] : some_type) {
                if (search_case(val.get_name(), querry)) {
                    found_ids.push_back(id);
                }
            }
            break;
        }
        case 2: {
            if constexpr(std::is_same_v<T, Pipe>) {
                std::cout << "Искать трубы:\n1. В ремонте\n2. В работе\nВыбор: ";
                int rep_choice;
                while (!(std::cin >> rep_choice) || (rep_choice != 1 && rep_choice != 2)) {
                    clear_input();
                    std::cout << "Введите 1 или 2: ";
                }
                clear_input();
                bool target_repair = (rep_choice == 1);
                for (const auto& [id, val] : some_type) {
                    if (val.is_in_repair() == target_repair) {
                        found_ids.push_back(id);
                    }
                }   
            } else {
                std::cout << "Искать кс:\nВведите процент незадейственных цехов (0..100): ";
                double rep_choice;
                while (!(std::cin >> rep_choice) || (rep_choice < 0.0 || rep_choice > 100.0)) {
                    clear_input();
                    std::cout << "Введите от 0% до 100%: ";
                }
                clear_input();
                for (const auto& [id, val] : some_type) {
                    if (val.get_percentage() >= rep_choice) {
                        found_ids.push_back(id);
                    }
                }
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
    log_action("Выполнен поиск. Найдено: " +std::to_string(found_ids.size()));
    return found_ids;
}

template<typename T>
void edit_p_batch(std::unordered_map<int, T>& items) {
    std::vector<int> found_ids = search_p(items);
    if (found_ids.empty()) return;

    std::string type_name = std::is_same_v<T, Pipe> ? "труб" : "КС";

    std::cout << "\nПакетное действие со найденными объектами:\n";
    std::cout << "1. Применить редактирование / изменение статуса\n";
    std::cout << "2. Удалить\n";
    std::cout << "0. Отмена\n";
    std::cout << "Выберите действие: ";

    int act;
    while (!(std::cin >> act) || (act < 0 || act > 2)) {
        clear_input();
        std::cout << "Введите число от 0 до 2: ";
    }
    clear_input();
    
    if (act == 0) return;

    std::cout << "\nПрименить к:\n";
    std::cout << "1. Всем найденным объектам\n";
    std::cout << "2. Выбранному подмножеству ID\n";
    std::cout << "Выберите: ";

    int sub_choice;
    while (!(std::cin >> sub_choice) || (sub_choice != 1 && sub_choice != 2)) {
        clear_input();
        std::cout << "Введите 1 или 2: ";
    }
    clear_input();

    std::vector<int> target_ids;
    switch (sub_choice) {
        case 1: {
            target_ids = found_ids;
            break;
        }
        case 2: {
            std::cout << "Введите ID через пробел (завершите ввод элементом 0 или буквой): ";
            int id;
            while (std::cin >> id && id != 0) {
                if (std::find(found_ids.begin(), found_ids.end(), id) != found_ids.end()) {
                    target_ids.push_back(id);
                } else {
                    std::cout << "ID " << id << " отсутствует среди найденных!\n";
                }
            }
            clear_input();
        }
    }
    if (target_ids.empty()) {
        std::cout << "Список ID для обработки пуст.\n";
        return;
    }
    switch (act) {
        case 1: {
            if constexpr (std::is_same_v<T, Pipe>) {
                std::cout << "Выберите новый статус для выбранных труб (1 - В работе, 0 - В ремонте): ";
                int new_status;
                while (!(std::cin >> new_status) || (new_status != 0 && new_status != 1)) {
                    clear_input();
                    std::cout << "Введите 0 или 1: ";
                }
                clear_input();

                for (int id : target_ids) {
                    items[id].set_work(new_status == 1);
                }
                std::cout << "Статус успешно обновлен для " << target_ids.size() << " труб(ы).\n";
            } else {
                std::cout << "\nРедактирование КС:\n";
                std::cout << "1. Запустить цех\n";
                std::cout << "2. Остановить цех\n";
                std::cout << "Выберите действие: ";

                int cs_act;
                while (!(std::cin >> cs_act) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || (cs_act != 1 && cs_act != 2)) {
                    clear_input();
                    std::cout << "Некорректный ввод. Введите 1 или 2: ";
                }
                clear_input();
                for (int id : target_ids) {
                    std::cout << "\nРедактирование КС ID: " << id << "\n";
                    items[id].edit_cs(cs_act);
                }
                std::cout << "Изменения успешно применены к " << target_ids.size() << " КС.\n";
            }

            log_action("Пакетное изменение статуса. Обновлено объектов: " + std::to_string(target_ids.size()));
            break;
        } 
        case 2: {
            for (int id : target_ids) {
                items.erase(id);
            }
            std::cout << "Успешно удалено: " << target_ids.size() << "\n";
            log_action("Пакетное удаление. Удалено объектов: " + std::to_string(target_ids.size()));
            break;
        }
    }
} 

int main() {
    setlocale(LC_ALL, "ru-RU.UTF-8");
    
    GTS gts;

    while (true) {
        clear_screen();
        std::cout << "\n     Меню\n";
        std::cout << "1. Добавить трубу\n";
        std::cout << "2. Добавить КС\n";
        std::cout << "3. Просмотр всех объектов\n";
        std::cout << "4. Редактировать трубу\n";
        std::cout << "5. Редактировать КС\n";
        std::cout << "6. Сохранить\n";
        std::cout << "7. Загрузить\n";
        std::cout << "0. Выход\n\n";
        std::cout << "Выберите действие: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Неверно, введите число от 0 до 7!\n";
            clear_input();
            pause();
            continue;
        }
        clear_input();

        switch (choice) {
            case 1: {
                clear_screen();
                Pipe p;
                p.add_p();
                gts.add_pipes(p);
                log_action("Добавлена новая труба");
                break;
            }
            case 2: {
                clear_screen();
                Cs cs;
                cs.add_cs();
                gts.add_css(cs);
                log_action("Добавлена новая КС");
                break;
            }
            case 3: {
                clear_screen();
                std::cout << "\n    Трубы    \n";
                if (gts.get_pipes().empty()) {
                    std::cout << "Труб пока нет.\n";
                } else {
                    for (const auto& [id, pipe] : gts.get_pipes()) {
                        std::cout << "ID: " << id << "\n";
                        pipe.print();
                        std::cout << "------------------\n";
                    }
                }
                std::cout << "\n    КС    \n";
                if (gts.get_css().empty()) {
                    std::cout << "КС пока нет.\n";
                } else {
                    for (const auto& [id, cs] : gts.get_css()) {
                        std::cout << "ID: " << id << "\n";
                        cs.print();
                        std::cout << "------------------\n";
                    }
                }
                log_action("Просмотр всех объектов");
                break;
            }
            case 4: {
                clear_screen();
                edit_p_batch(gts.get_pipes_mutable());
                break;
            }
            case 5: {
                clear_screen();
                edit_p_batch(gts.get_css_mutable());
                break;
            }
            case 6: {
                clear_screen();
                std::cout << "Введите имя файла для сохранения: ";
                std::string filename;
                std::getline(std::cin, filename);
                gts.save_f(filename);
                log_action("Сохранение данных в файл: " + filename);
                break;
            }
            case 7: {
                clear_screen();
                std::cout << "Введите имя файла для загрузки: ";
                std::string filename;
                std::getline(std::cin, filename);
                gts.load_f(filename);
                log_action("Загрузка данных из файла: " + filename);
                break;
            }
            case 0: {
                log_action("Завершение работы программы");
                std::cout << "Завершение работы программы\n";
                return 0;
            }
            default:{
                std::cout << "Неверный пункт меню. Попробуйте снова.\n";
                break;
            }
        }
        pause();
    }
}