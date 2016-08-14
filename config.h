// Pins for LED indicators:
#define A_INDICATOR  0
#define B_INDICATOR  1
#define C_INDICATOR  2
#define D_INDICATOR  3
#define E_INDICATOR  4
#define F_INDICATOR  5
#define G_INDICATOR  6
#define DP_INDICATOR 8

#define SOUND_PIN 11
#define THERMISTOR_PIN A7

#define BLINKING_SEG 1

#define HOUR_CLOCK_SCREENID 0
#define SETTINGS_SCREENID 2
#define MIN_CLOCK_SCREENID 1
#define ALARM_SETUP_SCREENID 3
#define ALARM_ACTION_SCREENID 4
#define ALARM_FAST_SCREENID 5
#define TEMPERATURE_SCREENID 6

// LED groups pins
#define A_GROUP 10
#define B_GROUP 12
#define C_GROUP 13
#define D_GROUP A3

#define SETTINGS_BLINK_DELAY 250
#define SHOWTIME_BLINK_DELAY 500
#define MAX_OFFSET 86400000
#define ALARM_ACTIVE_DELTA 500

#define BEDTEMPTABLE_LEN ()

bool numbers_masks[16][7] = {
  //A  B  C  D  E  F  G
  { 1, 1, 1, 1, 1, 1, 0}, //0 0
  { 0, 1, 1, 0, 0, 0, 0}, //1 1
  { 1, 1, 0, 1, 1, 0, 1}, //2 2
  { 1, 1, 1, 1, 0, 0, 1}, //3 3
  { 0, 1, 1, 0, 0, 1, 1}, //4 4
  { 1, 0, 1, 1, 0, 1, 1}, //5 5 
  { 1, 0, 1, 1, 1, 1, 1}, //6 6
  { 1, 1, 1, 0, 0, 0, 0}, //7 7
  { 1, 1, 1, 1, 1, 1, 1}, //8 8
  { 1, 1, 1, 1, 0, 1, 1}, //9 9

  { 0, 0, 0, 0, 0, 0, 0}, //10 EMPTY
  { 1, 1, 1, 1, 1, 1, 0}, //11 O
  { 1, 1, 1, 0, 1, 1, 0}, //12 N
  { 1, 0, 0, 0, 1, 1, 1}, //13 F
  { 1, 0, 0, 1, 1, 1, 0}, //14 C
  { 0, 0, 0, 0, 0, 0, 1}  //15 MINUS
};
int melody[17][3] = {
  {440, 250, 250},
  {165, 250, 250},
  {440, 250, 500},
  {440, 250, 250},
  {165, 250, 250},
  {440, 250, 500},
  {330, 125, 125},
  {293, 125, 125},
  {262, 125, 124},
  {494, 125, 125},
  {440, 125, 125},
  {494, 125, 125},
  {262, 125, 125},
  {294, 125, 125},
  {330, 250, 250},
  {165, 250, 250},
  {440, 250, 250}
};