#include <HCSR04.h>
#include <Servo.h>

#define DETECT_DIST_CM 100.0
#define DETECT_TIMEOUT_SEC 10
#define DETECT_COUNT_SPRAY 5
#define SPRAY_POS 150
#define SPRAY_DELAY 5

const int triggerPin = 7;
const int echoPin = 8;
const int servoPin = 9;
const int ledPin = 13;

bool catDetected = false;
unsigned int detectLast = 0;
int detectCount = 0;
int detectTimeout = DETECT_TIMEOUT_SEC * 1000;

int pos = 0;

UltraSonicDistanceSensor distSensor(triggerPin, echoPin);
Servo sprayServo;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial.begin(115200);

  delay(1000);

  sprayServo.attach(servoPin);
  sprayServo.write(90);

  for (int x = 0; x < 10; x++) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    delay(100);
  }
}

void loop() {
  float distCm = distSensor.measureDistanceCm();
  Serial.print(F("distCm: "));
  Serial.println(distCm);

  if (distCm <= DETECT_DIST_CM) {
    detectLast = millis();
    detectCount++;
    digitalWrite(ledPin, LOW);
  }

  if (detectCount == DETECT_COUNT_SPRAY) {
    spray();
    detectCount = 0;
    digitalWrite(ledPin, HIGH);
  }

  else if ((millis() - detectLast) > detectTimeout) {
    detectCount = 0;
    digitalWrite(ledPin, HIGH);
  }

  Serial.print(F("detectCount: "));
  Serial.println(detectCount);

  delay(1000);
}

void spray() {
  Serial.print(F("Spraying..."));

  for (int x = 90; x < SPRAY_POS; x++) {
    sprayServo.write(x);
    delay(SPRAY_DELAY);
  }
  for (int x = SPRAY_POS; x > 90; x--) {
    sprayServo.write(x);
    delay(SPRAY_DELAY);
  }

  Serial.println(F("complete."));
}
