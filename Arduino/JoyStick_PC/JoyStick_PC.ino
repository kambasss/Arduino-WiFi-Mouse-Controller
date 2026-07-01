#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "Arduino_LED_Matrix.h"

//---- Wifi info ----//
const char ssid[] = ""; //Your WiFi ssid
const char pswd[] = ""; //Your WiFi password


//---- PC info ----//
const char pc_ip[] = ""; //Your PC IP
const int pc_port = 5000;

WiFiUDP Udp;
ArduinoLEDMatrix matrix;

//---- PINS ----//
const int VRX = A0;
const int VRY = A1;
const int SW = 3;



//---- Frames for Matrix ----//

byte arrowRight[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowLeft[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowUp[8][12] = {
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowDown[8][12] = {
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowUpRight[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowUpLeft[8][12] = {
  { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowDownRight[8][12] = {
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte arrowDownLeft[8][12] = {
  { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte centerDot[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};


void setup() {
  Serial.begin(115200);

  pinMode(SW, INPUT_PULLUP); // when pressed zero
  matrix.begin();

  Serial.print("Connecting to WiFi: "); //Messages to Serial
  Serial.println(ssid);
  WiFi.begin(ssid, pswd); //Start Connection
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
  }

  Serial.println("\nConnected!");
  Udp.begin(1234); //create random port

}

void loop() {
  
  //---- X-Y-SW Values ----//
  int xVal = analogRead(VRX);
  int yVal = analogRead(VRY);
  int swVal = digitalRead(SW);

  int dx = 0;
  int dy = 0;

  //---- dx Deadzone  ----//
  if(xVal < 460){

    dx = map(xVal, 460, 0, 0, 30);

  }else if(xVal > 560){

    dx = map(xVal, 560, 1023, 0, -30);
  }


  //---- dy Deadzone  ----//
  if(yVal < 460){
  
   dy = map(yVal, 460, 0, 0, 30); 

  }else if(yVal> 560){ 
  
  dy = map(yVal, 560, 1023, 0, -30);

  }

  //---- Direction ----//

  if (dx == 0 && dy == 0) {
    matrix.renderBitmap(centerDot, 8, 12);
  } 
  else if (dx > 0 && dy == 0) {
    matrix.renderBitmap(arrowRight, 8, 12);
  } 
  else if (dx < 0 && dy == 0) {
    matrix.renderBitmap(arrowLeft, 8, 12);
  } 
  else if (dx == 0 && dy > 0) {
    matrix.renderBitmap(arrowDown, 8, 12);
  } 
  else if (dx == 0 && dy < 0) {
    matrix.renderBitmap(arrowUp, 8, 12);
  }
  // Diagonal
  else if (dx > 0 && dy > 0) {
    matrix.renderBitmap(arrowDownRight, 8, 12);
  }
  else if (dx < 0 && dy > 0) {
    matrix.renderBitmap(arrowDownLeft, 8, 12);
  }
  else if (dx > 0 && dy < 0) {
    matrix.renderBitmap(arrowUpRight, 8, 12);
  }
  else if (dx < 0 && dy < 0) {
    matrix.renderBitmap(arrowUpLeft, 8, 12);
  }


  String payload = String(dx) + "," + String(dy) + "," + String(swVal); //payload to PC

  Udp.beginPacket(pc_ip, pc_port);
  Udp.print(payload);
  Udp.endPacket();

  
}
