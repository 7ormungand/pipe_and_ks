#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <type_traits>


void clear_input() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

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


void save_f(const std::unordered_map<int, Pipe>& pipes, const std::unordered_map<int, Cs>& css) {
   std::cout << "Введите имя файла для сохранения: ";
   std::string filename;
   std::getline(std::cin, filename);
   
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Ошибка открытия файла!\n";
        return;
    }
    out << pipes.size() << "\n";
    for (const auto& [id, pipe] : pipes) {
        out << id << "\n";
        pipe.save_p(out);
    }
    out << css.size() << "\n";
    for (const auto& [id, cs] : css) {
        out << id << "\n";
        cs.save_cs(out);
    }
    log_action("Сохранение данных в файл: " + filename);
    std::cout << "Все данные успешно сохранены в файл: " << filename << "\n";
}


void load_f(std::unordered_map<int, Pipe>& pipes, std::unordered_map<int, Cs>& css) {
    std::cout << "Введите имя файла для загрузки: ";
    std::string filename;
    std::getline(std::cin, filename);

    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "Файл '" << filename << "' с данными не найден!";
        return;
    }
    pipes.clear();
    css.clear();

    size_t count_pipes = 0;
    if (in >> count_pipes) {
        for (size_t i = 0; i < count_pipes; ++i) {
            int id;
            in >> id;
            Pipe p;
            p.load_pipe(in);
            pipes[id] = p;
        }
    }

    size_t count_css = 0;
    if (in >> count_css) {
        for (size_t i = 0; i < count_css; ++i) {
            int id;
            in >> id;
            Cs cs;
            cs.load_cs(in);
            css[id] = cs;
        }
    }
    log_action("Загрузка данных из файла: " + filename);
    std::cout << "Данные успешно загружены из "<< filename << "!\n";
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
    std::cout << "2. Удалить трубы\n";
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


std::unordered_map<int, Pipe> pipe_data;
std::unordered_map<int, Cs> cs_data;
int num_pipe = 0;
int num_cs = 0;

int main() {
    setlocale(LC_ALL, "ru-RU.UTF-8");
    
    Pipe pipe;
    Cs cs;

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
                pipe_data[++num_pipe] = p;
                log_action("Добавлена труба с ID: " + std::to_string(num_pipe));
                break;
            }
            case 2: {
                clear_screen();
                Cs cs;
                cs.add_cs();
                cs_data[++num_cs] = cs;
                log_action("Добавлена КС с ID: " + std::to_string(num_cs));
                break;
            }
            case 3: {
                clear_screen();
                std::cout << "\n    Трубы    \n";
                if (pipe_data.empty()) {
                    std::cout << "Труб пока нет.\n";
                } else {
                    for (const auto& pair : pipe_data) {
                        std::cout << "ID: " << pair.first << "\n";
                        pair.second.print();
                        std::cout << "------------------\n";
                    }
                }
                std::cout << "\n    КС    \n";
                if (cs_data.empty()) {
                    std::cout << "Труб пока нет.\n";
                } else {
                    for (const auto& pair : cs_data) {
                        std::cout << "ID: " << pair.first << "\n";
                        pair.second.print();
                        std::cout << "------------------\n";
                    }
                }
                log_action("Просмотр всех объектов");
                break;
            }
            case 4: {
                clear_screen();
                edit_p_batch(pipe_data);
                break;
            }
            case 5: {
                clear_screen();
                edit_p_batch(cs_data);
                break;
            }
            case 6: {
                clear_screen();
                save_f(pipe_data, cs_data);
                break;
            }
            case 7: {
                clear_screen();
                load_f(pipe_data, cs_data);
                num_pipe = 0;
                for (const auto& [id, pipe] : pipe_data) {
                    if (id >= num_pipe) {num_pipe = id;}
                }
                num_cs = 0;
                for (const auto& [id, cs] : cs_data) {
                    if (id >= num_cs) {num_cs = id;}
                }
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