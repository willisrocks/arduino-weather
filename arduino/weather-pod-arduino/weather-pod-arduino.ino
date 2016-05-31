// Pins
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// Variables
int buttonState;         // variable for reading the pushbutton status
int ledState = HIGH;
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // start serial port
  Serial.begin(9600);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  if (ledState == HIGH){
    Serial.println("Gathering Data");
    // INSERT DATA GATHERING CODE

  }
  if (ledState == LOW){
    Serial.println("Pushing Data");
    // INSERT DATA UPLOAD CODE

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
