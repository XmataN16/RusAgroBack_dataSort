#pragma once
// ������� ��� ������ ������, ���������� ����� CURL
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t totalSize = size * nmemb;
    s->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// ������� ��� ���������� GET-������� � �����������
std::string api_request(const std::string& url, const std::string& params)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) 
    {
        std::string full_url = url + "?" + params;
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        // ���� ��������� ������������ SSL-����������� (�� ������������� ��� ����������)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "CURL �������: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

// ������� ��� ��������� ���� ID
std::vector<int> get_ids(std::string URL_with_ids, const std::unordered_map<std::string, std::string>& params)
{
    std::vector<int> ids;

    // ����������� JSON � ������ ����������
    std::string str_params = "";
    for (const auto& param : params)
    {
        str_params += param.first + "=" + param.second + "&";
    }

    std::string response = api_request(URL_with_ids, str_params);

    if (response.empty()) 
    {
        std::cerr << u8"������ ����� �� ������� ��� ��������� ID." << std::endl;
        return ids;
    }
    try 
    {
        auto json_response = nlohmann::json::parse(response);
        if (json_response.contains("data") && json_response["data"].is_array()) 
        {
            for (auto& id : json_response["data"]) 
            {
                ids.push_back(id.get<int>());
            }
        }
        else 
        {
            std::cerr << u8"����� �� �������� ���� 'data' ��� ��� �� �������� ��������." << std::endl;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << u8"������ ��� ������� JSON: " << e.what() << std::endl;
    }
    return ids;
}

// ������� ��� ��������� ������� ��������� ��������
std::vector<int> get_thousandth_elements(const std::vector<int>& arr)
{
    std::vector<int> result;

    if (!arr.empty()) 
    {
        result.push_back(arr[0]);
        for (size_t i = 999; i < arr.size(); i += 1000) 
        {
            result.push_back(arr[i]);
        }
    }

    return result;
}

// ������� ��� ��������� ������ �� from_id
std::vector<nlohmann::json> get_data(int from_id, std::string URL_with_data, const std::unordered_map<std::string, std::string>& params)
{
    std::vector<nlohmann::json> data;

    // ����������� JSON � ������ ����������
    std::string str_params = "from_id=" + std::to_string(from_id) + "&";
    for (const auto& param : params)
    {
        str_params += param.first + "=" + param.second + "&";
    }

    std::string response = api_request(URL_with_data, str_params);

    if (response.empty()) 
    {
        std::cerr << u8"������ ����� �� ������� ��� ��������� ������ ��� from_id = " << from_id << std::endl;
        return data;
    }
    try 
    {
        auto json_response = nlohmann::json::parse(response);
        if (json_response.contains("data") && json_response["data"].is_array()) 
        {
            for (auto& item : json_response["data"]) 
            {
                data.push_back(item);
            }
        }
        else 
        {
            std::cerr << u8"����� �� �������� ���� 'data' ��� ��� �� �������� �������� ��� from_id = " << from_id << std::endl;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << u8"������ ��� ������� JSON ��� from_id = " << from_id << ": " << e.what() << std::endl;
    }

    return data;
}

std::vector<nlohmann::json> get_data_thousandths(std::vector<int> thousandth_ids, std::string URL_with_data, const std::unordered_map<std::string, std::string>& params)
{
    std::vector<nlohmann::json> jsonData;

    int count = 1;

    // ��������� ������ �� ������� ��������� ID
    for (const auto& el : thousandth_ids)
    {
        // �������� � 1 �������
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        std::vector<nlohmann::json> batch = get_data(el, URL_with_data, params);
        jsonData.insert(jsonData.end(), batch.begin(), batch.end());

        std::cout << count << u8"������ �� ��������� ������..." << std::endl;
        count++;
    }
    return jsonData;
}

// ������� ��� �������� JSON ������ � ���������
template<typename T>
void loadJSONtoStruct(std::vector<T>& data, const nlohmann::json& jsonData)
{
    for (const auto& item : jsonData)
    {
        data.push_back(item);
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
        if (instance.year.has_value())
        {
            instanceJson["year"] = instance.year.value();
        }
        else
        {
            instanceJson["year"] = nullptr;
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

// ������� ��� ������ JSON �����
nlohmann::json readJSONFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    // ���������, ������� �� ������� ����
    if (!file.is_open())
    {
        std::cerr << u8"������ ��� �������� �����: " << filePath << std::endl;
        return nullptr;
    }

    nlohmann::json jsonData;
    try
    {
        // ������ � ������� ������ �� �����
        file >> jsonData;
    }
    catch (nlohmann::json::parse_error& e)
    {
        std::cerr << u8"������ ��� �������� JSON: " << e.what() << std::endl;
    }

    file.close();
    return jsonData;
}

