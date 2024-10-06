#pragma once

std::vector<TempInstance> calc_result(std::vector<Field> fields, std::vector<HistoryItem> historyItems, std::vector<FieldsGroup> fieldsGroup)
{
	std::vector<TempInstance> result;
	for (int i = 0; i < fields.size(); i++)
	{
		TempInstance instance;

		instance.field_id = fields[i].id;

		for (int obj = 0; obj < fieldsGroup.size(); obj++)
		{
			if ((fieldsGroup[obj].id == fields[obj].group_id) and fieldsGroup[obj].name.has_value())
			{
				instance.pu = fieldsGroup[obj].name.value();
			}
		}
		if (fields[i].name.has_value())
		{
			instance.field_number = fields[i].name;
		}
		else
		{
			instance.field_number = std::nullopt;
		}
		if (fields[i].calculated_area.has_value())
		{
			instance.area = fields[i].calculated_area;
		}
		else
		{
			instance.area = std::nullopt;
		}
		for (int obj = 0; obj < historyItems.size(); obj++)
		{
			if (instance.field_id == historyItems[obj].field_id and historyItems[obj].sowing_date.has_value())
			{
				instance.sawing_date = historyItems[obj].sowing_date;
			}
		}
		for (int i = 0; i < 36; i++)
		{
			instance.measurements.push_back(Decade(Decades[i]));
		}
		result.push_back(instance);
	}
	
	return result;
}

void chooseDecade(std::vector<TempInstance>& res, const std::vector<ScoutReport>& scoutReports)
{
    for (const auto& report : scoutReports)
    {
        if (!report.report_time.has_value()) 
        {
            continue; // Пропускаем отчёты без даты
        }

        std::tm date = report.report_time.value();
        TempInstance* currentElement = nullptr;

        // Ищем TempInstance с соответствующим field_id
        for (auto& element : res)
        {
            if (element.field_id.has_value() && element.field_id.value() == report.field_id)
            {
                currentElement = &element;
                break;
            }
        }

        if (!currentElement)
        {
            // Если TempInstance не найден, можно создать новый или пропустить
            continue;
        }

        // Определение декады
        int month = date.tm_mon;  // Месяц от 0 до 11
        int day = date.tm_mday;   // День месяца

        int decadeIndex = -1;
        if (day >= 1 && day <= 10)
        {
            decadeIndex = month * 3;       // 1-я декада
        }
        else if (day >= 11 && day <= 20)
        {
            decadeIndex = month * 3 + 1;   // 2-я декада
        }
        else if (day >= 21 && day <= 31)
        {
            decadeIndex = month * 3 + 2;   // 3-я декада
        }

        // Проверяем корректность индекса декады
        if (decadeIndex >= 0 && decadeIndex < static_cast<int>(currentElement->measurements.size()))
        {
            // Для каждой метрики в отчёте создаём объект Measurement и добавляем в reports
            for (const auto& meas : report.measurements)
            {
                Measurement newMeasurement;
                newMeasurement.name = meas.name;
                newMeasurement.value = meas.value;
                newMeasurement.report_date = date;
                currentElement->measurements[decadeIndex].reports.push_back(newMeasurement);
            }
        }
        else
        {
            // Обработка некорректного индекса декады, если необходимо
        }
    }
}