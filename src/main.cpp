#include <Arduino.h>
#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

// Define I2C addresses (for displays only)
#define SSD1306_I2C_ADDRESS 0x3C
#define LCD_I2C_ADDRESS 0x27

// Define UART pins for communication
#define UART_RX_PIN 3   // GPIO3 (RX)
#define UART_TX_PIN 1   // GPIO1 (TX)

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 4, 25, 26};
byte colPins[COLS] = {18, 13, 14, 15};

Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Adafruit_SSD1306 display(128, 64, &Wire, -1);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

// Use Serial2 for UART communication
HardwareSerial mySerial(2);  // UART2

char serialBuffer[100];
int serialIndex = 0;

void setup() {
  Serial.begin(115200);  // Debugging only
  Serial.println("Starting ESP with UART comm...");

  // Initialize Serial2 with custom RX/TX pins
  mySerial.begin(115200, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  // Initialize displays
  if (!display.begin(SSD1306_I2C_ADDRESS, SSD1306_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 init failed"));
    while (true);
  }

  lcd.init();
  lcd.backlight();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Ready");
  display.display();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Ready"));
}

void displayText(const char* text, int textSize) {
  const char* content = text;
  bool displayOled = false, displayLcd = false;

  if (strncmp(text, "oled:", 5) == 0) {
    content = text + 6;
    displayOled = true;
  } else if (strncmp(text, "lcd:", 4) == 0) {
    content = text + 5;
    displayLcd = true;
  } else if (strncmp(text, "both:", 5) == 0) {
    content = text + 6;
    displayOled = true;
    displayLcd = true;
  }

  if (displayOled) {
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(0, 0);
    display.print(content);
    display.display();
  }

  if (displayLcd) {
    lcd.clear();
    lcd.setCursor(0, 0);
    int i = 0;
    while (i < 16 && content[i] != '\0') {
      lcd.print(content[i]);
      i++;
    }
    if (content[i] != '\0') {
      lcd.setCursor(0, 1);
      while (i < 32 && content[i] != '\0') {
        lcd.print(content[i]);
        i++;
      }
    }
  }
}

void loop() {
  // Handle keypad press
  char key = keypad.getKey();
  if (key) {
    mySerial.write(key);  // Send key to external device via UART2
    Serial.print(F("Key Pressed: "));
    Serial.println(key);

    // Show locally as well
    String local = "both:";
    local += key;
    displayText(local.c_str(), 2);
  }

  // Handle incoming UART data
  while (mySerial.available()) {
    char c = mySerial.read();
  
    if (c == '|') {  // Use newline as message terminator
      serialBuffer[serialIndex] = '\0'; // Null-terminate string
      displayText(serialBuffer, 1);     // Display incoming message
      serialIndex = 0;
      memset(serialBuffer, 0, sizeof(serialBuffer)); // Clear buffer
    } else {
      if (serialIndex < sizeof(serialBuffer) - 1) {
        serialBuffer[serialIndex++] = c;
      }
    }
  }
  
}
