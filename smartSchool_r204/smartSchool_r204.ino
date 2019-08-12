#include <ArduinoJson.h>
#include <SocketIoClient.h>
#include <ESP8266WiFiMulti.h>
#include <Servo.h> 
#include "SSD1306.h"
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;



/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "SuBee Team"
#define WLAN_PASS       "Subee@123"


#define QUAT 16  //D0
#define DENTREN 5   //D1
#define DENDUOI 4   //D2
#define CUA 0  //D3
#define QUATDUOI 2  //D4

//#define KHOACUA 12  //D6
//#define NULL 13   //D7
//#define NULL 15   //D8
Servo Servo1;
const char* maphong="R204";
bool light1,light2,fan,door,Status;
const char* room;
void event1(const char * payload, size_t length) {

 USE_SERIAL.println(payload);
const size_t capacity = JSON_ARRAY_SIZE(4) + 8*JSON_OBJECT_SIZE(1) + 5*JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(9) + 940;
DynamicJsonBuffer jsonBuffer(capacity);

const char* json = payload;

JsonArray& root = jsonBuffer.parseArray(json);

JsonObject& root_3 = root[3];
 Status = root_3["status"]; // true
const char* root_3__id = root_3["_id"]; // "5d3f1f091c9d4400001d30f8"
 room= root_3["nameRoom"]; // "LT201"

 light1 = root_3["light"]["light1"]; // false
 light2 = root_3["light"]["light2"]; // false

 fan = root_3["fan"]["fan1"]; // true

 door = root_3["door"]["door1"]; // "false"

 USE_SERIAL.print("den1:");
  USE_SERIAL.println(light1);
  USE_SERIAL.print("den2:");
   USE_SERIAL.println(light2);
   USE_SERIAL.print("quat1:");
    USE_SERIAL.println(fan);
    USE_SERIAL.print("cua1:");
     USE_SERIAL.println(door);
  USE_SERIAL.println(room);

     
}
void mocua(bool trangthai){
  if(trangthai==true){
  
    Servo1.write(110);     
   
  }
}
void dongcua(bool trangthai){
  if(trangthai==false){
 
    Servo1.write(0);     
    
  }
}
void batdentren(bool trangthai){
  if(!trangthai==true){
    digitalWrite(DENTREN,HIGH);
  }else{
    digitalWrite(DENTREN,LOW);
  }
}
void batdenduoi(bool trangthai){
  if(!trangthai==true){
    digitalWrite(DENDUOI,HIGH);
  }else{
    digitalWrite(DENDUOI,LOW);
  }
}
void batquat(bool trangthai){
  if(!trangthai==true){
    digitalWrite(QUAT,HIGH);
  }else{
    digitalWrite(QUAT,LOW);
  }
}

void bathethong(bool trangthai){
  if(!trangthai==true){
      mocua(true);
     batdentren(true);
     batdenduoi(true);
     batquat(true);
  }
}
void setup() {
    USE_SERIAL.begin(115200);
  
    USE_SERIAL.setDebugOutput(true);
  Servo1.attach(CUA); 
  pinMode(DENTREN,OUTPUT);
  pinMode(DENDUOI,OUTPUT);
  pinMode(QUAT,OUTPUT);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP(WLAN_SSID,  WLAN_PASS);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.on("event", event1);
    webSocket.begin("smartroomserver.herokuapp.com");
    // use HTTP Basic Authorization this is optional remove if not needed
  

}

void loop() {
    webSocket.loop();
       webSocket.on("changeRoom",event1);
       

  if(Status==true && strcmp(room,maphong)==0){
      mocua(door);
      dongcua(door);
     batdentren(light1);
     batdenduoi(light2);
     batquat(fan);
 
  
 }
 
}
