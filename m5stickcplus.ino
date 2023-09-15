/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "Next-Guest";
const char * networkPswd = "";

// Port of udp server
const int udpPort = 3333;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
  // Initilize hardware serial:
  // Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setRotation(1);

  M5.Lcd.println("Being penis >:3");
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);

  delay(5000);
  
  xTaskCreatePinnedToCore(UdpThread, "UdpThread", 4096, NULL, 2, NULL, 1);
}

#define BUFFER_SIZE 255
char packetBuffer[BUFFER_SIZE];

void loop(){
  //only send data when connected
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.println(packetBuffer);
  //Wait for 1 second
  delay(1000);
}

void UdpThread(void *pvParameters) {
  for (;;) {
    if(connected){
      // Recieve packet
      int packetSize = udp.parsePacket();
      int length = udp.read(packetBuffer,BUFFER_SIZE);
      // M5.Lcd.println("Pp");
      // M5.Lcd.fillScreen(BLACK);
      // M5.Lcd.setCursor(0,0);
      // M5.Lcd.println(WiFi.localIP());
      // M5.Lcd.println("Len: " + length);
      vTaskDelay(100);
    }
  }
}

void connectToWiFi(const char * ssid, const char * pwd){
  M5.Lcd.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  M5.Lcd.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          //When connected set 
          M5.Lcd.print("WiFi connected! IP address: ");
          M5.Lcd.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(udpPort);
          connected = true;
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          M5.Lcd.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}