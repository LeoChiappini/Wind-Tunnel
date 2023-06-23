// C++ code

#include <Servo.h> // Imports Servo library
#include <LiquidCrystal.h> // Imports LCD library

// Digital Pins

const int LEDPin = 6;
const int switchPin = 7;
const int powerPin = 8;
const int MOSFETPin = 9;
const int servoPin = 10;

// Analog Pins
const int fanPin = A0;
const int servoControlPin = A1;
const int temperaturePin = A2;

// LCD Pins
const int rs = 12;
const int textEntry = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

// Variables
int fanVal = 0;
int servoVal = 0;
int unit = 0; // Unit=0 means Celsius. Unit=1 means Farenheit.
int power = 0;

// Component Initializing
Servo shapeServo;

LiquidCrystal LCD(rs, textEntry, d4, d5, d6, d7);

void setup(){
  
  	// Sets up serial monitor and pins
  	Serial.begin(9600);
  	pinMode(switchPin, INPUT);
  	pinMode(temperaturePin, INPUT);
  	pinMode(LEDPin, OUTPUT);
    pinMode(MOSFETPin, OUTPUT);
  	
  
  	// Initializes LCD
  	LCD.begin(16, 2);
  	LCD.print("Hello, World!");

    shapeServo.attach(10);
  
  
}

void loop()	{
  
  // Checks the power status
  if(digitalRead(powerPin) == HIGH) {
    if (power == 0) {power = 1;}
	  else {
      power = 0;
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.write("Hello World!");
    }


	  Serial.println("Power Button Pressed");
    Serial.println(power);
  }

  // Checks if units must be changed
  if(digitalRead(switchPin) == HIGH) {
    if (unit == 0) {unit = 1;}
    else {unit = 0;}
    Serial.println("Unit Changed");
  }
  
  if (power == 1) {
    
    // Turns on power indicator LED
    digitalWrite(LEDPin, HIGH);
  
  	// Reads Fan and Servo analog values
  	fanVal = analogRead(fanPin);
  	servoVal = analogRead(servoControlPin);
  
  	Serial.println(fanVal); // Used for diagnosing Servo
  	
    // Maps analog values to respective digital values or angle
  	int motorSpeed = map(fanVal, 0, 1023, 255, 0);
  	int servoAngle = map(servoVal, 0, 1023, 180, 0);
    
     // Writes analog values to Fan and Servo
  	digitalWrite(MOSFETPin, motorSpeed);
    shapeServo.write(servoAngle);
  
    // Reads temperature sensor and calculates temperature in C
  	int sensorReading = analogRead(temperaturePin);
  	float temperature = calcTemperature(sensorReading);
  
  	// Writes the angle of attack to LCD
    // LCD.clear();
  	LCD.setCursor(0, 0);
  	LCD.print("Attack Angle:");
    
    if (servoAngle < 90) {
      LCD.print(servoAngle - 90);
    }
    else {
      LCD.setCursor(13, 0);
      LCD.write(" ");
      LCD.print(servoAngle - 90);
    }
  
  	// Writes the temperature to the LCD
  	LCD.setCursor(0, 1);
  	LCD.print("Temp: ");
    if (unit == 0) { // Checks for different units
      LCD.print(temperature);
  	  LCD.print(" C");
    }
    else if (unit == 1) {
      LCD.print(calcFarenheit(temperature));
      LCD.print(" F");
      
    }
  }
  
  if (power == 0) {
    
    // Turns on power indicator LED
    digitalWrite(LEDPin, LOW);
    
    // Turns off Fan and servo
    //analogWrite(MOSFETPin, 0);
    digitalWrite(MOSFETPin, LOW);
  	shapeServo.write(90);

    // Clears LCD and writes "Hello World"
    
  }
    
  delay(100); // Delay a little bit to improve simulation performance
}
  

float calcTemperature(int val) { return (((val / 1024.0) * 5.0) - 0.5) * 100; }

float calcFarenheit (float val) {return ((val * 9 / 5) + 32);}
  
  
 