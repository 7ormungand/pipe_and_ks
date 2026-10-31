#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "gts.hpp"


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
            "0. Выход\n\n",
            "Выберите действие: "
        };

        bool search_case(const std::string& str, const std::string& substr);

        template<typename T>
        std::vector<int> search_p(const std::unordered_map<int, T>& some_type);

        void redact_pipe(std::unordered_map<int, Pipe>& items, std::vector<int>& target_ids);
        void redact_cs(std::unordered_map<int, Cs>& items, std::vector<int>& target_ids);

        template<typename T>
        void edit_p_batch(std::unordered_map<int, T>& items);

    public:
        Console(GTS& gts) : gts(gts) {}

        void printMenu() const;
        void printGts() const;

        void add_pipe_in_gts();
        void add_cs_in_gts();

        void batch_edit_pipes();
        void batch_edit_stations();
};  