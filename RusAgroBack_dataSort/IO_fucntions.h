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
