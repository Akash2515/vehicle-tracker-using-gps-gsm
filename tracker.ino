 #include <SerialGSM.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

SerialGSM cell(2,3);

float lat, lng; 

TinyGPS gps;

String alertMessage = "missing";
String myPhoneNumber = "+919786493150";

void setup(){
  
  Serial.begin(9600);
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.DeleteAllSMS();
  cell.FwdSMS2Serial();
}


void loop() {
  if (cell.ReceiveSMS()>0) {
   Serial.print("Sender: ");
   Serial.println(cell.Sender());
   Serial.print("message: ");
   Serial.println(cell.Message());
   if(String(cell.Sender()) == myPhoneNumber && String(cell.Message()) == alertMessage)
    sendLocation();
   cell.DeleteAllSMS();
  }

  while(Serial.available()>0) {
    if(gps.encode(Serial.read())) {
      gps.f_get_position(&lat,&lng);
      
      Serial.println("Latitude:");
      
      Serial.println(lat);
      
      Serial.println("Longitude:");
      
      Serial.println(lng);
    }
  }
}

void sendLocation() {
      String tempStr = "Latitude: " + String(lat) + "\n" + "Longitude: " + String(lng);
      const char *mes = tempStr.c_str();
      const char *phoneTemp = myPhoneNumber.c_str();
      cell.Rcpt(phoneTemp);
      cell.Message(mes);
      cell.SendSMS();  
}

