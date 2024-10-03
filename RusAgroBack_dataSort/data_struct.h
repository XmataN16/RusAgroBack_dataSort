#pragma once
// Структура для хранения ответа с первого API запроса
struct Endpoint1
{
public:
    int id;
    int field_group_id;
    std::optional<std::string> name;
    std::optional<double> calculated_area;

    Endpoint1(nlohmann::json jsonData)
    {
        id = jsonData["id"];
        field_group_id = jsonData["field_group_id"];
        name = jsonData["name"].is_null() ? std::nullopt : std::optional(jsonData["name"].get<std::string>());
        calculated_area = jsonData["calculated_area"].is_null() ? std::nullopt : std::optional(jsonData["calculated_area"].get<double>());
    }

    void print()
    {
        std::cout << "id: " << id << std::endl;
        std::cout << "field_group_id: " << field_group_id << std::endl;
        if (name.has_value())
        {
            std::cout << "name: " << name.value() << std::endl;
        }
        else
        {
            std::cout << "name: NULL \n";
        }
        if (calculated_area.has_value())
        {
            std::cout << "calculated_area: " << calculated_area.value() << std::endl;
        }
        else
        {
            std::cout << "calculated_area: NULL \n";
        }

        std::cout << "-------------------------------------\n";
    }

    // Декларация для сопоставления с JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Endpoint1, id, field_group_id, name.value(), calculated_area.value())
};


struct Measurement 
{
    std::string name;
    std::optional<int> value;      // Некоторые значения измерений — float

    Measurement()
    {
        this->name = "NULL";
        this->value = 0;

    }

    // Декларация для сопоставления с JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Measurement, name, value.value())
};

// Структура для хранения ответа с второго API запроса
struct Endpoint2 
{
    int field_id;
    std::optional<std::tm> report_time;
    std::vector<Measurement> measurements = {Measurement(), Measurement(), Measurement(), Measurement()};  // Массив измерений

    // Конструктор для инициализации из JSON
    Endpoint2(const nlohmann::json& jsonData) 
    {
        field_id = jsonData["field_id"];
        report_time = jsonData["report_time"].is_null() ? std::nullopt : str_to_tm(std::optional(jsonData["report_time"]));

        // Парсинг массива measurements
        for (const auto& measurementJson : jsonData["measurements"]) 
        {
            Measurement measurement;
            //std::cout << measurementJson["name"] << std::endl;
            if (measurementJson["name"] == u8"Средний вес корня, грамм")
            {

                if (measurementJson.contains("name") && measurementJson["name"].is_string())
                {
                    measurement.name = measurementJson["name"];
                }
                else
                {
                    std::cerr << "Measurement 'name' is missing or not a string\n";
                }
                if (measurementJson.contains("value") && measurementJson["value"].is_number())
                {
                    measurement.value = measurementJson["value"];
                }
                else
                {
                    measurement.value = 0;
                }
                measurements[0] = measurement;
            }
            else if (measurementJson["name"] == u8"Густота, тыс. шт/га")
            {
                if (measurementJson.contains("name") && measurementJson["name"].is_string())
                {
                    measurement.name = measurementJson["name"];
                }
                else
                {
                    std::cerr << "Measurement 'name' is missing or not a string\n";
                }
                if (measurementJson.contains("value") && measurementJson["value"].is_number())
                {
                    measurement.value = measurementJson["value"];
                }
                else
                {
                    measurement.value = 0;
                }
                measurements[1] = measurement;
            }
            else if (measurementJson["name"] == u8"Дигестия корня, %")
            {
                if (measurementJson.contains("name") && measurementJson["name"].is_string())
                {
                    measurement.name = measurementJson["name"];
                }
                else
                {
                    std::cerr << "Measurement 'name' is missing or not a string\n";
                }
                if (measurementJson.contains("value") && measurementJson["value"].is_number())
                {
                    measurement.value = measurementJson["value"];
                }
                else
                {
                    measurement.value = 0;
                }
                measurements[2] = measurement;
            }
            else
            {
                continue;
            }
            
        }

        // Парсинг изображений

    }

    void print() const
    {
        std::cout << "field_id: " << this->field_id << std::endl;

        if (this->report_time.has_value())
        {
            std::tm report_time_tm = this->report_time.value();
            std::cout << "report_time: " << std::put_time(&report_time_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
        }
        else
        {
            std::cout << "report_time: NULL" << std::endl;
        }

        std::cout << "Measurements:" << std::endl;
        for (const auto& measurement : measurements)
        {
            std::cout << "  name: " << measurement.name << std::endl;
            if (measurement.value.has_value())
            {
                std::cout << "  value: " << measurement.value.value() << std::endl;
            }
            else
            {
                std::cout << "  value: NULL" << std::endl;
            }
        }

        std::cout << "-------------------------------------\n";
    }
};

// Структура для хранения ответа с третьего API запроса
struct Endpoint3
{
    int field_id;
    std::optional<std::tm> sowing_date;

    Endpoint3(const nlohmann::json& jsonData)
    {
        field_id = jsonData["field_id"];
        sowing_date = jsonData["sowing_date"].is_null() ? std::nullopt : str_to_tm(std::optional(jsonData["sowing_date"]));
    }

    void print() const
    {
        std::cout << "field_id: " << this->field_id << std::endl;

        if (this->sowing_date.has_value())
        {
            std::tm sowing_date_tm = this->sowing_date.value();
            std::cout << "sowing_date: " << std::put_time(&sowing_date_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
        }
        else
        {
            std::cout << "sowing_date: NULL" << std::endl;
        }
        std::cout << "-------------------------------------\n";
    }
};

// Структура для хранения ответа с четвертого API запроса
struct Endpoint4
{
    int id;
    std::optional<std::string> name;

    Endpoint4(const nlohmann::json& jsonData)
    {
        id = jsonData["id"];
        name = jsonData["name"].is_null() ? std::nullopt : std::optional(jsonData["name"].get<std::string>());
    }

    void print() const
    {
        std::cout << "id: " << id << std::endl;
        if (name.has_value())
        {
            std::cout << "name: " << name.value() << std::endl;
        }
        else
        {
            std::cout << "name: NULL \n";
        }

        std::cout << "-------------------------------------\n";
    }
};

// Структура для хранения декад с данными
struct ResultMeasurement 
{
    std::string date;
    int density;
    int root_weight;
    int digestion;
    int biological_yield;

    // Конструктор для инициализации из JSON
    ResultMeasurement()
    {

    }

    // Функция для вывода данных измерения
    void print() const 
    {
        std::cout << "Date: " << date << std::endl;
        std::cout << "Density: " << density << std::endl;
        std::cout << "Root Weight: " << root_weight << std::endl;
        std::cout << "Digestion: " << digestion << std::endl;
        std::cout << "Biological Yield: " << biological_yield << std::endl;
        std::cout << "------------------------" << std::endl;
    }
};

// Структура для хранения данных разбитых по декадам
struct FieldData 
{
    std::string pu;
    std::string field_number;
    double area;
    std::string sawing_date;
    std::vector<ResultMeasurement> ResultsMeasurements;

    // Конструктор для инициализации из JSON
    FieldData() 
    {

    }

    // Функция для вывода данных поля
    void print() const 
    {
        std::cout << "PU: " << pu << std::endl;
        std::cout << "Field Number: " << field_number << std::endl;
        std::cout << "Area: " << area << std::endl;
        std::cout << "Sawing Date: " << sawing_date << std::endl;
        std::cout << "Measurements:" << std::endl;
        for (const auto& measurement : ResultsMeasurements)
        {
            measurement.print();
        }
        std::cout << "------------------------" << std::endl;
    }
};