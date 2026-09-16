#include <iostream>
#include <string>
#include <vector>


struct Pipe {
    std::string name = "";
    double length = 0;
    int d = 0;
    bool work = false;
    bool alive = false;
};

struct Cs {
    std::string name = "";
    int c_shop = 0;
    int c_shop_w = 0;
    bool work = false;
    char cl = 'E';
    bool alive = false;
};


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


void add_pipe(Pipe& pipe) {
    std::cout << "      Добавление трубы\n";
    std::cout << "Введите километровую отметку (название): ";
    std::getline(std::cin, pipe.name);

    std::cout << "Введите длину (км): ";
    while (!(std::cin >> pipe.length) || pipe.length <= 0) {
        std::cout << "Некорректная длина. Повторите ввод: ";
        clear_input();
    }

    std::cout << "Введите диаметр (мм): ";
    while (!(std::cin >> pipe.d) || pipe.d <= 0) {
        std::cout << "Некорректный диаметр. Повторите ввод: ";
        clear_input();
    }
    pipe.work = true;
    pipe.alive = true;
    clear_input();
    std::cout << "Труба добавлена!\n";
}


void print_pipe(const Pipe& pipe) {
    if (!pipe.alive) {
        std::cout << "Труба ещё не создана\n";
        return;
    }
    std::cout << "\n    Труба    \n";
    std::cout << "Название (км отметка): " << pipe.name << "\n";
    std::cout << "Длина: " << pipe.length << "\n";
    std::cout << "Диаметр: " << pipe.d << "\n";
    std::cout << "Статус: " << (pipe.work ? "В работе" : "В ремонте" ) << "\n";
}


void edit_pipe(Pipe& pipe) {
    if (!pipe.alive) {
        std::cout << "Ошибка: труба не создана!\n";
        return;
    }
    pipe.work = !pipe.work;
    std::cout << "Статус: '" << (!pipe.work ? "В работе" : "В ремонте") << "' заменён на: " << (pipe.work ? "В работе" : "В ремонте") << "\n";
}


void add_cs(Cs& cs) {
    std::cout << "      Добавление КС\n";
    std::cout << "Введите название КС: ";
    std::getline(std::cin, cs.name);

    std::cout << "Введите общее количество цехов: ";
    while (!(std::cin >> cs.c_shop) || cs.c_shop <= 0) {
        std::cout << "Количество цехов должно быть > 0. Повторите:";
        clear_input();
    }

    std::cout << "Введите количество цехов в работе: ";
    while (!(std::cin >> cs.c_shop_w) || cs.c_shop_w < 0 || cs.c_shop_w > cs.c_shop) {
        std::cout << "Цехов в работе не может быть больше, чем всего (" << cs.c_shop << "). Повторите: ";
        clear_input();
    }

    std::string input_cl;
    std::cout << "Введите Класс станции (A, B, C, D, E): ";
    while (true) {
        std::cin >> input_cl;
        if (input_cl.length() == 1) {
            char c = toupper(input_cl[0]);
            if (c == 'A' || c == 'B' ||c == 'C' || c == 'D' || c == 'E') {
                cs.cl = c;
                break;
            }
        }
        std::cout << "Некорректный класс! Введите одну из букв (A, B, C, D, E): ";
        clear_input();
    }

    cs.alive = true;
    clear_input();
    std::cout << "КС добавлена!\n";
}


void print_cs(const Cs& cs) {
    if (!cs.alive) {
        std::cout << "\nКС ещё не создана\n";
        return;
    }
    std::cout << "\n    КС    \n";
    std::cout << "Название: " << cs.name << "\n";
    std::cout << "Всего цехов: " << cs.c_shop << "\n";
    std::cout << "Цехов в работе: " << cs.c_shop_w << "\n";
    std::cout << "Класс станции: " << cs.cl << "\n";
}


void edit_cs(Cs& cs) {
    if (!cs.alive) {
        std::cout << "Ошибка: КС не создана!\n";
        return;
    }
    std::cout << "\nРедактирование КС:\n";
    std::cout << "1. Запустить цех\n";
    std::cout << "2. Остановить цех\n";
    std::cout << "Выберите действие: ";

    int choice;
    if (!(std::cin >> choice)) {
        clear_input();
        std::cout << "Неверно!\n";
        return;
    }

    clear_screen();
    clear_input();
    if (choice == 1) {
        if (cs.c_shop_w < cs.c_shop) {
            cs.c_shop_w++;
            std::cout << "Цех запущен. Цехов в работе: " << cs.c_shop_w << "\n";
        } else {
            std::cout << "Все цеха уже работают!\n";
        }
    } else if (choice == 2) {
        if (cs.c_shop_w > 0) {
            cs.c_shop_w--;
            std::cout << "Цех остановлен. Цехов в работе: " << cs.c_shop_w << "\n";
        } else {
            std::cout << "Все цеха уже остановлены!\n";
        }
    } else {
            std::cout << "Неверно!\n";
    }
}


int main() {
    setlocale(LC_ALL, "ru-RU.UTF-8");
    
    Pipe pipe;
    Cs cs;

    while (true) {
        clear_screen();
        std::cout << "\n        Меню\n";
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
                add_pipe(pipe);
                pause();
                break;
            }
            case 2: {
                clear_screen();
                add_cs(cs);
                pause();
                break;
            }
            case 3: {
                clear_screen();
                print_pipe(pipe); 
                print_cs(cs); 
                pause();
                break;
            }
            case 4: {
                clear_screen();
                edit_pipe(pipe);
                pause();
                break;
            }
            case 5: {
                clear_screen();
                edit_cs(cs);
                pause();
                break;
            }
            // case 6: {}
            // case 7: {}
            case 0: {
                std::cout << "Завершение работы программы\n";
                return 0;
            }
            default:{
                std::cout << "Неверный пункт меню. Попробуйте снова.\n";
                pause();
                break;
            }
        }
    }
}