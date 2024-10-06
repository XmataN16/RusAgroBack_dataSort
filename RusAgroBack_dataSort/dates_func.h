#pragma once
#include "cmath"
//Функция создания объекта даты
std::tm create_date(int year, int month, int day) 
{
    std::tm date;
    std::memset(&date, 0, sizeof(date)); // Очищаем структуру (обнуляем поля)

    date.tm_year = year - 1900; // Год с 1900
    date.tm_mon = month - 1;    // Месяц (январь = 0, февраль = 1, ..., декабрь = 11)
    date.tm_mday = day;         // День месяца

    return date;
}

//Функция сравнения двух дат
std::optional<std::tm> max_date(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = mktime(const_cast<std::tm*>(&date2));

    if (difftime(time1, time2) > 0)
    {
        return date1;
    }
    else
    {
        return date2;
    }
}

//Функция сравнения двух дат
std::optional<std::tm> min_date(const std::optional<std::tm>& date1, const std::optional<std::tm>& date2)
{
    if (!date1.has_value() and !date2.has_value())
    {
        return std::nullopt;
    }
    else if (date1.has_value() and !date2.has_value())
    {
        return date1;
    }
    else if (!date1.has_value() and date2.has_value())
    {
        return date2;
    }
    else
    {
        std::time_t time1 = mktime(const_cast<std::tm*>(&date1.value()));
        std::time_t time2 = mktime(const_cast<std::tm*>(&date2.value()));
        if (difftime(time1, time2) > 0)
        {
            return date2;
        }
        else
        {
            return date1;
        }
    }
}

bool max_date_bool(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = std::mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = std::mktime(const_cast<std::tm*>(&date2));

    return std::difftime(time1, time2) > 0;
}

// Оператор "больше" для двух std::optional<std::tm>
bool operator > (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Оператор "меньше" для двух std::optional<std::tm>
bool operator < (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Оператор "меньше или равно" для двух std::optional<std::tm>
bool operator <= (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Оператор "больше или равно" для двух std::optional<std::tm>
bool operator >= (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Оператор "равно" для двух std::optional<std::tm>
bool operator == (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // Преобразуем std::tm в time_t для сравнения
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Функция для нахождения максимальной даты среди трёх дат с учетом значения nullopt
std::optional<std::tm> custom_max(const std::vector<std::optional<std::tm>>& dates)
{
    if (!dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return std::nullopt;
    }
    else if (dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] > dates[1])
        {
            if (dates[0] > dates[2])
            {
                return dates[0];
            }
            else
            {
                return dates[2];
            }
        }
        else
        {
            if (dates[1] > dates[2])
            {
                return dates[1];
            }
            else
            {
                return dates[2];
            }
        }
    }
    else if (dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        if (dates[0] > dates[1])
        {
            return dates[0];
        }
        else
        {
            return dates[1];
        }
    }
    else if (dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] > dates[2])
        {
            return dates[0];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[1] > dates[2])
        {
            return dates[1];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        return dates[2];
    }
    else if (!dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        return dates[1];
    }
    else if (dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return dates[0];
    }
}

// Функция для нахождения минимальной даты среди трёх дат с учетом значения nullopt
std::optional<std::tm> custom_min(const std::vector<std::optional<std::tm>>& dates)
{
    if (!dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return std::nullopt;
    }
    else if (dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] < dates[1])
        {
            if (dates[0] < dates[2])
            {
                return dates[0];
            }
            else
            {
                return dates[2];
            }
        }
        else
        {
            if (dates[1] < dates[2])
            {
                return dates[1];
            }
            else
            {
                return dates[2];
            }
        }
    }
    else if (dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        if (dates[0] < dates[1])
        {
            return dates[0];
        }
        else
        {
            return dates[1];
        }
    }
    else if (dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] < dates[2])
        {
            return dates[0];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[1] < dates[2])
        {
            return dates[1];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        return dates[2];
    }
    else if (!dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        return dates[1];
    }
    else if (dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return dates[0];
    }
}

// Преобразование строки в std::tm
std::optional<std::tm> str_to_tm(const std::optional<std::string>& str)
{
    if (str.has_value())
    {
        std::tm tm = {};  // Инициализируем структуру std::tm
        std::istringstream ss(str.value());
        ss >> std::get_time(&tm, "%Y-%m-%d");  // Парсим строку в объект tm
        if (ss.fail()) 
        {
            return std::nullopt;  // Если произошла ошибка при парсинге
        }
        return tm;  // Возвращаем std::tm внутри std::optional
    }
    else
    {
        return std::nullopt;
    }
}

// Преобразование std::tm обратно в строку
std::optional<std::string> tm_to_str(const std::optional<std::tm>& tm)
{
    if (tm.has_value())
    {
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm.value());
        return std::string(buffer);
    }
    else
    {
        return std::nullopt;
    }
}

// Функция для вывода даты
void print_tm(const std::optional<std::tm>& date, const std::string& label) 
{
    if (date.has_value()) 
    {
        std::cout << label << ": " << std::put_time(&*date, "%Y-%m-%d") << "\n";
    }
    else 
    {
        std::cout << label << ": NULL\n";
    }
}