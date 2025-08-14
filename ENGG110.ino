#include <Wire.h>
#include <MPU6050.h>
#include <IRremote.h>

class Motor {
  public: 
    char* direction;
    int code;
    bool isOn;
    void (*turnOn)();
    void (*turnOff)();

    Motor(char* motorName, int motorCode, bool motorIsOn, void(*MotorTurnOn)(), void(*MotorTurnOff)()) {
      direction = motorName;
      code = motorCode;
      isOn = motorIsOn;
      turnOn = MotorTurnOn;
      turnOff = MotorTurnOff;
    }
};

// make obj struct for variable where button down data is stored, this is in name and pressed down preoprties to help with iteration 
#define IR_RECEIVE_PIN 15
// 15

// create variable to compare duration of time between button presses
unsigned long lastPress = 0;

// up
#define BOARD1AIN1 18 // board 1 is the right board
#define BOARD1AIN2 19
// right
#define BOARD1BIN1 17
#define BOARD1BIN2 5
// down
#define BOARD2AIN1 27
#define BOARD2AIN2 14
// left
#define BOARD2BIN1 26
#define BOARD2BIN2 25

// functions to run motors depending on direction. Also for each direction, both motors in the same axis turn on in clockwise or anticlockwise to help move the structure
void runUp() {
  digitalWrite(BOARD1AIN1, HIGH);  
  digitalWrite(BOARD1AIN2, LOW);
  digitalWrite(BOARD2AIN1, LOW);  
  digitalWrite(BOARD2AIN2, HIGH);   
}
void runDown() {
  digitalWrite(BOARD1AIN1, LOW);  
  digitalWrite(BOARD1AIN2, HIGH);
  digitalWrite(BOARD2AIN1, HIGH);  
  digitalWrite(BOARD2AIN2, LOW);   
}
void runRight() {
  digitalWrite(BOARD1BIN1, HIGH);  
  digitalWrite(BOARD1BIN2, LOW);
  digitalWrite(BOARD2BIN1, LOW);  
  digitalWrite(BOARD2BIN2, HIGH);   
}
void runLeft() {
  digitalWrite(BOARD1BIN1, LOW);  
  digitalWrite(BOARD1BIN2, HIGH);
  digitalWrite(BOARD2BIN1, HIGH);  
  digitalWrite(BOARD2BIN2, LOW); 
}

// functions to turn off motors
void offUp() {
  digitalWrite(BOARD1AIN1, LOW);  
  digitalWrite(BOARD1AIN2, LOW);
  digitalWrite(BOARD2AIN1, LOW);  
  digitalWrite(BOARD2AIN2, LOW);   
}
void offDown() {
  digitalWrite(BOARD1AIN1, LOW);  
  digitalWrite(BOARD1AIN2, LOW);
  digitalWrite(BOARD2AIN1, LOW);  
  digitalWrite(BOARD2AIN2, LOW);   
}
void offRight() {
  digitalWrite(BOARD1BIN1, LOW);  
  digitalWrite(BOARD1BIN2, LOW);
  digitalWrite(BOARD2BIN1, LOW);  
  digitalWrite(BOARD2BIN2, LOW);   
}
void offLeft() {
  digitalWrite(BOARD1BIN1, LOW);  
  digitalWrite(BOARD1BIN2, LOW);
  digitalWrite(BOARD2BIN1, LOW);  
  digitalWrite(BOARD2BIN2, LOW); 
}

Motor motors[] = {
  Motor("up", 24, false, runUp, offUp),
  Motor("down", 82, false, runDown, offDown),
  Motor("right", 90, false, runRight, offRight),
  Motor("left", 8, false, runLeft, offLeft),
};

void setup() {
  // activate all pins that are being used
  pinMode(BOARD1AIN1, OUTPUT);  
  pinMode(BOARD1AIN2, OUTPUT);  
  pinMode(BOARD1BIN1, OUTPUT);  
  pinMode(BOARD1BIN2, OUTPUT);  
  pinMode(BOARD2AIN1, OUTPUT);  
  pinMode(BOARD2AIN2, OUTPUT);  
  pinMode(BOARD2BIN1, OUTPUT);  
  pinMode(BOARD2BIN2, OUTPUT);  
  IrReceiver.begin(IR_RECEIVE_PIN);  // Start IR receiver
}

void loop() {
  // when button pressed set buttonDown of respected direction that has been pressed to true
  if(IrReceiver.decode()){
    uint8_t message = IrReceiver.decodedIRData.command;
    // Serial.println(message);

    for (int i = 0; i < 4; i++) {
      if(motors[i].code == message && !motors[i].isOn) {
        motors[i].turnOn();
        motors[i].isOn = true;
      } 
    }
    if (message == 28) {
      for (int i = 0; i < 4; i++) {
      motors[i].turnOff();
      motors[i].isOn = false;
      }

    }
    IrReceiver.resume();
  }	
}


