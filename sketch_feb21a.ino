#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 53         // example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 10        // using digital pin 4 on Arduino Nano 328, can use other pins
#include <TMRpcm.h>                 // also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;                      // create an object for use in this sketch

// Ultrasonic sensor pins
int trigpin1 = 2;
int echopin1 = 3;
int trigpin2 = 4;
int echopin2 = 11;
long time1;
int cm1;
long time2;
int cm2;

// Motion sensor pins
int motionin = 6;
int motionout;

// IR sensor pins
int irin = 7;
int irout;
int count;
int counttotal;

void setup() {
  Serial.begin(9600);

  // Ultrasonic sensor pins
  pinMode(13, OUTPUT);              // LED indicator pin
  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin2, INPUT);

  // Motion sensor pins
  pinMode(motionin, INPUT);

  // IR sensor pins
  pinMode(irin, INPUT);

  tmrpcm.speakerPin = 5;            // 5, 6, 11 or 46 on Mega, 9 on Uno, Nano, etc

  Serial.println("speakerPin");
  
  // Initialize SD card
  if (!SD.begin(SD_ChipSelectPin)) {  
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }

  Serial.println("set volume");
  tmrpcm.setVolume(4);              // Set volume
  
  Serial.println("play");
  // tmrpcm.play("Not ok because there is a man within 5 meters.wav");  // Uncomment this line if you want to play a specific sound file
}

void loop() { 
  tmrpcm.play("five.wav");  // Play a sound

  // Ultrasonic sensor - 180°
  digitalWrite(trigpin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1, LOW);
  time1 = pulseIn(echopin1, HIGH);
  cm1 = (time1 / 2) * 0.0343;

  // Ultrasonic sensor - 45°
  digitalWrite(trigpin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2, LOW);
  time2 = pulseIn(echopin2, HIGH);
  cm2 = (time2 / 2) * 0.0343;

  // Motion sensor
  motionout = digitalRead(motionin);

  // IR sensor
  irout = digitalRead(irin);

  if (irout == 0) {
    count = 0;
  } 
  else if (irout == 1) {
    count = 1;
  } 
  else if (irout == 2) {
    count = 2;
  } 
  else if (irout == 3) {
    count = 3;
  } 
  else if (irout == 4) {
    count = 4;
  } 
  else if (irout == 5) {
    count = 5;
  } 
  else {
    count = 6;
  }

  if (count == 0) {
    counttotal = 0;
  } 
  else if (0 < count <= 5) {
    counttotal = 1;
  } 
  else {
    counttotal = 2;
  }

  if (cm1 > 300 && cm2 > 100 && motionout == 0) {
    tmrpcm.play("ok, you can go now.wav");
    delay(1000);
  } 
  else if (cm1 > 300 && cm2 > 100 && motionout == 1) {
    tmrpcm.play("Not ok because there is a man within 5 meters.wav");
    delay(1000);
  } 
  else if (cm1 > 300 && cm2 < 100 && motionout == 0 ) {
    tmrpcm.play("not ok, because beneath you there is a small barrier.wav");
    delay(1000);
  } 
  else if (cm1 < 300 && cm2 > 150 && motionout == 0 ) {
    tmrpcm.play("not ok, because there is a small barrier within 3 meters distance.wav");
    delay(1000);
  } 
  else if (cm1 < 300 && cm2 > 150 && motionout == 1 ) {
    tmrpcm.play("not ok, because there is a barrier within 3 meters distance and some people are standing within 5 meters distance.wav");
    delay(1000);
  } 
  else if (cm1 < 300 && cm2 < 150 && motionout == 0 ) {
    //tmrpcm.play("two.wav");
    Serial.println("ok");
    delay(1000);
  } 
  else if (cm1 < 300 && cm2 > 150 && motionout == 1 ) {
    tmrpcm.play("not ok, because there is a barrier within 3 meters distance and some people are standing within 5 meters distance.wav");
    delay(1000);
  }

  // High people
  else if (cm1 > 300 && cm2 > 150 && motionout == 1 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else if (cm1 > 300 && cm2 > 150 && motionout == 0 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else if (cm1 > 300 && cm2 < 150 && motionout == 0 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else if (cm1 < 300 && cm2 > 150 && motionout == 0 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else if (cm1 < 300 && cm2 > 150 && motionout == 1 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else if (cm1 < 300 && cm2 < 150 && motionout == 1 && counttotal == 2) {
    tmrpcm.play("not ok, because this place is crowded.wav");
  } 
  else {
    tmrpcm.play("not ok, because beneath you there
