#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define enA 10//Enable1 L298 Pin enA 
#define in1 9 //Motor1  L298 Pin in1 
#define in2 8 //Motor1  L298 Pin in1 
#define in3 7 //Motor2  L298 Pin in1 
#define in4 6 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 
#define L_S A0 //ir sensor Left
#define R_S A1 //ir sensor Right
#define echo A2    //Echo pin
#define trigger A3 //Trigger pin
#define servo 11  //fitted to ultrasonic
#define servoArm 3      // Servo motor for arm 
#define SDA A4 // lcd i2c display SDA pin
#define SCL A5 // lcd i2c display SCL pin

int Set=20; //distance

int distance_L, distance_F, distance_R; 

LiquidCrystal_I2C lcd(0x27, 16, 2); //I2C LCD display

int table = 0;

void setup(){ 
Serial.begin(9600); 
pinMode(R_S, INPUT); // declare if sensor as input  
pinMode(L_S, INPUT); // declare ir sensor as input
pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  
pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 
analogWrite(enA, 115); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, 115); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 
pinMode(servoArm, OUTPUT);
pinMode(servo, OUTPUT);

// initialize lcd screen
  lcd.begin(16, 2);
  lcd.init();
  // turn on the backlight
  lcd.backlight();

  //Clear the screen 
  lcd.clear();

  // welcome message
  lcd.setCursor(5, 0);
  lcd.print("Hello!");
  lcd.setCursor(2, 1);
  lcd.print("I am Bella !");
  delay(800);

  //FIND THE POSITION
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
distance_F = Ultrasonic_read();
delay(50);

servoPulse(servoArm, 90);

}


void loop(){  
distance_F = Ultrasonic_read();
Serial.print("D F=");Serial.println(distance_F);

line();

}

void line(){
  //if Right Sensor and Left Sensor are at White color then it will call forword function
 if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){
  if(distance_F > Set){
    forword();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moving to Table");
  }
  else{
    Stop();
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 4);
    lcd.print("Stoped...!");
    delay(100);
    lcd.setCursor(0, 1);
    lcd.print("Cannot Move...!");
    Check_side();}  
 }  
 
//if Right Sensor is Black and Left Sensor is White then it will call turn Right function
else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnRight();}  
//if Right Sensor is White and Left Sensor is Black then it will call turn Left function
else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnLeft();} 
else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){
  //if Right Sensor is Black and Left Sensor is Black then it will call stop function
  Stop();
  delay(1000);

  //display arrivals
  //ArrivedPrints();
  //serving
  serveFood();
  //move to next checking
  checking();
}
delay(10);
}


void serveFood() {
  // Move arms to serve food
  moveArmsToServeFood();
  delay(3000); // Wait for serving action
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Here's your Order");
  lcd.setCursor(0,1);
  lcd.print("Enjoy your meal!");
  // Move arms back to initial position
  moveArmsToInitialPosition();
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank You.!");
  delay(800);
}
void moveArmsToServeFood() {
  servoPulse(servoArm, 45); // Adjust as needed
}

void moveArmsToInitialPosition() {
  servoPulse(servoArm, 90); // Adjust as needed
}

void checking(){
  table= table + 1;

  if(table==1){
    path1();
    turnAround();
    Stop();
    delay(3000);
  }
  else if(table==2){
    path2();
    turnAround2();
    Stop();
    delay(3000);
  }
  else if(table==3){
    path3();
    turnAround();
    Stop();
    delay(3000);
  }
}

void turnAround(){
  turnLeft();
  delay(500);
  turnLeft();
  delay(500);
  turnLeft();
  delay(500);
  turnLeft();
  delay(500);
  turnLeft();
  delay(500);
  turnLeft();
  delay(800);
}
void turnAround2(){
  turnRight();
  delay(500);
  turnRight();
  delay(500);
  turnRight();
  delay(500);
  turnRight();
  delay(500);
  turnRight();
  delay(500);
  turnRight();
  delay(800);
}
void path1(){
  //back to table 01
    backword();
    delay(600);
    Stop();
    delay(100);
    turnLeft();
    delay(1000);
    turnLeft();
    delay(300);
    forword();
    delay(400);
    Stop();
    delay(5000);
}
void path2(){
  //back to kitchen from table 02
  backword();
  delay(750);
  Stop();
  delay(5000);
}
void path3(){
  //back to kitchen from table 03
    backword();
    delay(600);
    Stop();
    delay(100);
    turnRight();
    delay(1000);
    turnRight();
    delay(300);
    forword();
    delay(400);
    Stop();
    delay(5000);
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}
//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger , LOW);
  
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}
void compareDistance(){
    if(distance_L > distance_R){
  //display
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Avoiding");
  lcd.setCursor(5, 1);
  lcd.print("Obstacle");
  delay(800);

  Stop();
  delay(500);
  backword();
  delay(500);
  Stop();
  delay(200);
  turnLeft();
  delay(1000);
  turnLeft();
  delay(500);
  forword();
  delay(500);
  turnRight();
  delay(1000);
  forword();
  delay(500);

  //display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Move On");
  delay(800);

  }
  else{
    //display
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Avoiding");
  lcd.setCursor(5, 1);
  lcd.print("Obstacle");
  delay(800);

  Stop();
  delay(500);
  backword();
  delay(500);
  Stop();
  delay(200);
  turnRight();
  delay(1000);
  turnRight();
  delay(500);
  forword();
  delay(600);
  turnLeft();
  delay(1000);
  forword();
  delay(600);

  //display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Move On");
  delay(800);
  }
}
void Check_side(){
    Stop();
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}
void forword(){  //forword
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}
void backword(){ //backword
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}
void turnRight(){ //turnRight
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}
void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}
void Stop(){ //stop
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}