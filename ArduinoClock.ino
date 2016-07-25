#include "config.h"

long currentTime = 0;
long alarmTime = 0;
long userTimeOffset = 0;
bool alarmActive = false;

byte currentScreen = 0;

byte indicators[] = { A_INDICATOR, B_INDICATOR, C_INDICATOR, D_INDICATOR, E_INDICATOR, F_INDICATOR, G_INDICATOR, DP_INDICATOR};
byte groups[] = {A_GROUP, B_GROUP, C_GROUP, D_GROUP};

void (*screen_array[7])(void);

void setup()
{
  screen_array[HOUR_CLOCK_SCREENID] = hourClockScreen;
  screen_array[MIN_CLOCK_SCREENID] = minClockScreen;
  screen_array[SETTINGS_SCREENID] = settingsScreen;
  screen_array[ALARM_SETUP_SCREENID] = alarmSetupScreen;
  screen_array[ALARM_ACTION_SCREENID] = alarmActionScreen;
  screen_array[ALARM_FAST_SCREENID] = alarmFastScreen;
  screen_array[TEMPERATURE_SCREENID] = temperatureScreen;
  
  for (int i = 0; i < 8; i++)
  {
    if (i < 4) pinMode(groups[i], OUTPUT);
    pinMode(indicators[i], OUTPUT);
  }
  pinMode(SOUND_PIN, OUTPUT);
  clear();
}
void loop()
{
  currentTime = (millis() % MAX_OFFSET + userTimeOffset) % MAX_OFFSET; //Time initialization and normalization (overflow safe)

  handleAlarm();
  handleFastScreens();

  screen_array[currentScreen](); //Show current screen
}

void settingsScreen()
{
  static byte state = 0;

  if (isButtonDown(0))
  {
    if (++state == 2)
    {
      state = 0;
      currentScreen = HOUR_CLOCK_SCREENID;
      return;
    }
  }

  if (isButtonDown(2))
  {
    if (state == 0)
      userTimeOffset = (userTimeOffset - MAX_OFFSET / 24) % MAX_OFFSET;
    else userTimeOffset = (userTimeOffset - 60000) % MAX_OFFSET;
  }
  if (isButtonDown(3))
  {
    if (state == 0)
      userTimeOffset = (userTimeOffset + MAX_OFFSET / 24) % MAX_OFFSET;
    else userTimeOffset = (userTimeOffset + 60000) % MAX_OFFSET;
  }

  if (userTimeOffset < 0)
  {
    userTimeOffset = MAX_OFFSET + userTimeOffset;
  }

  int mins = getMins(currentTime);
  int hours = getHours(currentTime);

  byte groupsData[] = {
    state == 0 && blinking(SETTINGS_BLINK_DELAY) ? 10 : hours / 10,
    state == 0 && blinking(SETTINGS_BLINK_DELAY) ? 10 : hours % 10,
    state == 1 && blinking(SETTINGS_BLINK_DELAY) ? 10 : mins / 10,
    state == 1 && blinking(SETTINGS_BLINK_DELAY) ? 10 : mins % 10
  };

  displayClock(groupsData, 0);
}
void minClockScreen()
{
  int seconds = getSecs(currentTime);
  int mins = getMins(currentTime);

  byte groupsData[] = { mins / 10, mins % 10, seconds / 10, seconds % 10 };

  displayClock(groupsData, SHOWTIME_BLINK_DELAY);

  if (isButtonDown(0))
    currentScreen = SETTINGS_SCREENID;
  else if (isButtonDown(1))
    currentScreen = ALARM_SETUP_SCREENID;
  else if (isButtonDown(2))
    currentScreen = TEMPERATURE_SCREENID;
}
void hourClockScreen()
{
  int mins = getMins(currentTime);
  int hours = getHours(currentTime);

  byte groupsData[] = { hours / 10, hours % 10, mins / 10, mins % 10 };

  displayClock(groupsData, SHOWTIME_BLINK_DELAY);

  if (isButtonDown(0))
    currentScreen = SETTINGS_SCREENID;
  else if (isButtonDown(1))
    currentScreen = ALARM_SETUP_SCREENID;
  else if (isButtonDown(2))
    currentScreen = MIN_CLOCK_SCREENID;
}
void alarmSetupScreen()
{
  static byte state = 0;

  if (isButtonDown(1))
  {
    if (++state == 3)
    {
      state = 0;
      currentScreen = HOUR_CLOCK_SCREENID;
      return;
    }
  }

  if (isButtonDown(2))
  {
    if (state == 0)
      alarmTime = (alarmTime - MAX_OFFSET / 24) % MAX_OFFSET;
    else if (state == 1)
      alarmTime = (alarmTime - 60000) % MAX_OFFSET;
    else alarmActive = !alarmActive;
  }
  if (isButtonDown(3))
  {
    if (state == 0)
      alarmTime = (alarmTime + MAX_OFFSET / 24) % MAX_OFFSET;
    else if (state == 1)
      alarmTime = (alarmTime + 60000) % MAX_OFFSET;
    else alarmActive = !alarmActive;
  }

  if (alarmTime < 0)
  {
    alarmTime = MAX_OFFSET + alarmTime;
  }

  int mins = getMins(alarmTime);
  int hours = getHours(alarmTime);

  if (state == 0)
  {
    byte groupsData[] = {
      blinking(SETTINGS_BLINK_DELAY) ? 10 : hours / 10,
      blinking(SETTINGS_BLINK_DELAY) ? 10 : hours % 10,
      mins / 10,
      mins % 10
    };
    displayClock(groupsData, 0);
  }
  else if (state == 1)
  {
    byte groupsData[] = {
      hours / 10,
      hours % 10,
      blinking(SETTINGS_BLINK_DELAY) ? 10 : mins / 10,
      blinking(SETTINGS_BLINK_DELAY) ? 10 : mins % 10
    };
    displayClock(groupsData, 0);
  }
  else if (alarmActive)
  {
    byte groupsData[] = { //ON
      10,
      10,
      11,
      12
    };
    bool mask[] = {0, 0, 0, 0};
    displayLED(groupsData, mask);
  }
  else
  {
    byte groupsData[] = {  //OFF
      10,
      11,
      13,
      13
    };
    bool mask[] = {0, 0, 0, 0};
    displayLED(groupsData, mask);
  }
}
void alarmActionScreen()
{
  if (isAnyButtonDown())
  {
    currentScreen = HOUR_CLOCK_SCREENID;
    alarmActive = false;
  }

  static long nextTone = 0;
  static int note_index = 0;

  if (currentTime >= nextTone)
  {
    tone(SOUND_PIN, melody[note_index][0], melody[note_index][1]);
    nextTone = currentTime + melody[note_index++][2];
  }

  if (note_index >= 17) note_index = 0;

  byte groupData[4];
  bool mask[] = {0, 0, 0, 0};
  if (blinking(100))
  {
    byte groupData[] = { 10, 10, 10, 10};
    displayLED(groupData, mask);
  }
  else
  {
    byte groupData[] = { random(0, 10), random(0, 10), random(0, 10), random(0, 10) };
    displayLED(groupData, mask);
  }
}
void alarmFastScreen()
{
  int mins = getMins(alarmTime);
  int hours = getHours(alarmTime);

  byte groupsData[] = { hours / 10, hours % 10, mins / 10, mins % 10 };

  displayClock(groupsData, 0);

  if (isButtonDown(0))
    currentScreen = SETTINGS_SCREENID;
  else if (isButtonDown(1))
    currentScreen = ALARM_SETUP_SCREENID;
  else if (isButtonDown(2))
    currentScreen = MIN_CLOCK_SCREENID;
}
void temperatureScreen()
{
  if (isButtonDown(2))
    currentScreen = HOUR_CLOCK_SCREENID;

  double t = getTemperature();

  byte data[] = {0, 0, 0, 14};
  
  if (t >= 0)
  {
    
  }
  else
  {
    
  }
  
  bool mask[] = {0, 0, 0, 1};

  displayLED(data, mask);
}

inline void handleAlarm()
{
  long deltaTime = currentTime - alarmTime;
  if (alarmActive && currentScreen != ALARM_SETUP_SCREENID && deltaTime >= 0 && deltaTime <= ALARM_ACTIVE_DELTA)
  {
    currentScreen = ALARM_ACTION_SCREENID;
  }
}
inline void handleFastScreens()
{
  static int lastScreenID = 0;
  if (currentScreen == SETTINGS_SCREENID || currentScreen == ALARM_SETUP_SCREENID) return;

  
  if (isButton(3))
  {
    if (currentScreen != ALARM_FAST_SCREENID)
    {
      lastScreenID = currentScreen;
      currentScreen = ALARM_FAST_SCREENID;
    }
  }
  else if (currentScreen == ALARM_FAST_SCREENID)
  {
    currentScreen = lastScreenID;
  }
}

