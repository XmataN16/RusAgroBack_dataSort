#pragma once
// ������� ��� ������ ������, ���������� ����� CURL
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t totalSize = size * nmemb;
    s->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

nlohmann::json api_request(const char* URL)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // ������������� CURL
    curl = curl_easy_init();
    if (curl)
    {
        // ��������� URL ��� �������
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        // ����������� ������� ��� ������ ������
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // ��������� ������
        res = curl_easy_perform(curl);

        // ��������� ��������� �������
        if (res != CURLE_OK)
        {
            std::cerr << u8"curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            // ������ ����� � ������ JSON
            try
            {
                nlohmann::json jsonData = nlohmann::json::parse(readBuffer);
                return jsonData;
            }
            catch (nlohmann::json::parse_error& e)
            {
                std::cerr << u8"������ ��� �������� JSON: " << e.what() << std::endl;
            }
        }

        // ��������� ������ � CURL
        curl_easy_cleanup(curl);
    }
}

template<typename T>
void loadJSONtoStruct(std::vector<T>& data, nlohmann::json jsonData)
{
    for (const auto& item : jsonData["data"])
    {
        data.push_back(T(item));
    }
}

void saveResultToFile(const std::vector<ResultInstance>& result, const std::string& fileName)
{
    nlohmann::json jsonResult;

    // �������� �� ������� �������� ������� ResultInstance � ��������� � JSON
    for (const auto& instance : result)
    {
        nlohmann::json instanceJson;

        // ��������� PU, ���� ����
        if (instance.pu.has_value())
        {
            instanceJson["pu"] = instance.pu.value();
        }
        else
        {
            instanceJson["pu"] = nullptr;
        }

        // ��������� field_number
        instanceJson["field_number"] = instance.field_number;

        // ��������� ������� (area)
        instanceJson["area"] = instance.area;

        // ��������� ���� ������ (sawing_date)
        if (instance.sawing_date.has_value())
        {
            std::tm date = instance.sawing_date.value();
            char buffer[20];
            strftime(buffer, 20, "%Y-%m-%d", &date);
            instanceJson["sawing_date"] = buffer;
        }
        else
        {
            instanceJson["sawing_date"] = nullptr;
        }

        // ��������� measurements ��� ������ ������
        for (const auto& measurement : instance.decade_measurements)
        {
            nlohmann::json measurementJson;
            measurementJson["decade"] = measurement.name_decade;

            if (measurement.report.has_value())
            {
                nlohmann::json reportsJson;
                for (const auto& meas : measurement.report.value())
                {
                    nlohmann::json measJson;
                    measJson["name"] = meas.name.value_or("unknown");
                    measJson["value"] = meas.value.value_or(0.0);
                    reportsJson.push_back(measJson);
                }
                measurementJson["reports"] = reportsJson;
            }
            else
            {
                measurementJson["reports"] = nullptr;
            }

            instanceJson["measurements"].push_back(measurementJson);
        }

        jsonResult.push_back(instanceJson);
    }

    // ���������� JSON � ����
    std::ofstream file(fileName);
    if (file.is_open())
    {
        file << std::setw(4) << jsonResult << std::endl;
        file.close();
        std::cout << "Result saved to file: " << fileName << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}

