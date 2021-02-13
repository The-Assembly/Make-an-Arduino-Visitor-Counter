#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

int buzzerPin = 6;

int trigPin1 = 8;
int echoPin1 = 9;

int trigPin2 = 10;
int echoPin2 = 11;

String sequence = "";

int timeoutCounter = 0;
int currentPeople = 0;
int maxPeople = 5;

int distance1, distance2, initialDistance1, initialDistance2;

int measureDistance(int trigPin, int echoPin){
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   int duration = pulseIn (echoPin, HIGH);
   return (duration/2) / 29.1;
}

void displayText(String txt){
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(txt);
  display.display(); 
}
  
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  displayText("Visitor Counter");
  
  pinMode(buzzerPin, OUTPUT); // declare piezo as output
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  Serial.begin(9600);

  //initial readings
  initialDistance1 = measureDistance(trigPin1, echoPin1);
  initialDistance2 = measureDistance(trigPin2, echoPin2);
}

void loop(){
  delay(200);// reading will be taken after 200 milliseconds

  distance1 = measureDistance(trigPin1, echoPin1);
  distance2 = measureDistance(trigPin2, echoPin2);

  //Process the data
  if(distance1 < initialDistance1 - 30 && sequence.charAt(0) != '1'){
    sequence += "1";
  }else if(distance2 < initialDistance2 - 30 && sequence.charAt(0) != '2'){
    sequence += "2";
  }
  
  if(sequence.equals("12")){
    currentPeople++;  
    sequence="";
    delay(550);
  }else if(sequence.equals("21") && currentPeople > 0){
    currentPeople--;
    sequence="";
    delay(550);
  }

  //Resets the sequence if it is invalid or timeouts
  if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200){
    sequence="";
  }

  if(sequence.length() == 1){ //
    timeoutCounter++;
  }else{
    timeoutCounter=0;
  }
 //Print values to serial
  Serial.print("Seq: ");
  Serial.println(sequence);
  Serial.print("S1: ");
  Serial.println(distance1);
  Serial.print("S2: ");
  Serial.println(distance2);
  Serial.print("Current people: ");
  Serial.println(currentPeople);
  
  displayText(String(currentPeople));
      
  //If the number of people is high, trigger the buzzer
  if(currentPeople > maxPeople){
    tone(buzzerPin, 1, 500);
    displayText(String(currentPeople));
  }else{
    noTone(buzzerPin);
  }

}
 

  
  
  


  
  
