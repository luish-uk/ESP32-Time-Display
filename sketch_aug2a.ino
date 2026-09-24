// Libraries for the touchscreen display to work
#include <SPI.h>
//These two libraries must also be installed on the arduino IDE
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h> 

#include <WiFi.h>
#include <ctime>

TFT_eSPI tft = TFT_eSPI(); //Creating an instance

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

// Creates touchscreenSPI and touchscreen instances 
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

//Resolution of display and text
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

// Center coordinates for displaying text
#define CENTERX SCREEN_WIDTH / 2
#define CENTERY SCREEN_HEIGHT / 2

//Network Settings - config yourself for your own network
const char ssid[] = "test";
const char password[] = "123456789";

void setup() {
  Serial.begin(115200);

  //Attempts to connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWifi connected");

  // Time is configured for 0 offset in Winters, and 1hr offset in summer from UTC (in short: London time)
  configTime(0, 3600, "pool.ntp.org");

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
  // Set X and Y coordinates for center of display

  // Intro message 
  tft.drawCentreString("Hello World!", CENTERX, CENTERY, FONT_SIZE);
  
}

void loop() {
  struct tm timeinfo;
  // Checks if Touchscreen was touched, and prints peakaboo to the display
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // For some reason the screen is instead black with white text but either is fine 
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);

    // Scary message 
    tft.drawCentreString("Peakaboo", CENTERX, CENTERY, FONT_SIZE);
    delay(100);

  } else {

    // creates a time instance for both tm and ctime
    struct tm timeinfo;
    time_t timestamp;

    // gets the local time from timeinfo structure
    if (getLocalTime(&timeinfo)) {
      tft.fillScreen(TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);

      // Converts the struct into a timestamp 
      timestamp = mktime(&timeinfo);
      
      // Converts to a string
      char* timeString = std::ctime(&timestamp);

      tft.drawCentreString(timeString, CENTERX, CENTERY, FONT_SIZE);
      delay(1000);
    }  
  }
}
