#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <UltrasonicHCSR04.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define BUZZER_PIN 32
#define TRIG_PIN 26
#define ECHO_PIN 12
#define THRESHOLD 10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
UltrasonicHCSR04 ultrasonic(TRIG_PIN, ECHO_PIN);

int distance = 0;
int buzzerDelay = 0;
int buzzerFreq = 0;

void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
    {
    }
  }

  pinMode(BUZZER_PIN, OUTPUT);

  display.setTextSize(2);
  display.clearDisplay();
  display.display();
}

void loop()
{
  distance = ultrasonic.measureDistanceCM();
  if (distance > 0 && distance < 100)
  {
    buzzerDelay = map(distance, 1, 100, 1000, 5000);
    buzzerFreq = map(distance, 1, 100, 2000, 200);

    display.clearDisplay();
    if (distance <= THRESHOLD)
    {
      display.setTextColor(SSD1306_INVERSE);
    }
    else
    {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(0, 24);
    display.printf("Dist: %dcm", distance);
    display.display();

    tone(BUZZER_PIN, buzzerFreq, buzzerDelay);
    delay(buzzerDelay);
  }
  else
  {
    noTone(BUZZER_PIN);

    display.clearDisplay();
    display.setCursor(0, 24);
    display.setTextColor(SSD1306_WHITE);
    display.print("No Signal");
    display.display();
    delay(500);
  }
}
