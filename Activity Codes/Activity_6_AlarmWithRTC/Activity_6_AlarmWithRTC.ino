#include <TimeLib.h>
#include <DS1307RTC.h>

#define BUZZER_PIN 0       // Connect buzzer to GPIO-0

// EASY ALARM CONFIGURATION - SET YOUR ALARM TIME HERE
const uint8_t ALARM_HOUR = 14;    // 24-hour format (e.g., 7 for 7AM, 15 for 3PM)
const uint8_t ALARM_MINUTE = 58; // Minutes (0-59)

TwoWire Wire(0); // I2C-0

bool parse = false;
bool config = false;
bool alarmActive = false;
unsigned long lastBeepChange = 0;  // For tracking beep pattern timing
uint8_t beepState = 0;             // For tracking position in beep pattern
  
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;
tmElements_t alarmTime;  // To store the alarm time

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communication
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin as output
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
  
  delay(1000);
  Serial.println("RTC Alarm Clock with Buzzer");
  
  // Set RTC time using compiler time
  char timeString[9];
  char dateString[12];
  strcpy(timeString, __TIME__);
  strcpy(dateString, __DATE__);
  
  // get the date and time the compiler was run
  if (getDate(dateString) && getTime(timeString)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  // Set the alarm using predefined hour and minute
  if (config) {
    setAlarmExact(ALARM_HOUR, ALARM_MINUTE);
  }
}

// the loop function runs over and over again forever
void loop() {
  // Read current time from RTC
  if (RTC.read(tm)) {
    displayTime();
    checkAlarm();
  } else {
    Serial.println("Failed to read RTC!");
  }
  
  delay(1000); // Update every second
}

// Display the current time on serial monitor
void displayTime() {
  Serial.print("Current time: ");
  Serial.print(tm.Hour);
  Serial.print(":");
  if (tm.Minute < 10) Serial.print("0");
  Serial.print(tm.Minute);
  Serial.print(":");
  if (tm.Second < 10) Serial.print("0");
  Serial.print(tm.Second);
  Serial.print(" ");
  Serial.print(tm.Day);
  Serial.print(" ");
  Serial.print(monthName[tm.Month-1]);
  Serial.print(" ");
  Serial.println(tmYearToCalendar(tm.Year));
}

// Set alarm to a specific hour and minute
void setAlarmExact(uint8_t hour, uint8_t minute) {
  alarmTime.Hour = hour;
  alarmTime.Minute = minute;
  
  Serial.print("Alarm set for: ");
  Serial.print(alarmTime.Hour);
  Serial.print(":");
  if (alarmTime.Minute < 10) Serial.print("0");
  Serial.println(alarmTime.Minute);
}

// Set alarm to trigger after specified minutes from current time
void setAlarm(int minutesFromNow) {
  if (RTC.read(alarmTime)) {
    // Set alarm time based on current time + minutesFromNow
    alarmTime.Minute += minutesFromNow;
    if (alarmTime.Minute >= 60) {
      alarmTime.Minute -= 60;
      alarmTime.Hour += 1;
      if (alarmTime.Hour >= 24) {
        alarmTime.Hour -= 24;
      }
    }
    
    Serial.print("Alarm set for: ");
    Serial.print(alarmTime.Hour);
    Serial.print(":");
    if (alarmTime.Minute < 10) Serial.print("0");
    Serial.println(alarmTime.Minute);
  }
}

// Check if current time matches alarm time
void checkAlarm() {
  // Check if alarm should trigger (match hour and minute)
  if (tm.Hour == alarmTime.Hour && tm.Minute == alarmTime.Minute) {
    if (!alarmActive) {
      alarmActive = true;
      Serial.println("ALARM TRIGGERED!");
      lastBeepChange = millis(); // Initialize beep timing
      beepState = 0;            // Start pattern from beginning
    }
    // Sound the buzzer with pattern
    soundAlarmPattern();
  } else {
    // Turn off buzzer and reset alarm flag if time no longer matches
    if (alarmActive) {
      alarmActive = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

// Create a more complex alarm sound pattern
void soundAlarmPattern() {
  // SOS pattern (... --- ...) with timing
  // Short beeps are 100ms, long beeps are 300ms, pauses are 100ms
  // Sequence pause is 500ms
  
  const uint8_t PATTERN_LENGTH = 19;  // Total states in pattern
  const uint16_t timings[PATTERN_LENGTH] = {
    100, 100, 100, 100, 100, 100, // Three short beeps (S)
    300, 100, 300, 100, 300,      // Three long beeps (O)
    100, 100, 100, 100, 100, 100, // Three short beeps (S)
    500, 500                      // Longer pause before repeating
  };
  
  const bool states[PATTERN_LENGTH] = {
    HIGH, LOW, HIGH, LOW, HIGH, LOW, // Three short beeps (S)
    HIGH, LOW, HIGH, LOW, HIGH,      // Three long beeps (O)
    HIGH, LOW, HIGH, LOW, HIGH, LOW, // Three short beeps (S)
    LOW, LOW                         // Longer pause before repeating
  };
  
  unsigned long currentTime = millis();
  
  if ((currentTime - lastBeepChange) > timings[beepState]) {
    beepState = (beepState + 1) % PATTERN_LENGTH;
    digitalWrite(BUZZER_PIN, states[beepState]);
    lastBeepChange = currentTime;
  }
}

// Original simple alarm pattern (kept for reference)
void soundAlarm() {
  // Create a beeping pattern for the alarm
  if ((tm.Second % 2) == 0) {
    // On during even seconds
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    // Off during odd seconds
    digitalWrite(BUZZER_PIN, LOW);
  }
}

bool getTime(char *str) {
  // get the time by seperating the string:
  char* time[3];
  int count = 0;
  char* token = strtok(str, ":");
  while (token != NULL) {
    time[count] = token;
    count++;
    token = strtok(NULL, ":");
  }
  
  if(count == 3) {
    tm.Hour = atoi(time[0]);
    tm.Minute = atoi(time[1]);
    tm.Second = atoi(time[2]); 
    return true;
  }
  return false;
}

bool getDate(char *str) {
  // get the date by seperating the string:
  char* date[3];
  uint8_t monthIndex;

  int count = 0;
  char* token = strtok(str, " ");
  while (token != NULL) {
    date[count] = token;
    count++; 
    token = strtok(NULL, " ");
  }
  if(count != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(date[0], monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = atoi(date[1]);
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(atoi(date[2]));
  return true;
}
