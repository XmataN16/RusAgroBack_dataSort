#pragma once
// Структура для хранения информации о поле
struct Field
{
public:
    int id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<double> calculated_area;
    std::optional<double> legal_area;
    std::optional<double> tillable_area;
    std::optional<double> lat;
    std::optional<double> lon;
    std::optional<std::tm> created_at;
    std::optional<std::tm> updated_at;

    Field(nlohmann::json jsonData)
    {
        this->id = jsonData["id"];
        this->name = jsonData["name"].is_null() ? std::nullopt : std::optional(jsonData["name"].get<std::string>());
        this->description = jsonData["description"].is_null() ? std::nullopt : std::optional(jsonData["description"].get<std::string>());
        this->calculated_area = jsonData["calculated_area"].is_null() ? std::nullopt : std::optional(jsonData["calculated_area"].get<double>());
        this->legal_area = jsonData["legal_area"].is_null() ? std::nullopt : std::optional(jsonData["legal_area"].get<double>());
        this->tillable_area = jsonData["tillable_area"].is_null() ? std::nullopt : std::optional(jsonData["tillable_area"].get<double>());
        this->lat = jsonData["lat"].is_null() ? std::nullopt : std::optional(jsonData["lat"].get<double>());
        this->lon = jsonData["lon"].is_null() ? std::nullopt : std::optional(jsonData["lon"].get<double>());
        this->created_at = jsonData["created_at"].is_null() ? std::nullopt : str_to_tm(std::optional(jsonData["created_at"]));
        this->updated_at = jsonData["updated_at"].is_null() ? std::nullopt : str_to_tm(std::optional(jsonData["updated_at"]));
    }

    void print()
    {

        std::cout << "id: " << this->id << std::endl;
        if (this->name.has_value())
        {
            std::cout << "name: " << this->name.value() << std::endl;
        }
        else
        {
            std::cout << "name: NULL \n";
        }
        if (this->description.has_value())
        {
            std::cout << "descriptuion: " << this->description.value() << std::endl;
        }
        else
        {
            std::cout << "description: NULL \n";
        }
        if (this->calculated_area.has_value())
        {
            std::cout << "calculated_area: " << this->calculated_area.value() << std::endl;
        }
        else
        {
            std::cout << "calculated_area: NULL \n";
        }
        if (this->legal_area.has_value())
        {
            std::cout << "legal_area: " << this->legal_area.value() << std::endl;
        }
        else
        {
            std::cout << "legal_area: NULL \n";
        }
        if (this->tillable_area.has_value())
        {
            std::cout << "tillable_area: " << this->tillable_area.value() << std::endl;
        }
        else
        {
            std::cout << "tillable_area: NULL \n";
        }
        if (this->lat.has_value())
        {
            std::cout << "lat: " << this->lat.value() << std::endl;
        }
        else
        {
            std::cout << "lat: NULL \n";
        }
        if (this->lon.has_value())
        {
            std::cout << "lon: " << this->lon.value() << std::endl;
        }
        else
        {
            std::cout << "lon: NULL \n";
        }
        if (created_at.has_value())
        {
            std::tm tm_date = created_at.value();
            char buffer[80];
            strftime(buffer, 80, "%Y-%m-%d", &tm_date);
            std::cout << "created_at: " << buffer << "\n";
        }
        else
            std::cout << "created_at: NULL\n";
        if (updated_at.has_value())
        {
            std::tm tm_date = updated_at.value();
            char buffer[80];
            strftime(buffer, 80, "%Y-%m-%d", &tm_date);
            std::cout << "updated_at: " << buffer << "\n";
        }
        else
            std::cout << "updated_at: NULL\n";

        std::cout << "-------------------------------------\n";
    }

    // Декларация для сопоставления с JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Field, id, name.value(), description.value(), calculated_area.value(), legal_area.value(), tillable_area.value(), lat.value(), lon.value())
};