#pragma once

std::string Decades[36] = { u8"1-� ������ ������", u8"2-� ������ ������", u8"3-� ������ ������", u8"1-� ������ �������", u8"2-� ������ �������", u8"3-� ������ �������",
    u8"1-� ������ �����", u8"2-� ������ �����", u8"3-� ������ �����", u8"1-� ������ ������", u8"2-� ������ ������", u8"3-� ������ ������",
    u8"1-� ������ ���", u8"2-� ������ ���", u8"3-� ������ ���", u8"1-� ������ ����", u8"2-� ������ ����", u8"3-� ������ ����",
    u8"1-� ������ ����", u8"2-� ������ ����", u8"3-� ������ ����", u8"1-� ������ �������", u8"2-� ������ �������", u8"3-� ������ �������",
    u8"1-� ������ ��������", u8"2-� ������ ��������", u8"3-� ������ ��������", u8"1-� ������ �������", u8"2-� ������ �������", u8"3-� ������ �������",
    u8"1-� ������ ������", u8"2-� ������ ������", u8"3-� ������ ������", u8"1-� ������ �������", u8"2-� ������ �������", u8"3-� ������ �������"
};

// ������� ��� ������ std::optional ��������
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

// ��������� ��� �������� ������ � ������� API �������
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

    // ���������� ��� ������������� � JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Field, id, group_id, name.value(), calculated_area.value())
};


struct Measurement 
{
    std::string name;
    std::optional<int> value;      // ��������� �������� ��������� � float
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

// ��������� ��� �������� ������ � ������� API �������
struct ScoutReport
{
    int field_id;
    std::optional<std::tm> report_time;
    std::vector<Measurement> measurements = {Measurement(), Measurement(), Measurement(), Measurement()};  // ������ ���������

    // ����������� ��� ������������� �� JSON
    ScoutReport(const nlohmann::json& jsonData)
    {
        field_id = jsonData["field_id"];
        report_time = jsonData["report_time"].is_null() ? std::nullopt : str_to_tm(std::optional(jsonData["report_time"]));

        // ������� ������� measurements
        for (const auto& measurementJson : jsonData["measurements"]) 
        {
            Measurement measurement;
            //std::cout << measurementJson["name"] << std::endl;
            if (measurementJson["name"] == u8"������� ��� �����, �����")
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
            else if (measurementJson["name"] == u8"�������, ���. ��/��")
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
            else if (measurementJson["name"] == u8"�������� �����, %")
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

        // ������� �����������

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

// ��������� ��� �������� ������ � �������� API �������
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

// ��������� ��� �������� ������ � ���������� API �������
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

