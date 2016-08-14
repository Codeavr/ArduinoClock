void displayClock(byte *groupData, int blinkDelay)
{
	bool points[] = { 0, blinking(blinkDelay), 0, 0 };
	displayLED(groupData, points);
}
void displayLED(byte *groupsData, bool *points)
{
	for (int i = 0; i < 4; i++)
	{
		setIndicators(numbers_masks[groupsData[i]]);

		if (points[i])
		{
			digitalWrite(DP_INDICATOR, HIGH);
		}
		else digitalWrite(DP_INDICATOR, LOW);

		digitalWrite(groups[i], HIGH);
		delay(1);
		digitalWrite(groups[i], LOW);
	}
}
inline void setIndicators(bool *mask)
{
	for (int i = 0; i < 7; i++)
	{
		digitalWrite(indicators[i], mask[i]);
	}
}
inline void clear()
{
	for (int i = 0; i < 8; i++)
	{
		digitalWrite(indicators[i], LOW);
	}
}
inline double getTemperature()
{
	double t = log(10000.0 * ((1023.0 / analogRead(THERMISTOR_PIN) - 1)));
	t = 1.0 / (0.001129148 + (0.000234125 + (0.0000000876741 * t * t)) * t);
	return t - 273.15;
	//double R = 1024 / analogRead(THERMISTOR_PIN);
	//return 14.156 * R * R * R - 67.858 * R*R + 114.378 * R - 45.438;
}
bool isButton(byte group)
{
	pinMode(groups[group], INPUT);

	bool result = digitalRead(groups[group]);

	pinMode(groups[group], OUTPUT);

	return result;
}
bool isButtonDown(byte group)
{
	static bool buttonStates[] = { 0, 0, 0, 0 };

	pinMode(groups[group], INPUT);

	if (digitalRead(groups[group]))
	{
		if (!buttonStates[group])
		{
			buttonStates[group] = true;
			return true;
		}
	}
	else buttonStates[group] = false;

	pinMode(groups[group], OUTPUT);

	return false;
}
bool isAnyButtonDown()
{
	bool anyKeyPressed = false;
	for (int i = 0; i < 4; i++)
	{
		if (isButtonDown(i))
		{
			anyKeyPressed = true;
			break;
		}
	}
	return anyKeyPressed;
}
