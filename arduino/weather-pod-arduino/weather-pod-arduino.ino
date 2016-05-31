#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

// **********************************
// Variables
// **********************************

// Pins
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin
const int chipSelect = 4;

// Buttons
int buttonState;         // variable for reading the pushbutton status
int ledState = HIGH;
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

// Weather
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
byte mac[] = { 0xFE, 0xAD, 0xBE, 0xEF, 0xFE, 0xDE };
const char kHostname[] = "httpbin.org";
const char kPath[] = "/post?ross=noob";
const int kNetworkTimeout = 30 * 1000;
const int kNetworkDelay = 1000;
int networkSetup = 0;
int sdcardSetup = 0;


void setup() {
  // buttons
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, ledState);

  Serial.begin(9600);
}

void loop() {
  if (ledState == HIGH){
    Serial.println("Gathering Data");
    if (sdcardSetup == 1) {
      // INSERT DATA GATHERING CODE

      
    } else {
      setupSD;
    }

  }
  if (ledState == LOW){
    Serial.println("Pushing Data");
    
    if (networkSetup == 1) {
      // INSERT DATA UPLOAD CODE

      
    } else {
      setupNetwork();
    }

  }

  // **********************************
  // Button Code Below
  // **********************************
  
  int reading = digitalRead(buttonPin);
  // read the state of the pushbutton value:
  //buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (reading != lastButtonState) {

    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay){

    if (reading != buttonState){
      buttonState = reading;

      if (buttonState == HIGH) {
        ledState = !ledState;
        //change ledState to pushing and pulling data
        Serial.println("Changing State");//prints every change
      }
    }
  }
  digitalWrite(ledPin, ledState);

  lastButtonState = reading;

}

void setupNetwork() {
  // network and SD card
  Serial.println("Starting w5100...");
  if (!Ethernet.begin(mac)) {
    
    Serial.println("Ethernet not connected");
    
  } else {
    
    Serial.println(Ethernet.localIP());

    while (Ethernet.begin(mac) != 1)
    {
      Serial.println("Error getting IP address via DHCP, trying again...");
      delay(10000);
    }
  }
  Serial.println("Ethernet connected");
  networkSetup = 1;
}

void setupSD() {
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.print("Initializing SD card...");

    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    Serial.println("card initialized.");
    sdcardSetup = 1;
}

