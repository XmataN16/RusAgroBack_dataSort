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

    // Инициализация URL адресов API и токена
    std::string TOKEN = "ypiVZz-L9GvuKxJpmxe3";
    std::string URL1 = "https://operations.cropwise.com/api/v3/fields?user_api_token=";
    std::string URL2 = "https://operations.cropwise.com/api/v3/field_scout_reports_aggregated?scout_report_template_id=71&season=2024&user_api_token=";
    std::string URL3 = "https://operations.cropwise.com/api/v3/history_items?year=2024&user_api_token=";
    std::string URL4 = "https://operations.cropwise.com/api/v3/field_groups?user_api_token=";

    // Вызов функций для отправки API запросов по соответствующим URL для получения данных в формате JSON
    nlohmann::json jsonData1 = api_request((URL1 + TOKEN).c_str());
    nlohmann::json jsonData2 = api_request((URL2 + TOKEN).c_str());
    nlohmann::json jsonData3 = api_request((URL3 + TOKEN).c_str());
    nlohmann::json jsonData4 = api_request((URL4 + TOKEN).c_str());


    // Создаем вектор для хранения полей
    std::vector<Endpoint1> data1;
    std::vector<Endpoint2> data2;
    std::vector<Endpoint3> data3;
    std::vector<Endpoint4> data4;

    // Парсим поля из JSON и заполняем вектор
    for (const auto& item : jsonData1["data"])
    {
        data1.push_back(Endpoint1(item));
    }

    for (const auto& item : jsonData2["data"])
    {
        data2.push_back(Endpoint2(item));
    }

    for (const auto& item : jsonData3["data"])
    {
        data3.push_back(Endpoint3(item));
    }

    for (const auto& item : jsonData4["data"])
    {
        data4.push_back(Endpoint4(item));
    }

    for (int i = 0; i < data4.size(); i++)
    {
        data3[i].print();
    }

    return 0;
}