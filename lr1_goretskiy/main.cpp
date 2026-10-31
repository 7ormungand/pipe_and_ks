#include <iostream> 
#include <string> 
#include <fstream> 

#include <format> 
#include <chrono> 
#include <filesystem> 
#include "utils.hpp" 

#include "console.hpp" 
#include "gts.hpp" 


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

int main() {
    setlocale(LC_ALL, "ru-RU.UTF-8");
    
    std::filesystem::create_directories("logs");
    redirect_output_wrapper cerr_out(std::cerr);
    std::string time = std::format("{:%d_%m_%Y %H_%M_%OS}", std::chrono::system_clock::now());
    std::ofstream logfile("logs/log_" + time + ".txt");
    if (logfile) {
        cerr_out.redirect(logfile);
    }

    GTS gts;
    Console console(gts);

    while (true) {
        clear_screen();
        console.printMenu();

        switch (GetCorrectNumber(0, 7)) {
            case 1: {
                clear_screen();
                console.add_pipe_in_gts();
                break;
            }
            case 2: {
                clear_screen();
                console.add_cs_in_gts();
                break;
            }
            case 3: {
                clear_screen();
                console.printGts();
                break;
            }
            case 4: {
                clear_screen();
                console.batch_edit_pipes();
                break;
            }
            case 5: {
                clear_screen();
                console.batch_edit_stations();
                break;
            }
            case 6: {
                clear_screen();
                std::cout << "Введите имя файла для сохранения: ";
                std::string filename;
                INPUT_LINE(std::cin, filename);
                gts.save_f(filename);
                break;
            }
            case 7: {
                clear_screen();
                std::cout << "Введите имя файла для загрузки: ";
                std::string filename;
                INPUT_LINE(std::cin, filename);
                gts.load_f(filename);
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
        std::cerr << "\n";
    }
}