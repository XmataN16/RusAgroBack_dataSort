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

#include <dates_func.h> // ���� � ��������� ��� ������ � ����� std::tm (����)
#include <data_struct.h> // ���� � ��������� �������� ������ ��� ��������
#include <IO_fucntions.h> // ���� � ��������� ��� HTTP �������� � �������� ������� � JSON ������
#include <decades_functions.h> // ���� � ������� ����������� ��������� ������ �� ������

int main() 
{
    SetConsoleOutputCP(65001);

    // ������������� URL ������� API � ������
    std::string TOKEN = "ypiVZz-L9GvuKxJpmxe3";
    std::string URL1 = "https://operations.cropwise.com/api/v3/fields?user_api_token=";
    std::string URL2 = "https://operations.cropwise.com/api/v3/field_scout_reports_aggregated?scout_report_template_id=71&season=2024&user_api_token=";
    std::string URL3 = "https://operations.cropwise.com/api/v3/history_items?year=2024&user_api_token=";
    std::string URL4 = "https://operations.cropwise.com/api/v3/field_groups?user_api_token=";

    // ����� ������� ��� �������� API �������� �� ��������������� URL ��� ��������� ������ � ������� JSON
    nlohmann::json jsonData1 = api_request((URL1 + TOKEN).c_str());
    nlohmann::json jsonData2 = api_request((URL2 + TOKEN).c_str());
    nlohmann::json jsonData3 = api_request((URL3 + TOKEN).c_str());
    nlohmann::json jsonData4 = api_request((URL4 + TOKEN).c_str());


    // ������� ������ ��� �������� �����
    std::vector<Field> fields;
    std::vector<HistoryItem> historyItems;
    std::vector<FieldsGroup> fieldsGroup;
    std::vector<ScoutReport> scoutReports;

    // ������ ���� �� JSON � ��������� ������
    loadJSONtoStruct(fields, jsonData1);
    loadJSONtoStruct(scoutReports, jsonData2);
    loadJSONtoStruct(historyItems, jsonData3);
    loadJSONtoStruct(fieldsGroup, jsonData4);

    // �������� ������ � ��������� TempInstance
    std::vector<TempInstance> resultWithoutMeasurements = calc_result(fields, historyItems, fieldsGroup);
    // ��������� ������ �� �������
    std::vector<TempInstance> resultWithReports = chooseDecade(resultWithoutMeasurements, scoutReports);
    // ��������� ���������� �� ���� �������
    std::vector<ResultInstance> resultWithFilteredReports = filterMaxReportDate(resultWithReports);
    // ��������� ������������� �����������
    std::vector<ResultInstance> finalResult = calcBiological(resultWithFilteredReports);

    saveResultToFile(finalResult, "result.json");

    return 0;
}

