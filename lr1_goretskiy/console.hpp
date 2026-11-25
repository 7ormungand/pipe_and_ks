#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <set>

#include "gts.hpp"
#include "utils.hpp"


class Console {
    private:
        GTS& gts;

        std::vector<std::string> mainMenu {
            "\n     Меню\n",
            "1. Добавить трубу\n",
            "2. Добавить КС\n",
            "3. Просмотр всех объектов\n",
            "4. Редактировать трубу\n",
            "5. Редактировать КС\n",
            "6. Сохранить\n",
            "7. Загрузить\n",
            "8. Соединить КС трубой\n",
            "9. Отсоединить трубу\n",
            "10. Топологическая сортировка\n",
            "0. Выход\n\n",
            "Выберите действие: "
        };

        std::set<int> search(const std::unordered_map<int, Pipe>& some_type);
        std::set<int> search(const std::unordered_map<int, Cs>& some_type);

        void redact_pipe(std::set<int>& target_ids);
        void redact_cs(std::set<int>& target_ids);

        void edit_batch(const std::unordered_map<int, Pipe>& some_type_data);
        void edit_batch(const std::unordered_map<int, Cs>& some_type_data);

    public:
        Console(GTS& gts) : gts(gts) {}

        void printMenu() const;

        void connect_pipe();
        void disconnect_pipe();

        void batch_edit_pipes();
        void batch_edit_stations();
};  