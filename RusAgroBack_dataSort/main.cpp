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

    nlohmann::json jsonData = api_request("https://operations.cropwise.com/api/v3/fields?user_api_token=ypiVZz-L9GvuKxJpmxe3");

    // ������� ������ ��� �������� �����
    std::vector<Field> fields;

    // ������ ���� �� JSON � ��������� ������
    for (const auto& item : jsonData["data"])
    {
        fields.push_back(Field(item));
    }

    // ������ ������ ���������� � �����
    for (int i = 0; i < fields.size(); i++)
    {
        fields[i].print();
    }

    return 0;
}