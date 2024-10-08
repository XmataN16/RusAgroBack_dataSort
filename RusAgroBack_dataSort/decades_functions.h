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
			instance.field_number = fields[i].name.value();
		}
		else
		{
			instance.field_number = std::nullopt;
		}
		if (fields[i].calculated_area.has_value())
		{
			instance.area = fields[i].calculated_area.value();
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
			instance.decade_measurements.push_back(DecadeMeasurement(Decades[i]));
		}
		result.push_back(instance);
	}
	
	return result;
}

std::vector<TempInstance> chooseDecade(std::vector<TempInstance>& res, const std::vector<ScoutReport>& scoutReports)
{
    for (const auto& report : scoutReports)
    {
        if (!report.report_time.has_value())
        {
            continue; // ���������� ������ ��� ����
        }

        std::tm date = report.report_time.value();
        TempInstance* currentElement = nullptr;

        // ���� TempInstance � ��������������� field_id
        for (int index_el = 0; index_el < res.size(); index_el++)
        {
            if (res[index_el].field_id == report.field_id)
            {
                currentElement = &res[index_el];
                // ����������� ������
                int month = date.tm_mon;  // ����� �� 0 �� 11
                int day = date.tm_mday;   // ���� ������

                int decadeIndex = -1;
                if (day >= 1 && day <= 10)
                {
                    decadeIndex = month * 3;       // 1-� ������
                }
                else if (day >= 11 && day <= 20)
                {
                    decadeIndex = month * 3 + 1;   // 2-� ������
                }
                else if (day >= 21 && day <= 31)
                {
                    decadeIndex = month * 3 + 2;   // 3-� ������
                }

                // ��������� ������������ ������� ������
                if (decadeIndex >= 0 && decadeIndex < static_cast<int>(currentElement->decade_measurements.size()))
                {
                    // ��� ������ ������� � ������ ������ ������ Measurement � ��������� � reports
                    for (const auto& meas : report.measurements)
                    {
                        Measurement newMeasurement;
                        newMeasurement.name = meas.name;
                        newMeasurement.value = meas.value;

                        // ��������� ����� Measurement � ��������������� ������
                        currentElement->decade_measurements[decadeIndex].report = report;  // �����������, ��� report ������ Measurement
                    }
                }
                else
                {
                    // ��������� ������������� ������� ������, ���� ����������
                }
            }
        }

        if (!currentElement)
        {
            // ���� TempInstance �� ������, ����� ������� ����� ��� ����������
            continue;
        }
    }

    return res;
}

std::vector<ResultInstance> filterMaxReportDate(const std::vector<TempInstance>& data)
{
    std::vector<ResultInstance> result;

    for (const auto& temp : data)
    {
        ResultInstance res;

        // �������� �������� ����
        res.pu = temp.pu;
        res.field_number = temp.field_number.value_or("");
        res.area = temp.area.value_or(0.0f);
        res.sawing_date = temp.sawing_date;

        // ������������ ������ ������
        for (const auto& decade_measurement : temp.decade_measurements)
        {
            ResultDecadeMeasurement rdm;
            rdm.name_decade = decade_measurement.name_decade;

            if (decade_measurement.report.has_value())
            {
                // ��������� ��������� �� ���������� ������
                std::vector<Measurement> measurements = decade_measurement.report->measurements;

                rdm.report = measurements;
            }
            else
            {
                rdm.report = std::nullopt;
            }

            res.decade_measurements.push_back(rdm);
        }

        result.push_back(res);
    }

    return result;
}

std::vector<ResultInstance> calcBiological(std::vector<ResultInstance>& data)
{
    for (auto& instance : data)
    {
        for (auto& decade_measurement : instance.decade_measurements)
        {
            if (decade_measurement.report.has_value())
            {
                auto& measurements = decade_measurement.report.value();

                // ���� ����������� �������
                std::optional<float> density;
                std::optional<float> weight;

                for (const auto& meas : measurements)
                {
                    if (meas.name.has_value())
                    {
                        if (meas.name.value() == u8"�������, ���. ��/��")
                        {
                            density = meas.value;
                        }
                        else if (meas.name.value() == u8"������� ��� �����, �����")
                        {
                            weight = meas.value;
                        }
                    }
                }

                // ��������� ������� ����������� ������
                if (density.has_value() && weight.has_value())
                {
                    double biologicalYield = density.value() * 1000 * (weight.value() / 100000.0);

                    // ������ ����� �������
                    Measurement bioYield;
                    bioYield.name = u8"������������� ����������� �/��";
                    bioYield.value = static_cast<float>(biologicalYield);

                    // ��������� ����� �������
                    decade_measurement.report->push_back(bioYield);
                }
                else
                {
                    std::cerr << u8"����������� ����������� ������ ��� ������� ������������� �����������.\n";
                }
            }
        }
    }

    return data;
}






