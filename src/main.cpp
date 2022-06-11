#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>

#define SHIFT_DATA 0
#define SHIFT_LATCH 2
#define SHIFT_CLK 1

#define BUTTON_1 4
#define BUTTON_2 3

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

#define STATE_CALLIBRATING_START 70
#define STATE_CALLIBRATING_START_1 71
#define STATE_CALLIBRATING_START_2 72
#define STATE_CALLIBRATING_START_3 73

#define STATE_CALLIBRATING 81
#define STATE_CALLIBRATING_1 82
#define STATE_CALLIBRATING_2 83
#define STATE_CALLIBRATING_3 84


#define LED_ARRAY_DOT 1
#define BUTTON_DEBOUNCE_MS 100
#define EDIT_START_THRESHOLD 1000
#define CAL_START_THRESHOLD 4000
#define EDIT_TIMEOUT 10000
#define ERR_TIMEOUT 1000

#define VISION_PERSISTENCE delayMicroseconds(200)
#define CALLIBRATION_OK(v) (((v) > 0.2) && ((v) < 5))
#define COFFEETIME_OK(v) (((v) > 0) && ((v) < 99U*60U*1000U))
#define CLEAR shiftSegments(buzzing, 0, 0b0000)

/*
      0
   5     1
      6
   4     2
      3     7
*/

const uint8_t digits[] = {
    //01234567  
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110, // 9
};

const uint8_t cal[] = {
    //01234567  
    0b10011100,
    0b11101110,
    0b00011100
};

const uint8_t spinner[] = {
    //01234567  
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
};

const uint8_t err = 
    //01234567  
    0b10011110;

void shiftSegments(bool buzzer, uint8_t segments, uint8_t groundMask)
{
  for (uint8_t i = 0; i < 16; i++)
  {
    switch (i)
    {
    case 2:
    case 3:
      digitalWrite(SHIFT_DATA, !(groundMask & (1 << (i - 2))));
      break;
    case 4:
    case 5:
    case 6:
      digitalWrite(SHIFT_DATA, !!(segments & (1 << (i - 4))));
      break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      digitalWrite(SHIFT_DATA, !!(segments & (1 << (i - 8 + 3))));
      break;
    case 13:
    case 14:
      digitalWrite(SHIFT_DATA, !(groundMask & (1 << (i - 13 + 2))));
      break;
    case 15:
      digitalWrite(SHIFT_DATA, !!buzzer);
      break;
    }
    digitalWrite(SHIFT_CLK, HIGH);
    digitalWrite(SHIFT_CLK, LOW);
  }
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

uint8_t selectedButton = 1;
uint8_t state = 0;

uint32_t calStart = 0;
uint32_t calMinutes = 0;

uint32_t startedAt = 0;
uint32_t lastInteraction = 0;

uint32_t coffeeTime[2] = {240000, 180000};
uint8_t editMinutes = 0;
uint8_t editSeconds = 0;

bool buzzing = false;

bool shortBeep = false;
uint32_t shortBeepStart = 0;
uint32_t sleepTimeoutStart = 0;

float callibration = 1.0;

// EEPROM variables
float EEMEM callibrationMem = 1.0;
uint32_t EEMEM coffeeTime1Mem = 240000;
uint32_t EEMEM coffeeTime2Mem = 180000;

void setup()
{
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // Set up interrupts for sleep wakeup
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT3) | _BV(PCINT4);
  // Disable ADC
  ADCSRA &= ~_BV(ADEN);

  float maybeCal = eeprom_read_float(&callibrationMem);
  if (CALLIBRATION_OK(maybeCal)) {
    callibration = maybeCal;
  }

  uint32_t maybeCoffeeTime1 = eeprom_read_dword(&coffeeTime1Mem);
  if (COFFEETIME_OK(maybeCoffeeTime1)) {
    coffeeTime[0] = maybeCoffeeTime1;
  }

  uint32_t maybeCoffeeTime2 = eeprom_read_dword(&coffeeTime2Mem);
  if (COFFEETIME_OK(maybeCoffeeTime2)) {
    coffeeTime[1] = maybeCoffeeTime2;
  }
}

void writeNumber(uint16_t number)
{
  shiftSegments(buzzing, digits[number % 10], 0b0001);
  VISION_PERSISTENCE;
  number /= 10;
  shiftSegments(buzzing, number == 0 ? 0 : digits[number % 10], 0b0010);
  VISION_PERSISTENCE;
  number /= 10;
  shiftSegments(buzzing, number == 0 ? 0 : digits[number % 10], 0b0100);
  VISION_PERSISTENCE;
  number /= 10;
  shiftSegments(buzzing, number == 0 ? 0 : digits[number % 10], 0b1000);
  VISION_PERSISTENCE;
  CLEAR;
}

void writeDuration(uint32_t milliseconds, uint8_t mask)
{
  int seconds = ((milliseconds + 500) / 1000) % 60; // round up
  int minutes = ((milliseconds + 500) / 60000);

  shiftSegments(buzzing, digits[seconds % 10], 0b0001 & mask);
  VISION_PERSISTENCE;
  shiftSegments(buzzing, digits[seconds / 10 % 10], 0b0010 & mask);
  VISION_PERSISTENCE;
  shiftSegments(buzzing, digits[minutes % 10] + LED_ARRAY_DOT, 0b0100 & mask);
  VISION_PERSISTENCE;
  shiftSegments(buzzing, digits[minutes / 10 % 10], 0b1000 & mask);
  VISION_PERSISTENCE;
  CLEAR;
}

void writeCalibrating(int sequence)
{
  if (sequence == -1) {
    shiftSegments(buzzing, 0, 0b0001);
  } else {
    shiftSegments(buzzing, spinner[sequence % 6], 0b0001);
  }
  VISION_PERSISTENCE;

  for (int i = 2; i >= 0; i--) {
    shiftSegments(buzzing, cal[i], 0b1000 >> i);
    VISION_PERSISTENCE;
  }
  CLEAR;
} 

void writeCalibratingStart(int minutes, uint8_t mask)
{
  shiftSegments(buzzing, digits[minutes % 10], 0b0001 & mask);
  VISION_PERSISTENCE;

  if (minutes / 10 > 0) {
    shiftSegments(buzzing, digits[minutes / 10], 0b0010 & mask);
  } else {
    shiftSegments(buzzing, 0, 0b0010);
  }
  VISION_PERSISTENCE;

  shiftSegments(buzzing, 0, 0b0100);
  VISION_PERSISTENCE;

  shiftSegments(buzzing, cal[0], 0b1000);
  VISION_PERSISTENCE;
  CLEAR;
} 

void writeError()
{
  for (int i = 3; i >= 0; i--) {
    shiftSegments(buzzing, err, 0b1000 >> i);
    VISION_PERSISTENCE;
  }
  CLEAR;
}

void turnOffDisplay() { shiftSegments(buzzing, 0, 0b0000); }

ISR(PCINT0_vect)
{
  // Empty ISR, we only need it to be able to wake up from sleep
  // on button press.
}

#define CHECK_BUTTON_CUSTOM(shouldBePressed, timeout) \
  (buttonPressed ^ (shouldBePressed) &&               \
   currentTime - lastInteraction > timeout &&         \
   ((lastInteraction = currentTime), true))
#define BUTTON_UP CHECK_BUTTON_CUSTOM(false, BUTTON_DEBOUNCE_MS)
#define BUTTON_DOWN CHECK_BUTTON_CUSTOM(true, BUTTON_DEBOUNCE_MS)

#define EDITED_TIME (((uint32_t)editMinutes * 60 + editSeconds) * 1000)

#define SHORT_BEEP                \
  do                              \
  {                               \
    shortBeep = true;             \
    shortBeepStart = currentTime; \
  } while (false)

void goToSleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
}

bool callibrate() {
  float duration = (float)(millis() - calStart);

  float ratio = 60000.0 * ((float) calMinutes) / duration;

  if (CALLIBRATION_OK(ratio)) {
    // Skip updating EEPROM if the value is significantly outside normal range, as this could lead
    // to an unuasable UI.
    eeprom_write_float(&callibrationMem, ratio);
    callibration = ratio;
    return true;
  } else {
    return false;
  }
}

void writeCoffeeTime(uint8_t button, uint32_t value) {
  if (button == 0) {
    eeprom_write_dword(&coffeeTime1Mem, value);
  } else {
    eeprom_write_dword(&coffeeTime2Mem, value);
  }
  coffeeTime[button] = value;
}

void loop()
{
  uint32_t currentTime = (uint32_t)(((float)millis()) * callibration);
  uint8_t button1Pressed = digitalRead(BUTTON_1);
  uint8_t button2Pressed = digitalRead(BUTTON_2);
  uint8_t buttonPressed = button1Pressed && button2Pressed;

  // State transitions
  switch (state)
  {
  case STATE_DEFAULT_1:
    if (BUTTON_UP)
    {
      state = STATE_DEFAULT;
      sleepTimeoutStart = currentTime;
    }
    break;
  case STATE_DEFAULT:
    if (BUTTON_DOWN)
    {
      selectedButton = !button2Pressed ? 0 : 1;
      state = STATE_RUNNING_1;
    }
    break;
  case STATE_RUNNING_1:
    if (BUTTON_UP)
    {
      SHORT_BEEP;
      startedAt = currentTime;
      state = STATE_RUNNING;
    }
    else if (CHECK_BUTTON_CUSTOM(true, EDIT_START_THRESHOLD))
    {
      editSeconds = 0;
      editMinutes = 0;
      state = STATE_SETTING_MINUTES_1;
    }
    break;
  case STATE_CALLIBRATING_START_3:
    if (BUTTON_UP)
    {
      state = STATE_CALLIBRATING_START;
    }
    break;    
  case STATE_CALLIBRATING_START_1:
    if (BUTTON_UP)
    {
      state = STATE_CALLIBRATING_START;
    }
    break;
  case STATE_CALLIBRATING_START:
    if (BUTTON_DOWN) {
      if (button2Pressed) {
        calStart = millis();
        SHORT_BEEP;
        state = STATE_CALLIBRATING_1;
      } else {
        state = STATE_CALLIBRATING_START_2;
      }
    }
    break;   
  case STATE_CALLIBRATING_START_2:
    if (BUTTON_UP) {
      SHORT_BEEP;
      calMinutes = calMinutes + 1;
      if (calMinutes == 61) {
        calMinutes = 1;
      }
      state = STATE_CALLIBRATING_START;
    }
    break;       
  case STATE_CALLIBRATING_1:
    if (BUTTON_UP)
    {
      state = STATE_CALLIBRATING;
    }
    break; 
  case STATE_CALLIBRATING:
    if (BUTTON_DOWN)
    {
      if (!callibrate()) {
        state = STATE_CALLIBRATING_2;
      } else {
        state = STATE_DEFAULT_1;
      }
    }
    break;     
  case STATE_CALLIBRATING_2:
    if (BUTTON_UP)
    {
      state = STATE_CALLIBRATING_3;
    } else if (currentTime - lastInteraction > ERR_TIMEOUT)
    {
      state = STATE_DEFAULT_1;
    }
    break;               
  case STATE_CALLIBRATING_3:
    if (currentTime - lastInteraction > ERR_TIMEOUT)
    {
      state = STATE_DEFAULT;
      sleepTimeoutStart = currentTime;
    }
    break;         
  case STATE_SETTING_MINUTES_1:
    if (BUTTON_UP)
    {
      state = STATE_SETTING_MINUTES;
    } else if (CHECK_BUTTON_CUSTOM(true, CAL_START_THRESHOLD))
    {
      SHORT_BEEP;
      calMinutes = 1;
      state = STATE_CALLIBRATING_START_3;
    }
    break;
  case STATE_SETTING_MINUTES:
    if (BUTTON_DOWN)
    {
      state = STATE_SETTING_MINUTES_2;
    }
    else if (currentTime - lastInteraction > EDIT_TIMEOUT)
    {
      state = STATE_DEFAULT;
      sleepTimeoutStart = currentTime;
    }
    break;
  case STATE_SETTING_MINUTES_2:
    if (BUTTON_UP)
    {
      SHORT_BEEP;
      editMinutes = (editMinutes + 1) % 100;
      state = STATE_SETTING_MINUTES;
    }
    else if (CHECK_BUTTON_CUSTOM(true, EDIT_START_THRESHOLD))
    {
      SHORT_BEEP;
      state = STATE_SETTING_SECONDS_1;
    }
    break;
  case STATE_SETTING_SECONDS_1:
    if (BUTTON_UP)
    {
      state = STATE_SETTING_SECONDS;
    }
    break;
  case STATE_SETTING_SECONDS:
    if (BUTTON_DOWN)
    {
      state = STATE_SETTING_SECONDS_2;
    }
    else if (currentTime - lastInteraction > EDIT_TIMEOUT)
    {
      state = STATE_DEFAULT;
      sleepTimeoutStart = currentTime;
    }
    break;
  case STATE_SETTING_SECONDS_2:
    if (BUTTON_UP)
    {
      SHORT_BEEP;
      editSeconds = (editSeconds + 1) % 60;
      state = STATE_SETTING_SECONDS;
    }
    else if (CHECK_BUTTON_CUSTOM(true, EDIT_START_THRESHOLD))
    {
      SHORT_BEEP;
      writeCoffeeTime(selectedButton, EDITED_TIME);
      state = STATE_DEFAULT_1;
    }
    break;
  case STATE_RUNNING:
    if (BUTTON_DOWN)
    {
      state = STATE_DEFAULT_1;
    }
    else if (currentTime - startedAt > coffeeTime[selectedButton])
    {
      state = STATE_BUZZING;
      startedAt = currentTime;
    }
    break;
  case STATE_BUZZING:
    if (BUTTON_DOWN)
    {
      state = STATE_DEFAULT_1;
    }
    else if (currentTime - startedAt > 30000)
    {
      state = STATE_DEFAULT;
      sleepTimeoutStart = currentTime;
    }
    break;
  }

  if (shortBeep)
  {
    buzzing = true;
    if (currentTime - shortBeepStart > 60)
    {
      buzzing = false;
    }
  }

  // Display
  switch (state)
  {
  case STATE_DEFAULT:
  case STATE_DEFAULT_1:
    if (!shortBeep)
    {
      buzzing = false;
    }
    turnOffDisplay();
    break;
  case STATE_CALLIBRATING_START_3:
    writeCalibrating(-1);
    break;
  case STATE_CALLIBRATING_START:
  case STATE_CALLIBRATING_START_1:
  case STATE_CALLIBRATING_START_2:
    writeCalibratingStart(calMinutes, (currentTime / 300) % 2 ? 0b0000 : 0b1111);
    break;    
  case STATE_CALLIBRATING:
  case STATE_CALLIBRATING_1:
    writeCalibrating(currentTime / 100);
    break;        
  case STATE_CALLIBRATING_2:
  case STATE_CALLIBRATING_3:
    writeError();
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
    writeDuration(coffeeTime[selectedButton], 0b1111);
    break;
  case STATE_RUNNING:
    writeDuration(coffeeTime[selectedButton] - (currentTime - startedAt), 0b1111);
    break;
  case STATE_BUZZING:
    int bucket = (currentTime - startedAt) % 1000;
    int segment = bucket / 60;
    if ((segment < 8) && (segment % 2 == 0))
    {
      buzzing = true;
      writeDuration(0, 0b1111);
    }
    else
    {
      buzzing = false;
      turnOffDisplay();
    }
    break;
  }

  // We do this at the end to make sure the display is blank and the
  // buzzer is disabled.
  if (state == STATE_DEFAULT && currentTime - sleepTimeoutStart > 1000)
  {
    // Save battery by disabling the clock after one second of idle
    goToSleep();
    sleepTimeoutStart = currentTime;
  }
}
