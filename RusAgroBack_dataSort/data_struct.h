#pragma once

std::string Decades[36] = { u8"1-я декада Января", u8"2-я декада Января", u8"3-я декада Января", u8"1-я декада Февраля", u8"2-я декада Февраля", u8"3-я декада Февраля",
    u8"1-я декада Марта", u8"2-я декада Марта", u8"3-я декада Марта", u8"1-я декада Апреля", u8"2-я декада Апреля", u8"3-я декада Апреля",
    u8"1-я декада Мая", u8"2-я декада Мая", u8"3-я декада Мая", u8"1-я декада Июня", u8"2-я декада Июня", u8"3-я декада Июня",
    u8"1-я декада Июля", u8"2-я декада Июля", u8"3-я декада Июля", u8"1-я декада Августа", u8"2-я декада Августа", u8"3-я декада Августа",
    u8"1-я декада Сентября", u8"2-я декада Сентября", u8"3-я декада Сентября", u8"1-я декада Октября", u8"2-я декада Октября", u8"3-я декада Октября",
    u8"1-я декада Ноября", u8"2-я декада Ноября", u8"3-я декада Ноября", u8"1-я декада Декабря", u8"2-я декада Декабря", u8"3-я декада Декабря"
};

// Функция для вывода std::optional значений
template <typename T>
void print_optional(const std::optional<T>& opt, const std::string& label) 
{
    if (opt.has_value()) 
    {
        std::cout << label << ": " << *opt << "\n";
    }
    else {
        std::cout << label << ": NULL\n";
    }
}

// Структура для хранения ответа с первого API запроса
struct Field
{
public:
    int id;
    int group_id;
    std::optional<std::string> name;
    std::optional<double> calculated_area;

    Field(nlohmann::json jsonData)
    {
        id = jsonData["id"];
        group_id = jsonData["field_group_id"];
        name = jsonData["name"].is_null() ? std::nullopt : std::optional(jsonData["name"].get<std::string>());
        calculated_area = jsonData["calculated_area"].is_null() ? std::nullopt : std::optional(jsonData["calculated_area"].get<double>());
    }

    void print()
    {
        std::cout << "id: " << id << std::endl;
        std::cout << "field_group_id: " << group_id << std::endl;
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
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Field, id, group_id, name.value(), calculated_area.value())
};


struct Measurement 
{
    std::string name;
    std::optional<int> value;      // Некоторые значения измерений — float
    std::optional<std::tm> report_date;

    Measurement()
    {
        this->name = "NULL";
        this->value = 0;
    }

    Measurement(std::optional<std::tm> report_date, Measurement measurement)
    {
        this->name = measurement.name;
        this->value = measurement.value;
        this->report_date = report_date;
    }


};

// Структура для хранения ответа с второго API запроса
struct ScoutReport
{
    int field_id;
    std::optional<std::tm> report_time;
    std::vector<Measurement> measurements = {Measurement(), Measurement(), Measurement(), Measurement()};  // Массив измерений

    // Конструктор для инициализации из JSON
    ScoutReport(const nlohmann::json& jsonData)
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
struct HistoryItem
{
    int field_id;
    std::optional<std::tm> sowing_date;

    HistoryItem(const nlohmann::json& jsonData)
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
struct FieldsGroup
{
    int id;
    std::optional<std::string> name;

    FieldsGroup(const nlohmann::json& jsonData)
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

struct Decade
{
    std::string name;
    std::vector<Measurement> reports;

    Decade()
    {
        this->name = "NULL";
    }

    Decade(std::string name)
    {
        this->name = name;
    }
};

struct TempInstance
{
    std::optional<int> field_id;
    std::optional<std::string> pu;
    std::optional<std::string> field_number;
    std::optional<int> area;
    std::optional<std::tm> sawing_date;
    std::vector<Decade> measurements;

    void print() const
    {
        print_optional(field_id, "Field ID: ");
        print_optional(pu, "pu: ");
        print_optional(field_number, "field_number: ");
        print_optional(area, "area: ");
        print_tm(sawing_date, "sawing_date: ");
        std::cout << "Measuments: " << std::endl;
        for (int i = 0; i < measurements.size(); i++)
        {
            std::cout << "Decade: " << measurements[i].name << std::endl;
            for (int j = 0; j < measurements[i].reports.size(); j++)
            {
                std::cout << "name: " << measurements[i].reports[j].name << std::endl;
                print_optional(measurements[i].reports[j].value, "value: ");
            }
        }
        std::cout << "-------------------------------------\n";
    }
};

struct ResultInstance 
{
    int field_id;
    std::optional<std::string> name;
    std::optional<std::string> field_number;
    std::optional<double> calculated_area;
    std::optional<std::tm> sowing_date;
    std::optional<std::tm> report_time;
    std::vector<Decade> measurements;

    ResultInstance()
    {
        this->name = std::nullopt;
        this->field_number = std::nullopt;
        this->calculated_area = std::nullopt;
        this->sowing_date = std::nullopt;
        this->report_time = std::nullopt;
        for (int i = 0; i < 36; i++)
        {
            measurements.push_back(Decade(Decades[i]));
        }
    }

    void print() const
    {
        if (name.has_value())
        {
            std::cout << "name: " << name.value() << std::endl;
        }
        else
        {
            std::cout << "name: NULL" << std::endl;
        }

        if (field_number.has_value())
        {
            std::cout << "field_number: " << field_number.value() << std::endl;
        }
        else
        {
            std::cout << "field_number: NULL" << std::endl;
        }

        if (calculated_area.has_value())
        {
            std::cout << "calculated_area: " << calculated_area.value() << std::endl;
        }
        else
        {
            std::cout << "calculated_area: NULL" << std::endl;
        }

        if (sowing_date.has_value())
        {
            std::tm sowing_date_tm = sowing_date.value();
            std::cout << "sowing_date: " << std::put_time(&sowing_date_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
        }
        else
        {
            std::cout << "sowing_date: NULL" << std::endl;
        }

        if (report_time.has_value())
        {
            std::tm report_time_tm = report_time.value();
            std::cout << "report_time: " << std::put_time(&report_time_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
        }
        else
        {
            std::cout << "report_time: NULL" << std::endl;
        }

        std::cout << "-------------------------------------\n";
    }
};

