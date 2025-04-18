#include <Arduino.h>
#include <Keypad.h>

const byte ROWS = 2; //four rows
const byte COLS = 2; //three columns
char keys[ROWS][COLS] = {
    {'1','2'},
    {'4','5'}
};

byte rowPins[ROWS] = {10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11};    //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = LED_BUILTIN;

boolean blink = false;
boolean ledPin_state;

void keypadEvent(KeypadEvent key);

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
  digitalWrite(ledPin, HIGH);           // Turn the LED on.
  ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
}

void loop()
{
  char key = keypad.getKey();

  if(key)
  {
    Serial.println(key);
  }
  if(blink)
  {
    digitalWrite(ledPin, !digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
    delay(100);
  }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key)
{
  switch(keypad.getState())
  {
    case PRESSED:
      if(key == '#')
      {
        digitalWrite(ledPin, !digitalRead(ledPin));
        ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
      }
      break;

    case RELEASED:
      if(key == '*')
      {
        digitalWrite(ledPin, ledPin_state);    // Restore LED state from before it started blinking.
        blink = false;
      }
      break;

    case HOLD:
      if(key == '*')
      {
        blink = true;    // Blink the LED when holding the * key.
      }
      break;
  }
}