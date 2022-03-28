#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include "secrets.h"
#include <ArduinoJson.h>

int S = 33;
int E = 32;
int D4 = 19;
int D5 = 27;
int D6 = 26;
int D7 = 25;

int White = 15;
int Amber = 4;
int Green = 16;
int Blue = 17;
int Red = 18;

LiquidCrystal lcd(S, E, D4, D5, D6, D7);
const char *ssid = "TestWifi";
const char *password = "";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void blinkNum(int num, int mils, int pin)
{
  for (int i = 0; i < num; i++)
  {
    digitalWrite(pin, HIGH);
    delay(mils);
    digitalWrite(pin, LOW);
    delay(mils);
  }
}

void wipe(int count, int mils)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(Red, HIGH);
    delay(mils);
    digitalWrite(Blue, HIGH);
    delay(mils);
    digitalWrite(Green, HIGH);
    delay(mils);
    digitalWrite(Amber, HIGH);
    delay(mils);
    digitalWrite(White, HIGH);
    delay(mils);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(mils);
    digitalWrite(Red, LOW);
    delay(mils);
    digitalWrite(Blue, LOW);
    delay(mils);
    digitalWrite(Green, LOW);
    delay(mils);
    digitalWrite(Amber, LOW);
    delay(mils);
    digitalWrite(White, LOW);
    delay(mils);
    digitalWrite(LED_BUILTIN, LOW);
    delay(mils);
  }
}

void test()
{
  // LCD Test
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LCD Self Test");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Success...");
  delay(2000);
  lcd.clear();

  // LED Test
  lcd.setCursor(0, 0);
  lcd.print("LED Self Test");
  wipe(4, 80);
  lcd.clear();

  // WiFi Test
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    blinkNum(5, 100, LED_BUILTIN);
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
  }
  lcd.clear();
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(4000);
  lcd.clear();
}

void waitingStatus(int state)
{
  // state = 0 - Waiting
  // state = 1 - Not Waiting
  if (state == 0)
  {
    digitalWrite(Red, HIGH);
    digitalWrite(Amber, HIGH);
  }
  else if (state == 1)
  {
    digitalWrite(Red, LOW);
    digitalWrite(Amber, LOW);
  }
}

//LCD Pagination
void pagination(String tweet, int waitMs){
      if (tweet.length() <= 31)
    {
      lcd.setCursor(0, 0);
      lcd.print(tweet.substring(0, 15));
      lcd.setCursor(0, 1);
      lcd.print(tweet.substring(16, 31));
      delay(150);
      return;
    }
      for (int i = 0; i < tweet.length(); i += 31)
      {
        lcd.setCursor(0, 0);
        lcd.print(tweet.substring(i, i + 15));
        lcd.setCursor(0, 1);
        lcd.print(tweet.substring(i + 15, i + 31));
        delay(waitMs);
      }
    }


String parseJSON(String response)
{
  StaticJsonBuffer<3500>jsonBuffer;
  const char *json = response.c_str();
  JsonObject& root = jsonBuffer.parseObject(json);
  JsonArray& data = root["data"];
  const char* data_0_id = data[0]["id"];
  const char* data_0_text = data[0]["text"];  
  return String(data_0_text);
}

void request()
{
  lcd.clear();
  HTTPClient http;

  http.begin(api_endpoint);
  http.addHeader("Authorization", bearer_token);

  // Send HTTP GET request
  blinkNum(1, 100, Blue);
  lcd.print("Sending Request");
  int httpResponseCode = http.GET();
  lcd.clear();
  blinkNum(1, 100, Blue);

  if (httpResponseCode == 200)
  {
    lcd.print("Response: ");
    lcd.print(httpResponseCode);
    String payload = http.getString();
    blinkNum(2, 250, Green);
    lcd.clear();
    pagination(parseJSON(payload), 2500);
  }
  else
  {
    lcd.print("Response: ");
    lcd.print(httpResponseCode);
    String payload = http.getString();
    Serial.print(payload);
    blinkNum(8, 90, Red);
    lcd.clear();
  }
  // Free resources
  http.end();
  blinkNum(1, 600, Amber);
  waitingStatus(0);
  lcd.setCursor(0,1);
  lcd.print("Waiting...");
}

void setup()
{
  // Set LED pins to Output
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);
  // wipe(5,80);
  // Test Function
  test();

  // Clear LCD
  lcd.clear();

  // Init Serial
  Serial.begin(115200);

  // Init WiFi
  waitingStatus(0);
}

void loop()
{
  // Only enter this if the timer has expired
  if ((millis() - lastTime) > timerDelay)
  {
    waitingStatus(1);
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      // send the HTTP POST request
      request();
    }
    else
    {
      blinkNum(5, 70, Red);
      lcd.clear();
      lcd.print("Disconnected");
    }
    lastTime = millis();
  }
}
