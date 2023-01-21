/* ******************** * * *
      Stepper Example
      Supporting A4988 Stepper Driver
      LCD 16x2 I2C
      

 
 By: Gal Arbel
 2023
*/

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // on 0x27 i2c address, 16 by 2

class Stepexample {

  private:
  int speed = 1200;  // this is the delay in uSec between the stepper pulses
  byte stepPin = 0;
  byte dirPin = 0;
  byte xlimPin = 0;
  int xlocation = 0;

  void movefromedge(char direction){
    if (direction == 'l'){
        move(35);
        xlocation =- 35;
        Serial.println("hitting right edge!");
      }
      if (direction == 'r'){
        move(35);
        xlocation =+ 35;
        Serial.println("hitting left edge!");

   }

  }

  public:
    Stepexample(int stepPin, int dirPin, int xlimPin, int speed) {
      this->stepPin = stepPin;
      this->dirPin = dirPin;
      this->xlimPin = xlimPin;
      this->speed = speed;
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      pinMode(xlimPin, INPUT_PULLUP);
  }
  void begin(double bdrate) {
    Serial.begin(bdrate);
    lcd.init();  // on 0x27 i2c address, 16 by 2
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(" Initiated!");
    lcd.setCursor(1, 1);
    lcd.print("ready.......");
  }

  void move(int x) {  //move horizontally, measured in steps
    Serial.println("dx activated");
    Serial.print("Delta X =");
    Serial.print(x);
    Serial.println(" steps");

    if (x > 0) {
      digitalWrite(dirPin, HIGH);  // go CW. Makes 200 pulses for making one full cycle rotation
      for (int c = 0; c < x; c++) { // a loop to produce pulses for the motor
        digitalWrite(stepPin, HIGH); // that's a HIGH pulse
        delayMicroseconds(speed);   // and its duration
        digitalWrite(stepPin, LOW); // that's a LOW pulse
        delayMicroseconds(speed);   // and its duration
        xlocation++;
        if (!digitalRead(xlimPin)) {
          Serial.println("limit X reached");
          movefromedge('l');
          break;
        }
      }
    }
    if (x < 0) {
      digitalWrite(dirPin, LOW);  // go CCW
      for (int c = 0; c < abs(x); c++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(speed);
        xlocation--;
        if (!digitalRead(xlimPin)) {
          Serial.println("limit X reached");
          movefromedge('r');
          break;
        }
      }
    }
  }
  void movemm(int x) {  //in mm
    x = x / 0.157;      // the resolution at full steps
    move(x);
  }
  void display() {  // print to lcd
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Steps  | in mm");
    lcd.setCursor(2, 1);
    lcd.print(xlocation);
    lcd.setCursor(8, 1);
    lcd.print("|");
    lcd.setCursor(10, 1);
    lcd.print((xlocation * 0.157));
  }
  int locx();  // returns the current X location
  int locy();  // returns the current Y location
};
//Stepexample mystepper = Stepexample(3, 4,5);

Stepexample mystepper(3, 2, 5, 1000);//stepPin, dirPin, xlimPin, speed (usec delay between pulses)

void setup() {
  mystepper.begin(115200);
  Serial.println("Started");
}

void loop() {
  mystepper.movemm(100);
  mystepper.display();
  delay(1000);
  mystepper.movemm(-100);
  mystepper.display();
  delay(1000);
}
