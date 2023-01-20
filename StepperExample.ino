/* ******************** * * *
      Stepper Example
      Supporting A4988 Stepper Driver
      LCD 16x2 I2C
      

 
 By: Gal Arbel
 2023
*/

class Stepexample {
  
  private:
    int speed = 1100;
    byte stepPin = 0;
    byte dirPin = 0;
    byte xlimPin = 0;

    void movefromedge(char axis, char direction);
    int xlocation = 0;

  public:
    Stepexample(int stepPin, int dirPin, int xlimPin) {
      this->stepPin = stepPin;
      this->dirPin = dirPin; 
      this->xlimPin = xlimPin;
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      pinMode(xlimPin, INPUT_PULLUP);
    } 
    void begin(double bdrate){
      Serial.begin(bdrate);
    }
    
    void move(int x){//move horizontally, measured in steps 
      Serial.println("dx activated");
      Serial.print("Delta X =");
      Serial.println(x);
          
      if (x > 0){
        digitalWrite(dirPin, HIGH); // go CW. Makes 200 pulses for making one full cycle rotation
        for (int c = 0; c < x; c++){
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(speed); 
          digitalWrite(stepPin,LOW);  
          delayMicroseconds(speed); 
          xlocation++;
          if (!digitalRead(xlimPin)) {
            Serial.println("limit X reached"); 
            //movefromedge('x', 'l'); 
              break;
            }
        }
      }
      if (x < 0){
          digitalWrite(dirPin, LOW); // go CCW
          for(int c = 0; c < abs(x) ; c++) {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(speed);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(speed);    
          xlocation--;  
          if (!digitalRead(xlimPin)) {
            Serial.println("limit X reached"); 
            //movefromedge('x', 'r'); 
              break;
            }
        }
      }
    }    
    void movemm(int x){ //in mm 
      x = x / 0.157; // the resolution at full steps
      move(x);
    }
    void display(const char*){ // print to lcd

    }
    int locx(); // returns the current X location
    int locy(); // returns the current Y location
 
};
//Stepexample mystepper = Stepexample(3, 4,5);

Stepexample mystepper(3, 2, 5);

void setup() {
 mystepper.begin(115200);
 Serial.println("Started");
}

void loop() {
mystepper.movemm(100);
delay(1000);
mystepper.movemm(-100);
delay(1000);
}
