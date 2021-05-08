#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

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

#define LED_ARRAY_DOT 1
#define BUTTON_DEBOUNCE_MS 100
#define EDIT_START_THRESHOLD 1000
#define EDIT_TIMEOUT 10000

const uint8_t digits[] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110,
};

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

void writeNumber(bool buzzer, uint16_t number)
{
  shiftSegments(buzzer, digits[number % 10], 0b0001);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(buzzer, number == 0 ? 0 : digits[number % 10], 0b0010);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(buzzer, number == 0 ? 0 : digits[number % 10], 0b0100);
  delayMicroseconds(200);
  number /= 10;
  shiftSegments(buzzer, number == 0 ? 0 : digits[number % 10], 0b1000);
  delayMicroseconds(200);
}

void writeDuration(bool buzzer, uint32_t milliseconds, uint8_t mask)
{
  int seconds = ((milliseconds + 500) / 1000) % 60; // round up
  int minutes = ((milliseconds + 500) / 60000);

  shiftSegments(buzzer, digits[seconds % 10], 0b0001 & mask);
  delayMicroseconds(200);
  shiftSegments(buzzer, digits[seconds / 10 % 10], 0b0010 & mask);
  delayMicroseconds(200);
  shiftSegments(buzzer, digits[minutes % 10] + LED_ARRAY_DOT, 0b0100 & mask);
  delayMicroseconds(200);
  shiftSegments(buzzer, digits[minutes / 10 % 10], 0b1000 & mask);
  delayMicroseconds(200);
  // This is to ensure even brightness of all four digits if we take a long time
  // after this function is called.
  shiftSegments(buzzer, 0, 0b0000);
}

void turnOffDisplay(bool buzzer) { shiftSegments(buzzer, 0, 0b0000); }

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
}

ISR(PCINT0_vect)
{
  // Empty ISR, we only need it to be able to wake up from sleep
  // on button press.
}

uint8_t selectedButton = 1;

uint8_t state = 0;
uint32_t startedAt = 0;
uint32_t lastInteraction = 0;

uint32_t coffeeTime[2] = {240000, 180000};
uint8_t editMinutes = 0;
uint8_t editSeconds = 0;

bool buzzing = false;

bool shortBeep = false;
uint32_t shortBeepStart = 0;
uint32_t sleepTimeoutStart = 0;

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

void loop()
{
  uint32_t currentTime = millis();
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
    else if (CHECK_BUTTON_CUSTOM(true, 1000))
    {
      editSeconds = 0;
      editMinutes = 0;
      state = STATE_SETTING_MINUTES_1;
    }
    break;
  case STATE_SETTING_MINUTES_1:
    if (BUTTON_UP)
    {
      state = STATE_SETTING_MINUTES;
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
      coffeeTime[selectedButton] = EDITED_TIME;
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
    turnOffDisplay(buzzing);
    break;
  case STATE_SETTING_SECONDS:
  case STATE_SETTING_SECONDS_1:
  case STATE_SETTING_SECONDS_2:
    writeDuration(buzzing, EDITED_TIME,
                  (currentTime / 300) % 2 ? 0b1100 : 0b1111);
    break;
  case STATE_SETTING_MINUTES:
  case STATE_SETTING_MINUTES_1:
  case STATE_SETTING_MINUTES_2:
    writeDuration(buzzing, EDITED_TIME,
                  (currentTime / 300) % 2 ? 0b0011 : 0b1111);
    break;
  case STATE_RUNNING_1:
    writeDuration(buzzing, coffeeTime[selectedButton], 0b1111);
    break;
  case STATE_RUNNING:
    writeDuration(buzzing,
                  coffeeTime[selectedButton] - (currentTime - startedAt),
                  0b1111);
    break;
  case STATE_BUZZING:
    int bucket = (currentTime - startedAt) % 1000;
    int segment = bucket / 60;
    if ((segment < 8) && (segment % 2 == 0))
    {
      buzzing = true;
      writeDuration(buzzing, 0, 0b1111);
    }
    else
    {
      buzzing = false;
      turnOffDisplay(buzzing);
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