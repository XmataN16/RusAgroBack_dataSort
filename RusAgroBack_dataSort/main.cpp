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
#include <algorithm>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <omp.h>

#include <dates_func.h> // Файл с функциями для работы с типом std::tm (даты)
#include <data_struct.h> // Файл с описанием структур данных для хранения
#include <IO_fucntions.h> // Файл с функциями для HTTP запросов и парсинга ответов в JSON формат
#include <decades_functions.h> // Файл с логикой описывающей разбиение данных на декады

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
    std::vector<Field> fields;
    std::vector<HistoryItem> historyItems;
    std::vector<FieldsGroup> fieldsGroup;
    std::vector<ScoutReport> scoutReports;

    // Парсим поля из JSON и заполняем вектор
    loadJSONtoStruct(fields, jsonData1);
    loadJSONtoStruct(scoutReports, jsonData2);
    loadJSONtoStruct(historyItems, jsonData3);
    loadJSONtoStruct(fieldsGroup, jsonData4);

    // Собираем данные в структуру TempInstance
    std::vector<TempInstance> resultWithoutMeasurements = calc_result(fields, historyItems, fieldsGroup);
    // Разбиваем данные по декадам
    std::vector<TempInstance> resultWithReports = chooseDecade(resultWithoutMeasurements, scoutReports);
    // Применяем фильтрацию по дате отчётов
    std::vector<ResultInstance> resultWithFilteredReports = filterMaxReportDate(resultWithReports);
    // Вычисляем биологическую урожайность
    std::vector<ResultInstance> finalResult = calcBiological(resultWithFilteredReports);

    saveResultToFile(finalResult, "result.json");

    return 0;
}

