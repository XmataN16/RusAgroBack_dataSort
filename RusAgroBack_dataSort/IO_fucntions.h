#pragma once
// Функция для записи данных, полученных через CURL
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

    // Инициализация CURL
    curl = curl_easy_init();
    if (curl)
    {
        // Указываем URL для запроса
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        // Настраиваем функцию для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Выполняем запрос
        res = curl_easy_perform(curl);

        // Проверяем результат запроса
        if (res != CURLE_OK)
        {
            std::cerr << u8"curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            // Парсим ответ в формат JSON
            try
            {
                nlohmann::json jsonData = nlohmann::json::parse(readBuffer);
                return jsonData;
            }
            catch (nlohmann::json::parse_error& e)
            {
                std::cerr << u8"Ошибка при парсинге JSON: " << e.what() << std::endl;
            }
        }

        // Завершаем работу с CURL
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
