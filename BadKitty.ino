#include <HCSR04.h>
#include <Servo.h>

#define SPRAY_POS 45
#define SPRAY_DELAY 5

const int triggerPin = 7;
const int echoPin = 8;
const int servoPin = 9;

int pos = 0;

Servo sprayServo;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(5);
  }

  HCSR04.begin(triggerPin, echoPin);

  sprayServo.attach(servoPin);
  sprayServo.write(90);

  delay(1000);
}

void loop() {
  double* distCm = HCSR04.measureDistanceCm();

  Serial.print(F("distCm: "));
  Serial.println(distCm[0]);

  spray();
  while (true) {
    delay(1000);
  }

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
