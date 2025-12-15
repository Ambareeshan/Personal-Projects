#include <U8g2lib.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <FluxGarage_RoboEyes.h>
#include <time.h>
#include <Wire.h>


#define WIDTH 128 //Screen Width
#define HEIGHT 64 //Screen Heignt
#define OLED_RESET -1 //Reset Pin set to -1 since sharing with ESP32
#define TOUCH_INPUT 21 //Touch Sensor input pin on GPIO21

bool isPressed = LOW; //Variable to store Touch Sensore input
//WiFi Credentials to connnect to WiFi
const char* ssid = "Ramsharan";
const char* password = "Ram007Sharan*"; 
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 3600;
char timeHour[3];
char timeMin[3];
char timeSec[3];
char timeWeekDay[10];

//Location image details
static const unsigned char image_New_Project_bits[] = {0x00,0x00,0xf0,0x01,0xf8,0x03,0xb8,0x03,0x18,0x07,0x1c,0x07,0x18,0x07,0xf8,0x03,0xf0,0x03,0xf0,0x01,0xe0,0x00,0xc0,0x00,0x00,0x00};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup()
{
  Serial.begin(115200);
  u8g2.begin();

  //Connect to WiFi
  Serial.print("Connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected.");

  //Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect Wifi as it is no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi Disconnected");
}

void loop()
{
  u8g2.clearBuffer();
  delay(1000);
  printLocalTime();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(timeWeekDay, 10,"%A", &timeinfo);
  strftime(timeHour, 3,"%H", &timeinfo);
  strftime(timeMin, 3,"%M", &timeinfo);
  strftime(timeSec, 4, "%S", &timeinfo);

  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont17_tr);
  u8g2.drawStr(97, 55, timeSec);
  u8g2.setFont(u8g2_font_profont29_tr);
  u8g2.drawStr(14, 55, timeHour);
  u8g2.drawStr(62, 55, timeMin);
  u8g2.drawStr(46, 55, ":");
  u8g2.setFont(u8g2_font_profont17_tr);
  u8g2.drawStr(37, 30, timeWeekDay);
  u8g2.setFont(u8g2_font_profont11_tr);
  u8g2.drawStr(47, 12, "CHENNAI");
  u8g2.drawXBM(33, 2, 13, 13, image_New_Project_bits);
  u8g2.sendBuffer();
}


