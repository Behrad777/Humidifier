
#include <esp_now.h>
#include <WiFi.h>

int relayPin = 2;


//making the message struct 
typedef struct struct_message {
    float c;
} struct_message;

struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("humidity: ");
    Serial.println(myData.c);

    if(myData.c <55.0){
      digitalWrite(relayPin, LOW); //turns the relay on 
    } 
    //dead zone for 55 to 56 
    else if (myData.c > 56.0){
      digitalWrite(relayPin, HIGH); //turns the relay off
    } else{
      //do nothing 
    }

}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(relayPin, OUTPUT);
}//setup end

void loop() {
  delay(1000*60*10);
}
