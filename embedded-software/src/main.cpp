#include <Arduino.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// prototypes
void keypadEvent(KeypadEvent key);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

byte rowPins[ROWS] = {10, 9, 8};    // TODO: row pins
byte colPins[COLS] = {12, 11};      // TODO: column pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = LED_BUILTIN;

boolean blink = false;
boolean ledPin_state;

// Initialize software serial
SoftwareSerial mySoftwareSerial(2, 3);  // TODO: TX, RX (on DF player)
DFRobotDFPlayerMini myDFPlayer;
String line;
char command;
int pause = 0;
int repeat = 0;

void setup()
{
    // --------------------------- BUTTON MATRIX ---------------------------
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    digitalWrite(ledPin, HIGH);           // Turn the LED on.
    ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad


    // ----------------------------- DFPLAYER ------------------------------
    mySoftwareSerial.begin(9600); // Serial communication with the module

    // Check if the module is responding and if the SD card is found
    Serial.println(F("DFRobot DFPlayer Mini"));
    Serial.println(F("Initializing DFPlayer module ... Wait!"));

    // initialization didn't work
    if(!myDFPlayer.begin(mySoftwareSerial))
    {
        Serial.println(F("Not initialized:"));
        Serial.println(F("1. Check the DFPlayer Mini connections"));
        Serial.println(F("2. Insert an SD card"));
        while(true);
    }

    Serial.println(F("DFPlayer Mini module initialized!"));

    // Initial settings
    myDFPlayer.setTimeOut(500); // Serial timeout 500ms
    myDFPlayer.volume(10);      // Volume settings
    myDFPlayer.EQ(0);           // Normal equalization

    // optional equalization settings
    // myDFPlayer.EQ(DFPLAYER_EQ_POP);
    // myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
    // myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
    // myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
    // myDFPlayer.EQ(DFPLAYER_EQ_BASS);
}

void loop()
{
    char key = keypad.getKey();

    if(key)
        Serial.println(key);

    if(blink)
    {
        digitalWrite(ledPin, !digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
        delay(100);
    }

    // Play from first 9 files
    if((key >= '0') && (key <= '9'))
    {
        Serial.print("Music reproduction"); // text output
        Serial.println(key);                // print input

        // play sound file
        key = key - 48;
        myDFPlayer.play(key);
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
                ledPin_state = digitalRead(ledPin); // Remember LED state, lit or unlit.
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
            // Blink the LED when holding the RESET key.
            if(key == '*')
                blink = true;
            break;

        case IDLE:
            break;
    }
}