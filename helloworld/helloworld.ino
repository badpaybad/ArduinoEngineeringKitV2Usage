#include <ArduinoMotorCarrier.h>
#define INTERRUPT_PIN 6
#include <Arduino_LSM6DS3.h>

//Variable to store the battery voltage
static float batteryVoltage;
//low battery limit (discharged)
static float batteryLimit;
//Variable to change the motor speed and direction
int duty = 50;

int degreesX = 0;

int degreesY = 0;
float x, y, z;

void setup() {
  //Serial port initialization
  Serial.begin(115200);
  while (!Serial)
    ;
  IMU.begin();
  //Establishing the communication with the motor shield
  if (controller.begin()) {
    Serial.print("MKR Motor Shield connected, firmware version ");
    Serial.println(controller.getFWVersion());
  } else {
    Serial.println("Couldn't connect! Is the red led blinking? You may need to update the firmware with FWUpdater sketch");
    while (1)
      ;
  }

  // Reboot the motor controller; brings every value back to default
  Serial.println("reboot");
  controller.reboot();
  delay(500);

  //Take the battery status
  float batteryVoltage = (float)battery.getRaw() / 77;
  Serial.print("Battery voltage: ");
  Serial.println(batteryVoltage);

  M1.setDuty(0);
  M2.setDuty(0);
  M3.setDuty(0);
  M4.setDuty(0);

  batteryLimit = 9.50;  //Around 9V for a 3S LiPo battery
}
int direction = 1;

int encoderCount;

void loop() {

  //Take the battery status
  batteryVoltage = (float)battery.getRaw() / 77;

  //Reset to the default values if the battery levels are lower than 11V
  if (batteryVoltage < batteryLimit) {
    Serial.println(" ");
    Serial.println("WARNING: LOW BATTERY");
    Serial.println("ALL SYSTEMS DOWN");
    M1.setDuty(0);
    M2.setDuty(0);
    M3.setDuty(0);
    M4.setDuty(0);
    while (batteryVoltage < batteryLimit) {
      batteryVoltage = (float)battery.getRaw() / 77;
      controller.ping();
      //wait
      delay(50);
    }
  } else {
    //Motor test
    //M1.setDuty(25);
    //delay(50);

    float batteryVoltage = (float)battery.getRaw() / 77;
    //Serial.print("Battery voltage: ");
    //Serial.println(batteryVoltage);

    //servo3.setAngle(60- direction *30);

    direction = direction * -1;
  }

  if (IMU.accelerationAvailable()) {

    IMU.readAcceleration(x, y, z);

    if (x > 0.1) {

      x = 100 * x;

      degreesX = map(x, 0, 97, 0, 90);

      Serial.print("Tilting up ");

      Serial.print(degreesX);

      Serial.println("  degrees");
    }

    if (x < -0.1) {

      x = 100 * x;

      degreesX = map(x, 0, -100, 0, 90);

      Serial.print("Tilting down ");

      Serial.print(degreesX);

      Serial.println("  degrees");
    }

    if (y > 0.1) {

      y = 100 * y;

      degreesY = map(y, 0, 97, 0, 90);

      Serial.print("Tilting left ");

      Serial.print(degreesY);

      Serial.println("  degrees");
    }

    if (y < -0.1) {

      y = 100 * y;

      degreesY = map(y, 0, -100, 0, 90);

      Serial.print("Tilting right ");

      Serial.print(degreesY);

      Serial.println("  degrees");
    }

  }
  
    encoderCount = encoder1.getRawCount();
    Serial.print("encoderCount ");

    Serial.print(encoderCount);
    Serial.println("");

    //Keep active the communication MKR1000 & MKRMotorCarrier
    //Ping the samd11
    controller.ping();
    //wait
    delay(50);
}