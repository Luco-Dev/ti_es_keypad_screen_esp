#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define I2C_ADDRESS 0x08
#define SSD1306_I2C_ADDRESS 0x3C
#define LCD_I2C_ADDRESS 0x27

const byte ROWS = 4;
const byte COLS = 4;
const char keys[ROWS][COLS] PROGMEM = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {7, 6, 8, 9};
Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char receivedData[70];
char targetDevice[5];

char lastKey = '\0';

Adafruit_SSD1306 display(128, 64, &Wire, -1);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

void sendKey() {
  Wire.write(lastKey);
  lastKey = '\0';
}

// Combine temporary and received data into a single buffer
char buffer[70];
int bufferIndex = 0;

void receiveEvent(int bytes) {
  char c = '\0';
  
  while (Wire.available()) {
    c = Wire.read();
    
    if (c == '#') {
      break;
    }
    
    if (c != '\0' && bufferIndex < sizeof(buffer) - 1) {
      buffer[bufferIndex++] = c;
    }
  }

  //buffer[bufferIndex] = '\0';

  if (c == '#') {
    strcpy(receivedData, buffer);
    Serial.println(receivedData);

    bufferIndex = 0;
    memset(buffer, 0, sizeof(buffer));
  }
}

void setup() {
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendKey);
  Serial.begin(9600);
  
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  
  lcd.init();
  lcd.backlight();
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ready"));
}

void displayText(const char* text, int textSize) {
  //Serial.println(text);

  if (strncmp(text, "oled:", 5) == 0) {
    strcpy(targetDevice, "oled");
    text += 6;
  } else if (strncmp(text, "lcd:", 4) == 0) {
    strcpy(targetDevice, "lcd");
    text += 5;
  } else if (strncmp(text, "both:", 5) == 0) {
    strcpy(targetDevice, "both");
    text += 6;
  }

  bool displayOled = (strcmp(targetDevice, "oled") == 0 || strcmp(targetDevice, "both") == 0);
  bool displayLcd = (strcmp(targetDevice, "lcd") == 0 || strcmp(targetDevice, "both") == 0);

  if (displayOled) {
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(0, 0);
    display.print(text);
    display.display();
  }

  if (displayLcd) {
    lcd.clear();
    lcd.setCursor(0, 0);

    int i = 0;
    while (i < 16 && text[i] != '\0') {
      lcd.print(text[i]);
      i++;
    }

    if (text[i] != '\0') {
      lcd.setCursor(0, 1);
      while (i < 32 && text[i] != '\0') {
        lcd.print(text[i]);
        i++;
      }
    }
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    lastKey = key;
    Serial.print(F("Key Pressed: "));
    Serial.println(key);
    displayText(&key, 2);
  }

  if (receivedData[0] != '\0') {
    displayText(receivedData, 1);
    memset(receivedData, 0, sizeof(receivedData));
  }
}