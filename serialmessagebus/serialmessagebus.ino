void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
}

void loop() {
  //https://pijaeducation.com/data-type-conversion-arduino/
  // put your main code here, to run repeatedly:
  // Serial.write(45); // send a byte with the value 45
  int bytesSent = Serial.write("-----hello\r\n");  //send the string "hello" and return the length of the string.

  if (Serial.available() == 0) {
    const char* teststr = ("Read then write:" + Serial.readString()).c_str();

    Serial.write(teststr);
  }
  // Serial.println("Enter data:");
  // while (Serial.available() == 0) {}     //wait for data available
  // String teststr = Serial.readString();  //read until timeout
  // teststr.trim();                        // remove any \r \n whitespace at the end of the String
  // if (teststr == "red") {
  //   Serial.println("A primary color");
  // } else {
  //   Serial.println("Something else");
  // }

  delay(1000);
}
