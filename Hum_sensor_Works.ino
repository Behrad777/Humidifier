#include "Adafruit_SHT31.h"
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x7C,0x87,0xCE,0xE3,0x53,0x5C};




typedef struct struct_message {
  float c;
} struct_message;


struct_message myData;


esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


Adafruit_SHT31 sht31 = Adafruit_SHT31();


void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_STA);


if (! sht31.begin(0x44)) {  
    Serial.println("Check circuit. SHT31 not found!");
    while (1) delay(1);
  }

if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    Serial.println("tp3");
    return;
  }


esp_now_register_send_cb(OnDataSent);



memcpy(peerInfo.peer_addr, broadcastAddress, 6);
Serial.println("tp6");
peerInfo.channel = 0;  
peerInfo.encrypt = false;


if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}// end setup



void loop() {
  float hum = sht31.readHumidity();


  if (! isnan(hum)) {  
    Serial.print("Humidity(%): ");
    Serial.println(hum);
    //send the humidity to the other esp32
  } else {
    Serial.println("Failed to read humidity!");
  }

  myData.c = hum;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));


  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }


  delay(1000*60*10);
}//end loop
