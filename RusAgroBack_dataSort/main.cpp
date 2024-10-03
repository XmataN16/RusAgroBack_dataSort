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

#include <dates_func.h> // ���� � ��������� ��� ������ � ����� std::tm (����)
#include <data_struct.h> // ���� � ��������� �������� ������ ��� ��������
#include <IO_fucntions.h> // ���� � ��������� ��� HTTP �������� � �������� ������� � JSON ������


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
    std::vector<Endpoint1> data1;
    std::vector<Endpoint2> data2;
    std::vector<Endpoint3> data3;
    std::vector<Endpoint4> data4;

    // ������ ���� �� JSON � ��������� ������
    loadJSONtoStruct(data1, jsonData1);
    loadJSONtoStruct(data2, jsonData2);
    loadJSONtoStruct(data3, jsonData3);
    loadJSONtoStruct(data4, jsonData4);

    for (int i = 0; i < data4.size(); i++)
    {
        data2[i].print();
    }

    return 0;
}

