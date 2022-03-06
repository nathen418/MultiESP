#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>

// Create An LCD Object. Signals: [ RS, EN, D4, D5, D6, D7 ]
LiquidCrystal lcd(26, 27, 17, 5, 18, 19);
const char *ssid = "MyResNet Legacy";
const char *password = "REPLACE_WITH_YOUR_PASSWORD";

// Your Domain name with URL path or IP address with path
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
		digitalWrite(19, HIGH);
		delay(mils);
		digitalWrite(18, HIGH);
		delay(mils);
		digitalWrite(5, HIGH);
		delay(mils);
		digitalWrite(17, HIGH);
		delay(mils);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(mils);
		digitalWrite(19, LOW);
		delay(mils);
		digitalWrite(18, LOW);
		delay(mils);
		digitalWrite(5, LOW);
		delay(mils);
		digitalWrite(17, LOW);
		delay(mils);
		digitalWrite(LED_BUILTIN, LOW);
		delay(mils);
	}
}

void init()
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
		digitalWrite(34, HIGH); // Red
		digitalWrite(33, HIGH); // Yellow
	}
	else if (state == 1)
	{
		digitalWrite(34, LOW); // Red
		digitalWrite(33, LOW); // Yellow
	}
}

void request()
{
	HTTPClient http;

	// Your Domain name with URL path or IP address with path
	http.begin(serverName.c_str());

	// Send HTTP GET request
	blinkNum(1, 100, 18);
	int httpResponseCode = http.GET();
	blinkNum(1, 100, 18);

	if (httpResponseCode == 200)
	{
		Serial.print("OK");
		Serial.println(httpResponseCode);
		String payload = http.getString();
		Serial.println(payload);
		blinkNum(2, 250, 5);
	}
	else
	{
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);
		blinkNum(8, 90, 19);
	}
	// Free resources
	http.end();
	blinkNum(1, 600, 17);
	waitingStatus();
}

void setup()
{
	// LED pins
	pinMode(LED_BUILTIN, OUTPUT); // Integrated Blue LED
	pinMode(34, OUTPUT);		  // Red
	pinMode(35, OUTPUT);		  // Blue
	pinMode(32, OUTPUT);		  // Green
	pinMode(33, OUTPUT);		  // Yellow
	pinMode(25, OUTPUT);		  // White

	// LCD Pins
	// pinMode(23, OUTPUT); // LCD
	// pinMode(26, OUTPUT); // LCD
	// pinMode(25, OUTPUT); // LCD
	// pinMode(33, OUTPUT); // LCD
	// pinMode(32, OUTPUT); // LCD

	lcd.begin(16, 2);
	lcd.clear();
	lcd.print("Testing...");

	Serial.begin(115200);

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
	digitalWrite(19, HIGH);
	digitalWrite(17, HIGH);
}

void loop()
{
	// Only enter this if the timer has expired
	if ((millis() - lastTime) > timerDelay)
	{
		digitalWrite(19, LOW);
		digitalWrite(17, LOW);
		// Check WiFi connection status
		if (WiFi.status() == WL_CONNECTED)
		{
			// send the HTTP POST request
			request();
		}
		else
		{
			Serial.println("WiFi Disconnected");
			blinkNum(5, 70, 34);
			lcd.clear();
			lcd.print("WiFi Disconnected");
		}
		lastTime = millis();
	}
}
