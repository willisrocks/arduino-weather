#include <SPI.h>
//#include <Dhcp.h>
//#include <Dns.h>
#include <HttpClient.h>
#include <Ethernet.h>
//#include <EthernetClient.h>
//#include <EthernetServer.h>
//#include <EthernetUdp.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>


// **********************************
// Variables
// **********************************
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin
const int chipSelect = 4;
int buttonState = HIGH;         // variable for reading the pushbutton status
int ledState = HIGH;
int lastButtonState = HIGH;
long lastDebounceTime = 0;
long debounceDelay = 50;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
byte mac[] = { 0xFE, 0xAD, 0xBE, 0xEF, 0xFE, 0xDE };
const char kHostname[] = "weather.chrisfenton.info";
//char kPath[]; //"/post?ross=noob";
const int kNetworkTimeout = 30 * 1000;
const int kNetworkDelay = 1000;
int networkSetup = 0;
int sdcardSetup = 0;
File dataFile;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, ledState);
  Serial.begin(9600);
}

void loop() {
  if (ledState == HIGH){
    if (sdcardSetup == 1) {
      gatherData();
    } else {
      setupSD();
    }
  }
  if (ledState == LOW){
    //Serial.println("Pushing Data");
    if (networkSetup == 1) {
      pushData();
    } else {
      setupNetwork();
    }
  }
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != buttonState){
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastButtonState = reading;
}

void setupNetwork() {
  Serial.println("Starting w5100...");
  if (!Ethernet.begin(mac)) {
    Serial.println("Eth Error");
  } else {
    Serial.println(Ethernet.localIP());
    while (Ethernet.begin(mac) != 1)
    {
      delay(10000);
    }
  }
  networkSetup = 1;
}

void setupSD() {
  Serial.println("Setup SD");
  while (!Serial) {
    }
    if (!SD.begin(chipSelect)) {
      return;
    }
    sdcardSetup = 1;
}

void gatherData() {
//    String dataString = "";
//    if (! baro.begin()) {
//      Serial.println("Error opening SD");
//      return;
//    }
//    float pascals = baro.getPressure();
//    float pressure = (pascals / 3377);
//    dataString += String(pressure);
//    dataString += ",";
//    float altm = baro.getAltitude();
//    dataString += String(altm);
//    dataString += ",";
//    float tempC = baro.getTemperature();
//    float temperatureF = (tempC * 9.0 / 5.0) + 32.0;
//    dataString += String(temperatureF);
//    File dataFile = SD.open("datalog.txt", FILE_WRITE);
//    if (dataFile) {
//      dataFile.println(dataString);
//      dataFile.close();
//    }
//    else {
//    }
//    delay(5000);
//    if (! baro.begin()) {
//      return;
//    }
//    delay(50000);
}

void pushData() {
  char kPath[] = "/temps/?temp=79.2&pres=31.8&alt=5.1";
  EthernetClient c;
  HttpClient http(c);
  uint16_t kPort = 4567;

  http.post(kHostname, kPort, kPath);
  int response = http.responseStatusCode();
  Serial.println(response);

  http.stop();
  delay(500000);
}

