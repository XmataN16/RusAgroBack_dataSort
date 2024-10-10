#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
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
#include <thread>
#include <chrono>



#include <dates_func.h> // ���� � ��������� ��� ������ � ����� std::tm (����)
#include <data_struct.h> // ���� � ��������� �������� ������ ��� ��������
#include <IO_fucntions.h> // ���� � ��������� ��� HTTP �������� � �������� ������� � JSON ������
#include <decades_functions.h> // ���� � ������� ����������� ��������� ������ �� ������

int main() 
{
    SetConsoleOutputCP(65001);

    // ������������� URL ������� API � ������
    std::string URL1_with_ids = "https://operations.cropwise.com/api/v3/fields/ids";
    std::string URL1_with_data = "https://operations.cropwise.com/api/v3/fields";
    std::string URL2_with_ids = "https://operations.cropwise.com/api/v3/field_scout_reports_aggregated/ids";
    std::string URL2_with_data = "https://operations.cropwise.com/api/v3/field_scout_reports_aggregated";
    std::string URL3_with_ids = "https://operations.cropwise.com/api/v3/history_items/ids";
    std::string URL3_with_data = "https://operations.cropwise.com/api/v3/history_items";
    std::string URL4_with_ids = "https://operations.cropwise.com/api/v3/field_groups/ids";
    std::string URL4_with_data = "https://operations.cropwise.com/api/v3/field_groups";
    std::string TOKEN = "ypiVZz-L9GvuKxJpmxe3";

    // ��������� ���� ID
    std::vector<int> all_ids1 = get_ids(URL1_with_ids, { {"user_api_token", TOKEN} });
    std::vector<int> all_ids2 = get_ids(URL2_with_ids, { {"scout_report_template_id", "71"}, {"season", "2024"}, {"user_api_token", TOKEN} });
    std::vector<int> all_ids3 = get_ids(URL3_with_ids, { {"year", "2024"}, {"user_api_token", TOKEN} });
    std::vector<int> all_ids4 = get_ids(URL4_with_ids, { {"user_api_token", TOKEN} });

    if (all_ids2.empty()) 
    {
        std::cerr << u8"�� ������� �������� ID ���������. ���������� ���������." << std::endl;
        curl_global_cleanup();
        return 1;
    }

    // ��������� ������� ��������� ��������
    std::vector<int> thousandth_ids1 = get_thousandth_elements(all_ids1);
    std::vector<int> thousandth_ids2 = get_thousandth_elements(all_ids2);
    std::vector<int> thousandth_ids3 = get_thousandth_elements(all_ids3);
    std::vector<int> thousandth_ids4 = get_thousandth_elements(all_ids4);

    // ������ ��� �������� ���� ������
    std::vector<nlohmann::json> jsonData1 = get_data_thousandths(thousandth_ids1, URL1_with_data, { {"limit", "1000"}, {"user_api_token", TOKEN} });
    std::vector<nlohmann::json> jsonData2 = get_data_thousandths(thousandth_ids2, URL2_with_data, { {"scout_report_template_id", "71"}, {"season", "2024"}, {"limit", "1000"}, {"user_api_token", TOKEN} });
    std::vector<nlohmann::json> jsonData3 = get_data_thousandths(thousandth_ids3, URL3_with_data, { {"year", "2024"}, {"limit", "1000"}, {"user_api_token", TOKEN} });
    std::vector<nlohmann::json> jsonData4 = get_data_thousandths(thousandth_ids4, URL4_with_data, { {"limit", "1000"}, {"user_api_token", TOKEN} });

    // ������� ������ ��� �������� �����
    std::vector<Field> fields;
    std::vector<HistoryItem> historyItems;
    std::vector<FieldsGroup> fieldsGroup;
    std::vector<ScoutReport> scoutReports;

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

    for (int i = 0; i < resultWithoutMeasurements.size(); i++)
    {
        resultWithReports[i].print();
    }

    // ���������� � ����
    saveResultToFile(finalResult, "result.json");

    // ������� �������� libcurl
    curl_global_cleanup();

    return 0;
}

