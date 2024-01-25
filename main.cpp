#include <Arduino.h>

void updateSerial();
void test_sim800_module();
void send_SMS();


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(3000);
  Serial.println("Starting...");
  test_sim800_module();
  send_SMS();
}
void loop() {
  updateSerial();
}
void test_sim800_module()
{
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
void send_SMS()
{
  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+989127176496\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  Serial2.print("Circuit Digest"); //text content
  updateSerial();
Serial.println();
  Serial.println("Message Sent");
  Serial2.write(26);
}