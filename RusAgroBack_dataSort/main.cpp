#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <time.h>
#include <ctime>
#include <cstdio>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <omp.h>

#include <dates_func.h> // Файл с функциями для работы с типом std::tm (даты)
#include <data_struct.h> // Файл с описанием структур данных для хранения
#include <IO_fucntions.h> // Файл с функциями для HTTP запросов и парсинга ответов в JSON формат




int main() 
{
    SetConsoleOutputCP(65001);

    nlohmann::json jsonData = api_request("https://operations.cropwise.com/api/v3/fields?user_api_token=ypiVZz-L9GvuKxJpmxe3");

    // Создаем вектор для хранения полей
    std::vector<Field> fields;

    // Парсим поля из JSON и заполняем вектор
    for (const auto& item : jsonData["data"])
    {
        fields.push_back(Field(item));
    }

    // Пример вывода информации о полях
    for (int i = 0; i < fields.size(); i++)
    {
        fields[i].print();
    }

    return 0;
}