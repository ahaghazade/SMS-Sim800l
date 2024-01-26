#include <Arduino.h>
#include <SimCommands.h>

int CommandAttempts = 3;
String MobilePhone = "09024674437";

void updateSerial();
void test_sim800_module();
void send_SMS();

void SendReqtoSim(String Command, String DesiredRes)
{
  boolean at_flag = 1;
  int Attempts = 0;
  while (at_flag && Attempts < CommandAttempts)
  {
    Attempts++;
    Serial.printf("\nSending %s  ...(%d)\n", Command.c_str(), Attempts);
    delay(10);
    Serial2.println(Command);
    delay(100);
    while (Serial2.available() > 0)
    {
      if (Serial2.find(DesiredRes.c_str())) at_flag = 0;
    }
    delay(1000);
  }
  if (at_flag == 1)
  {
    Serial.printf("Request %s : Failed!\n", Command.c_str());
    //restart ESP or Sim800
  }
  else Serial.printf("Request %s : Success.\n", Command.c_str());
}

void sendSMS(String MobilePhoneNum, String Payload)
{
  String PhoneNumPrefex = "\"" + MobilePhoneNum + "\"";
  // Set TextMode
  Serial.println("\nSetting Text mode");
  SendReqtoSim(SetTextMode, ATOK);
  delay(100);
  // Set Phone Number
  SendReqtoSim(SetPhoneNum + PhoneNumPrefex, SMSStart);
  //send payload
  Serial.printf("\nSending sms payload - %s - \n\n", Payload.c_str());
  Serial2.print(Payload);
  delay(100);
  //send ctrl+z
  boolean at_flag = 1;
  int Attempts = 0;
  while (at_flag && Attempts < CommandAttempts)
  {
    Attempts++;
    Serial2.println((char)26);
    Serial.printf("Sending Ctrl+z...(%d)\n", Attempts);
    delay(100);
    while (Serial2.available() > 0)
    {
      if (Serial2.find(SMSSent)) at_flag = 0;
    }
    delay(500);
  }
  delay(1000);
  Serial.println("SMS Send Status: Success");
}

void callMe(String Num)
{
  Serial2.print(F("ATD"));
  Serial2.print(Num);
  Serial2.print(F(";\r\n"));
}

void GSM_Init()
{
  Serial.println("Finding SIM800 GSM...");
  // Check connection
  SendReqtoSim(AT, ATOK);
  // ECO Disabling Mode
  // SendReqtoSim(EchoDisable, ATOK);
  // CPIN?
  SendReqtoSim(CheckPin, PinReady);
  //Irancell
  SendReqtoSim(IRANCELL, ATOK);
}


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  delay(3000);
  Serial.println("Starting...");
  // test_sim800_module();
  GSM_Init();
  delay(100);
  sendSMS(MobilePhone, "Hello Test");
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
  Serial2.println("AT+CSMP=17,167,0,8");
  updateSerial();
}
void updateSerial()
{
  delay(100);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}