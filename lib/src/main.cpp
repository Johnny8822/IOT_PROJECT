#include <Arduino.h>
#include <WiFi.h> 
#include <ArduinoJson.h>  
#include <HTTPClient.h>  
#include <DallasTemperature.h> 
#include <OneWire.h> 
#include "env.h" 

const int temp_sensor_pin = 4; 
const int PIR_pin = 15; 
const int light_pin = 22; 
const int fan_pin = 23;

OneWire oneWire(temp_sensor_pin);  
DallasTemperature sensors(&oneWire); 



void getOutputDevices(){ 
  HTTPClient http;  
  http.begin(endpoint); 
  String RequestBody;   

  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String responseBody = http.getString(); 
    Serial.println(responseBody); 
    
    JsonDocument doc; 

    DeserializationError error = deserializeJson(doc, responseBody); 

    if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return; 
  } 
    bool fan_state = doc["fan"];  
    digitalWrite(fan_pin, fan_state); 
    
    bool light_state = doc["light"]; 
    digitalWrite(light_pin, light_state); 
  } 
  http.end();
}

// put function declarations here:
void getLight(){
  
  HTTPClient http;  
  http.begin(endpoint); 
  
  String RequestBody;  

 int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String responseBody = http.getString(); 
    Serial.println(responseBody); 
    
    JsonDocument doc; 

    DeserializationError error = deserializeJson(doc, responseBody); 

    if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return; 
  } 

     bool light_state = doc["light"]; 
    digitalWrite(light_pin, light_state); 
  } 
  http.end();
}
 




// void putTemp(){
//     HTTPClient http;  
//   http.begin((String)baseurl + "api/temp"); 
//   http.addHeader("api-key", api_key);  
//   http.addHeader("Content-Type", "application/json"); 
//   String RequestBody;  

 
//  String RequestBody;  

//   JsonDocument doc;  
//   doc ["temp"] = sensors.getTempCByIndex(0); 
//   doc.shrinkToFit(); 
//   serializeJson(doc, RequestBody); 
//   int httpResponseCode = http.PUT(); 
// }



void setup() {
  // put your setup code here, to run once: 
  pinMode(light_pin, OUTPUT);
  pinMode(fan_pin, OUTPUT);
  pinMode(PIR_pin,INPUT);
  pinMode(temp_sensor_pin,INPUT);

  
  Serial.begin(9600); 
  sensors.begin(); 
  sensors.setWaitForConversion(true); 
  delay(1000);
   

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  

}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
  getOutputDevices();
  } 
  else {
    Serial.println("WiFi Disconnected");
  }
}
