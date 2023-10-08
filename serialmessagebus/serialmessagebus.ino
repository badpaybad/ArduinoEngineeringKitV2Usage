#include <ArduinoMotorCarrier.h>
#define INTERRUPT_PIN 6
#include <Arduino_LSM6DS3.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  Serial.setTimeout(1);
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
}

String lastMsg = "";

int direction = 1;
void loop() {
  //https://pijaeducation.com/data-type-conversion-arduino/
  // put your main code here, to run repeatedly:
  // Serial.write(45); // send a byte with the value 45
int t0= millis();
String s=String(t0);
  Serial.write("hi777777777777777777777777777 at: ");
  Serial.write(s.c_str());
  Serial.write("\r\n");

  if (lastMsg != "") {
    const char* teststr = lastMsg.c_str();
    int bytesSent = Serial.write(teststr);  //send the string "hello" and return the length of the string.


    servo3.setAngle(60 - direction * 30);

    lastMsg = "";
  }
  //servo3.setAngle(60 - direction * 30);

  int t1 = millis();
  lastMsg = Serial.readString();
  //Serial.write("hi777777777777777777777777777");
  //servo3.setAngle(60 - direction * 30);
  //M1.setDuty(60 - direction * 30);
  direction = direction * -1;
  int t2 = millis();

  //Serial.println(t2 - t1);
  // while (Serial.available() == 0) {}     //wait for data available
  // String teststr = Serial.readString();  //read until timeout
  // teststr.trim();                        // remove any \r \n whitespace at the end of the String
  // if (teststr == "red") {
  //   Serial.println("A primary color");
  // } else {
  //   Serial.println("Something else");
  // }

  //delayMicroseconds(1000000);
  delay(1);
}
