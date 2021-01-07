#include <Arduino.h>

#define SHIFT_DATA 0
#define SHIFT_LATCH 1
#define SHIFT_CLK 2

#define BUZZER 4
#define BUTTON 3

#define STATE_DEFAULT 0
#define STATE_DEFAULT_1 1
#define STATE_RUNNING 2
#define STATE_RUNNING_1 3
#define STATE_BUZZING 4

#define STATE_SETTING_SECONDS 50
#define STATE_SETTING_SECONDS_1 51
#define STATE_SETTING_SECONDS_2 52
#define STATE_SETTING_MINUTES 60
#define STATE_SETTING_MINUTES_1 61
#define STATE_SETTING_MINUTES_2 62


#define LED_ARRAY_DOT 8
#define BUTTON_DEBOUNCE_MS 100
#define EDIT_START_THRESHOLD 1000
#define EDIT_TIMEOUT 10000

const uint8_t digits[] =  {
  246,
  132,
  117,
  213,
  135,
  211,
  243,
  148,
  247,
  215
};

void shiftSegments(uint8_t segments, uint8_t groundMask) {
  for (uint8_t i = 0; i < 16; i++)  {
    switch (i) {
      case 0:
      case 1:
      case 2: 
        digitalWrite(SHIFT_DATA, !!(segments & (1 << i)));
        break;
      case 3:
      case 4:
        digitalWrite(SHIFT_DATA, !(groundMask & (1 << (i-3))));
        break;
      case 8:
      case 9:
        digitalWrite(SHIFT_DATA, !(groundMask & (1 << (i-8+2))));
        break;
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
        digitalWrite(SHIFT_DATA, !!(segments & (1 << (i-10+3))));
        break;      
    }
    digitalWrite(SHIFT_CLK, HIGH);
    digitalWrite(SHIFT_CLK, LOW);
  }
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void writeNumber(uint16_t number) {
  shiftSegments(digits[number % 10], 0b0001);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(number == 0 ? 0 : digits[number % 10], 0b0010);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(number == 0 ? 0 : digits[number % 10], 0b0100);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(number == 0 ? 0 : digits[number % 10], 0b1000);
  delayMicroseconds(200);
} 

void writeDuration(uint32_t milliseconds, uint8_t mask)
{
  int seconds = ((milliseconds+500) / 1000) % 60; // round up
  int minutes = ((milliseconds+500) / 60000);

  shiftSegments(digits[seconds % 10], 0b0001 & mask);
  delayMicroseconds(200);
  shiftSegments(digits[seconds/10 % 10], 0b0010 & mask);
  delayMicroseconds(200);
  shiftSegments(digits[minutes % 10] + LED_ARRAY_DOT, 0b0100 & mask);
  delayMicroseconds(200);
  shiftSegments(digits[minutes/10 % 10], 0b1000 & mask);
  delayMicroseconds(200);
  // This is to ensure even brightness of all four digits if we take a long time
  // after this function is called.
  shiftSegments(0, 0b0000);
}

void turnOffDisplay() {
  shiftSegments(0, 0b0000);
} 

void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

uint8_t state = 0;
uint32_t startedAt = 0;
uint32_t lastInteraction = 0;

uint32_t coffeeTime = 180000;
uint8_t editMinutes = 3;
uint8_t editSeconds = 0;

bool shortBeep = false;
uint32_t shortBeepStart = 0;

#define CHECK_BUTTON_CUSTOM(shouldBePressed, timeout) \
  (buttonPressed ^ (shouldBePressed) && currentTime - lastInteraction > timeout && ((lastInteraction = currentTime), true))
#define BUTTON_UP CHECK_BUTTON_CUSTOM(false, BUTTON_DEBOUNCE_MS)
#define BUTTON_DOWN CHECK_BUTTON_CUSTOM(true, BUTTON_DEBOUNCE_MS)

#define EDITED_TIME (((uint32_t)editMinutes*60 + editSeconds) * 1000)

#define SHORT_BEEP do { shortBeep = true; shortBeepStart = currentTime; } while (false)

void loop() {
  uint32_t currentTime = millis();
  bool buttonPressed = digitalRead(BUTTON);

  // State transitions
  switch (state) {
    case STATE_DEFAULT_1:
      if (BUTTON_UP) {
        state = STATE_DEFAULT;
      }
      break;    
    case STATE_DEFAULT:
      if (BUTTON_DOWN) {
        state = STATE_RUNNING_1;
      } 
      break;
    case STATE_RUNNING_1:
      if (BUTTON_UP) {
        SHORT_BEEP;
        startedAt = currentTime;
        state = STATE_RUNNING;
      }
      else if (CHECK_BUTTON_CUSTOM(true, 1000)) {
        editSeconds = 0;
        editMinutes = 0;
        state = STATE_SETTING_MINUTES_1;
      }
      break;  
    case STATE_SETTING_MINUTES_1: 
      if (BUTTON_UP) {
        state = STATE_SETTING_MINUTES;
      }
      break;
    case STATE_SETTING_MINUTES:
      if (BUTTON_DOWN) {
        state = STATE_SETTING_MINUTES_2;
      } else if (currentTime - lastInteraction > EDIT_TIMEOUT) {
        state = STATE_DEFAULT;
      }
      break;   
    case STATE_SETTING_MINUTES_2:
      if (BUTTON_UP) {
        SHORT_BEEP;
        editMinutes = (editMinutes + 1) % 100;
        state = STATE_SETTING_MINUTES;
      }
      else if (CHECK_BUTTON_CUSTOM(true, EDIT_START_THRESHOLD)) {
        SHORT_BEEP;
        state = STATE_SETTING_SECONDS_1;
      }
      break; 
    case STATE_SETTING_SECONDS_1: 
      if (BUTTON_UP) {
        state = STATE_SETTING_SECONDS;
      }
      break;
    case STATE_SETTING_SECONDS:
      if (BUTTON_DOWN) {
        state = STATE_SETTING_SECONDS_2;
      } else if (currentTime - lastInteraction > EDIT_TIMEOUT) {
        state = STATE_DEFAULT;
      }
      break;   
    case STATE_SETTING_SECONDS_2:
      if (BUTTON_UP) {
        SHORT_BEEP;
        editSeconds = (editSeconds + 1) % 60;
        state = STATE_SETTING_SECONDS;
      }
      else if (CHECK_BUTTON_CUSTOM(true, EDIT_START_THRESHOLD)) {
        SHORT_BEEP;
        coffeeTime = EDITED_TIME;
        state = STATE_DEFAULT_1;
      }
      break;  
    case STATE_RUNNING:
      if (BUTTON_DOWN) {
        state = STATE_DEFAULT_1;
      }
      else if (currentTime - startedAt > coffeeTime) {
        state = STATE_BUZZING;
        startedAt = currentTime;
      }
      break;      
    case STATE_BUZZING:
      if (BUTTON_DOWN) {
        state = STATE_DEFAULT_1;
      } 
      else if (currentTime - startedAt > 10000) {
        state = STATE_DEFAULT;
      }
      break;
            
  }

  if (shortBeep) {
    digitalWrite(BUZZER, 1);
    if (currentTime - shortBeepStart > 100) {
      digitalWrite(BUZZER, 0);
      shortBeep = false;
    }
  }

 // Display
 switch (state) {
    case STATE_DEFAULT:
    case STATE_DEFAULT_1:
      turnOffDisplay();
      if (!shortBeep) {
        digitalWrite(BUZZER, 0);
      }
      break;
    case STATE_SETTING_SECONDS:
    case STATE_SETTING_SECONDS_1:
    case STATE_SETTING_SECONDS_2:
      writeDuration(EDITED_TIME, (currentTime / 300) % 2 ? 0b1100 : 0b1111);
      break;
    case STATE_SETTING_MINUTES:
    case STATE_SETTING_MINUTES_1:
    case STATE_SETTING_MINUTES_2:
      writeDuration(EDITED_TIME, (currentTime / 300) % 2 ? 0b0011 : 0b1111);
      break;
    case STATE_RUNNING_1:
      writeDuration(coffeeTime, 0b1111);
      break;
    case STATE_RUNNING:
      writeDuration(coffeeTime - (currentTime - startedAt), 0b1111);
      break;      
    case STATE_BUZZING:
      if ((currentTime / 150) % 2) {
        digitalWrite(BUZZER, 1);
        writeDuration(0, 0b1111);
      } else {
        digitalWrite(BUZZER, 0);
        turnOffDisplay();
      }
      break;            
  }  
}