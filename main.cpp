#include <Arduino.h>
// CONSTANTS
// The "start zone" which players must touch to start the game each time
const byte startPin = 3;
// Touching any part of the wire itself causes a failure
const byte failPin = 5;
// The "win zone" at the end of the wire
const byte endPin = 4;
const byte relayPin = 2;
// A piezo buzzer chirps to signify success/failure
const byte buzzerPin = 13;
const byte grnPin = 6;
const byte redPin = 7;

// GLOBALS
// Keep track of the current states of the game
enum GameState {FAILED, IN_PROGRESS, SUCCESS};
GameState gameState = GameState::FAILED;


void initGame(){
    digitalWrite(grnPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(relayPin, LOW);
}

void setup() {
  // Set the pins to the correct mode
  pinMode(startPin, INPUT_PULLUP);
  pinMode(failPin, INPUT_PULLUP);
  pinMode(endPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  // Begin a serial connection for debugging
  Serial.begin(9600);
}

void loop() {
  //Serial.println(gameState);
  switch(gameState) {
   
    case GameState::IN_PROGRESS:
      if(!digitalRead(endPin)) {
        gameState = GameState::SUCCESS;
        Serial.println("Congratulations!");
        tone(buzzerPin, 440, 50);
        digitalWrite(grnPin, HIGH);
        delay(60);
        tone(buzzerPin, 587, 250);
        
      }
      else if(!digitalRead(failPin)) {
        gameState = GameState::FAILED;
        Serial.println("FAILED");
        tone(buzzerPin, 440, 200);
        digitalWrite(redPin, HIGH);
        //digitalWrite(relayPin, HIGH);
        delay(200);
        tone(buzzerPin, 415, 200);
        delay(200);
        tone(buzzerPin, 392, 200);
        delay(200);
        tone(buzzerPin, 370, 400);
        digitalWrite(relayPin, HIGH);
        digitalWrite(redPin, LOW);
        delay(200);
        digitalWrite(relayPin, LOW);
      }
      break;

    case GameState::FAILED:
      //Do nothing
      //Run Success Case to Check for Game Start
    case GameState::SUCCESS:
      int sensorValue = digitalRead(startPin);
      Serial.println("StartPin:" +  String(sensorValue));
      if(!digitalRead(startPin) & digitalRead(failPin)) {
        gameState = GameState::IN_PROGRESS;
        Serial.println("New Game Started");
        initGame();
        tone(buzzerPin, 440, 100);
        delay(120);
        tone(buzzerPin, 554, 100);
        digitalWrite(grnPin, HIGH);
        delay(120);
        tone(buzzerPin, 650, 200);
        digitalWrite(grnPin, LOW);
      }
      break;    
  }
  delay(5);
}


