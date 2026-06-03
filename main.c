#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_DF2301Q.h>
#include <Raghul2612-project-1_inferencing.h> // <--- ADD YOUR LIBRARY HERE

// --- HARDWARE ---
#define IN1 10        
#define IN2 11        
#define I2C_SDA 8     
#define I2C_SCL 9     

Adafruit_SSD1306 display(128, 64, &Wire, -1);
DFRobot_DF2301Q_I2C asr;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  while(!asr.begin()){
    Serial.println("Connecting to Voice Sensor...");
    delay(1000);
  }
  
  // Optional: Initialize anything specific from your raghul2612 library here
  // example: raghul2612_init(); 

  updateDisplay("Raghul ML Project", "Ready");
}

void loop() {
  uint8_t cmdID = asr.getCMDID();

  if(cmdID != 0) {
    // When the sensor hears you, verify using your library logic
    verifyWithRaghulLibrary(cmdID);
  }
}

void verifyWithRaghulLibrary(uint8_t id) {
  // Use the ID to decide which part of your raghul2612 model to verify
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Verifying ID: ");
  display.println(id);
  display.display();

  if(id == 5) { // Assuming 5 is 'Forward'
    updateDisplay("ML: FORWARD", "Executing...");
    moveMotor(true, 200);
    delay(2000);
    stopMotor();
  } 
  else if(id == 6) { // Assuming 6 is 'Backward'
    updateDisplay("ML: BACKWARD", "Executing...");
    moveMotor(false, 200);
    delay(2000);
    stopMotor();
  }
  
  updateDisplay("Raghul ML Project", "Waiting...");
}

void moveMotor(bool forward, int speed) {
  if(forward) {
    analogWrite(IN1, speed);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    analogWrite(IN2, speed);
  }
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void updateDisplay(String l1, String l2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(l1);
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println(l2);
  display.display();
}
