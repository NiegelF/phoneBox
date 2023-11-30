#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the I2C address if necessary

const int increaseButtonPin = 2;  // Pin for increasing time button
const int decreaseButtonPin = 3;  // Pin for decreasing time button
const int startButtonPin = 4;     // Pin for starting timer button
int ResetPin = 6;
unsigned long timerDuration = 0;   // Timer duration in milliseconds
unsigned long startTime = 0;       // Start time of the timer
boolean timerRunning = false;      // Timer state

void setup() {
  digitalWrite(ResetPin, HIGH); // Set digital pin to 5V
  pinMode(ResetPin, OUTPUT); // Set the digital pin to an OUTPUT pin
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(increaseButtonPin, INPUT_PULLUP);
  pinMode(decreaseButtonPin, INPUT_PULLUP);
  pinMode(startButtonPin, INPUT_PULLUP);   
  lcd.print("Timer: 00:00:00");
}

void loop() {
  if (!timerRunning) {
    handleButtonPresses();
  } else {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    
    if (elapsedTime >= timerDuration) {
      timerRunning = false;
      lcd.clear();
      lcd.print("Timer Done!");
      myservo.write(30);              // tell servo to go to position in variable 'pos'
      delay(5000);
      digitalWrite(ResetPin, LOW); // This command grouunds the reset pin and reboots the Arduino

    } else {
      updateTimerDisplay(timerDuration - elapsedTime);
    }
  }
}

void handleButtonPresses() {
  if (digitalRead(increaseButtonPin) == LOW) {
    timerDuration += 1800000;  // Increase by 30 minutes
    updateTimerDisplay(timerDuration); // Update display without starting timer
    delay(200);
  }

  if (digitalRead(decreaseButtonPin) == LOW) {
    if (timerDuration >= 30 * 60 * 1000) {
      timerDuration -= 1800000;  // Decrease by 30 minutes
      updateTimerDisplay(timerDuration); // Update display without starting timer
    }
    delay(200);
  }

  if (digitalRead(startButtonPin) == LOW) {
    if (timerDuration > 0) {
      myservo.attach(8);  // attaches the servo on pin 9 to the servo object
      myservo.write(360);              // tell servo to go to position in variable 'pos'
      delay(1000);
      startTime = millis();
      timerRunning = true;
      lcd.clear();  // Clear the entire display
      lcd.setCursor(0, 0);
    }
    delay(200);
  }
}

void updateTimerDisplay(unsigned long remainingTime) {
  Serial.println(remainingTime);
  int hours = (remainingTime  / 3600000) % 60;
  int minutes = (remainingTime  / 60000) % 60;
  int seconds = (remainingTime  / 1000) % 60;
  Serial.println(minutes);
  lcd.setCursor(7, 1);  // Move to the second line
  lcd.print("                ");  // Clear the line
  lcd.setCursor(7, 0);
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
}

