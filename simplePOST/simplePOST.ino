#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>

int S = 33;
int E = 32;
int D4 = 19;
int D5 = 27;
int D6 = 26;
int D7 = 25;

int White = 15;
int Amber = 4;
int Green = 6;
int Blue = 17;
int Red = 18;

LiquidCrystal lcd(S, E, D4, D5, D6, D7);
const char *ssid = "MyResNet Legacy";
const char *password = "REPLACE_WITH_YOUR_PASSWORD";

String serverName = "https://www.playantares.com/";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

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
	lcd.setCursor(0, 1);
	lcd.print("Success...");
	delay(2000);
	lcd.clear();

	// WiFi Test
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		blinkNum(5, 100, LED_BUILTIN);
		lcd.setCursor(0, 0);
		lcd.print("Connecting to WiFi");
		lcd.setCursor(0, 1);
		lcd.print("SSID: ");
		lcd.print(ssid);
    
		lcd.print("...");
	}
	lcd.clear();
	lcd.print("Connected to WiFi");
	lcd.setCursor(0, 1);
	lcd.print("IP: ");
	lcd.print(WiFi.localIP());
	delay(4000);
	lcd.clear();

	// LED Test
	lcd.setCursor(0, 0);
	lcd.print("LED Test");
	wipe(4, 80);
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

void request()
{
	HTTPClient http;

	// Your Domain name with URL path or IP address with path
	http.begin(serverName.c_str());

	// Send HTTP GET request
	blinkNum(1, 100, Blue);
	int httpResponseCode = http.GET();
	blinkNum(1, 100, Blue);

	if (httpResponseCode == 200)
	{
		Serial.print("OK");
		Serial.println(httpResponseCode);
		String payload = http.getString();
		Serial.println(payload);
		blinkNum(2, 250, Green);
	}
	else
	{
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);
		blinkNum(8, 90, Red);
	}
	// Free resources
	http.end();
	blinkNum(1, 600, Amber);
	waitingStatus(0);
}

void setup()
{
  
	// Set LED pins to Output
	pinMode(Red, OUTPUT);
	pinMode(Blue, OUTPUT);
	pinMode(Green, OUTPUT);
	pinMode(Amber, OUTPUT);
	pinMode(White, OUTPUT);

  //Test Function
  //test();

  //Init LCD
	lcd.begin(16, 2);
	lcd.clear();
	lcd.print("Testing...");

  //Init Serial
	Serial.begin(115200);

  //Init WiFi
	WiFi.begin(ssid);
	Serial.println("Connecting");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to WiFi network with IP Address: ");
	Serial.println(WiFi.localIP());
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
			Serial.println("WiFi Disconnected");
			blinkNum(5, 70, Red);
			lcd.clear();
			lcd.print("WiFi Disconnected");
		}
		lastTime = millis();
	}
}
