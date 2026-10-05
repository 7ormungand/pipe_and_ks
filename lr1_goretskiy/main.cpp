#include <iostream>
#include <string>
#include <fstream>


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

        void print_p() {
            if (d == 0) {
                return;
            }
            
            std::cout << "Название (км отметка): " << (name.empty() ? "(пусто)" : name) << "\n";
            std::cout << "Длина: " << length << "\n";
            std::cout << "Диаметр: " << d << "\n";
            std::cout << "Статус: " << (work ? "В работе" : "В ремонте" ) << "\n";
        }

        void edit_p() {
            if (d == 0) {
                std::cout << "Ошибка: труба не создана!\n";
                return;
            }
            work = !work;
            std::cout << "Статус: '" << (!work ? "В работе" : "В ремонте") << "' заменён на: " << (work ? "В работе" : "В ремонте") << "\n";
        }

        void save_p(std::ofstream& out) const {
            bool has_pipe = (d > 0);
            out << has_pipe << "\n";
            if (has_pipe) {
                out << name << "\n" << length << "\n" << d << "\n" << work << "\n";
            }
        }

        void load_pipe(std::ifstream& in) {
            bool has_pipe = false;
            in >> has_pipe;
            if (has_pipe) {
                in.ignore();
                std::getline(in, name);
                in >> length >> d >>work;
            } else {
                *this = Pipe();
            }
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

        void print_cs() {
            if (c_shop == 0) {
                return;
            }
        
            std::cout << "Название: " << (name.empty() ? "(пусто)" : name) << "\n";
            std::cout << "Всего цехов: " << c_shop << "\n";
            std::cout << "Цехов в работе: " << c_shop_w << "\n";
            std::cout << "Класс станции: " << cl << "\n";
        }

        void edit_cs() {
            if (c_shop == 0) {
                std::cout << "Ошибка: КС не создана!\n";
                return;
            }
            std::cout << "\nРедактирование КС:\n";
            std::cout << "1. Запустить цех\n";
            std::cout << "2. Остановить цех\n";
            std::cout << "Выберите действие: ";

            int choice;
            while (!(std::cin >> choice) || (std::cin.peek() != '\n' && std::cin.peek() != EOF) || (choice != 1 && choice != 2)) {
                clear_input();
                std::cout << "Некорректный ввод. Введите 1 или 2: ";
            }

            clear_screen();
            clear_input();
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
            bool has_cs = (c_shop > 0);
            out << has_cs << "\n";
            if (has_cs) {
                out << name << "\n" << c_shop << "\n" << c_shop_w << "\n" << cl << "\n";
            }
        }

        void load_cs(std::ifstream& in) {
            bool has_cs = false;
            in >> has_cs;
            if (has_cs) {
                in.ignore();
                std::getline(in, name);
                in >> c_shop >> c_shop_w >> cl;
            } else {
                *this = Cs();
            }
        }
};


void save_f(const Pipe& pipe, const Cs& cs) {
    std::ofstream out("p_cs.txt");
    if (!out.is_open()) {
        std::cout << "Ошибка открытия файла!\n";
        return;
    }
    pipe.save_p(out);
    cs.save_cs(out);
    std::cout << "Данные трубы и КС сохранены в файл\n";
}


void load_f(Pipe& pipe, Cs& cs) {
    std::ifstream in("p_cs.txt");
    if (!in.is_open()) {
        std::cout << "Файла 'p_cs.txt' с данными не найден!";
        return;
    }
    pipe.load_pipe(in);
    cs.load_cs(in);
    std::cout << "Данные загружены из p_cs.txt!\n";
}


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
                pipe.add_p();
                break;
            }
            case 2: {
                clear_screen();
                cs.add_cs();
                break;
            }
            case 3: {
                clear_screen();
                std::cout << "\n    Труба    \n";
                pipe.print_p(); 
                std::cout << "\n    КС    \n";
                cs.print_cs(); 
                break;
            }
            case 4: {
                clear_screen();
                pipe.edit_p();
                break;
            }
            case 5: {
                clear_screen();
                cs.edit_cs();
                break;
            }
            case 6: {
                clear_screen();
                save_f(pipe, cs);
                break;
            }
            case 7: {
                clear_screen();
                load_f(pipe, cs);
                break;
            }
            case 0: {
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