#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <aJSON.h>
#include <stdio.h>
#include <PubSubClient.h>
#include <DHT.h>


#define DHTPIN 2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   // GPIO 2 pin of ESP8266
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE, 30);

#include <Wire.h>
//#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "images.h"
//SSD1306  display(0x3c, D3, D5);


const char* ssid     = "mobier";
const char* password = "mobier";
const char* mqtt_server = "192.168.11.1";


WiFiClient wificlient;
PubSubClient mqclient(wificlient);
long lastMsg = 0;
char * msg;
int value = 0;



String payload;
void setup() {

  Serial.begin(115200);
  dht.begin();
  delay(10);
  setupwifi();

  // MQTT---------
  mqclient.setServer(mqtt_server, 1883);
  mqclient.setCallback(callback);


}

void loop() {
  if (!mqclient.connected()) {
    reconnect();
  }
  mqclient.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    //  ++value;
    //  snprintf (msg, 75, "hello world #%ld", value);
    /*   int a = analogRead(A0);    //读取A0口的电压值
       float R = 1023.0 / ((float)a) - 1.0;
       R = 100000.0 * R;
       float temperature = 1.0 / (log(R / 100000.0) / 4275 + 1 / 298.15) - 273.15; //convert to temperature via datasheet ;
    */
    //msg =(char*)int(temperature);
    //    String data=temperature.toString();
    //String data="27";
    //   Serial.println(data.c_str());
    /* char *data;
      dtostrf(temperature,4,2,data);
      data[5]='\0';*/
    //   strcat(msg,temperature);
    /*
      //itoa(int(temperature),msg,10);
      Serial.print("Publish message: ");
      Serial.println(String(temperature).c_str());
      //  Serial.println(data);

      mqclient.publish("temperature", String(temperature).c_str());*/
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    // Compute heat index
    // Must send in temp in Celsius!
    float hi = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.println(t);

   // Serial.print("Publish message: ");
    //Serial.println(String(temperature).c_str());
    //  Serial.println(data);

    mqclient.publish("temperature", String(t).c_str()); 
    mqclient.publish("Humidity", String(h).c_str()); 

  }


}

void HumiditySwitch(byte* payload, unsigned int length){
  
}
void LightSwitch(byte* payload, unsigned int length){
  
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  if(topic=="Humidity"){
    HumiditySwitch(payload,length);
  }else if (topic=="Light"){
    LightSwitch(payload,length);
  }
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setupwifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



}
void reconnect() {
  // Loop until we're reconnected
  while (!mqclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqclient.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqclient.publish("temperature", "hello world");
      // ... and resubscribe
      mqclient.subscribe("inTopic1");
      mqclient.subscribe("inTopic2");
      mqclient.subscribe("inTopic3");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void getweather(){
HTTPClient http;
http.begin("api.heweather.com",80,"/x3/weather?cityid=CN101010300&key=XXXX");
int httpCode = http.GET();
  if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                 payload = http.getString();
            }
  }else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
   }
   http.end();
   char * data=(char*)payload.c_str();
   Serial.println(data);
   aJsonObject * msg =  aJson.parse(data);
   //unsigned char * udata ="HeWeather data service 3.0";
    aJsonObject * amsg = aJson.getObjectItem(msg, "HeWeather data service 3.0");
   aJsonObject * weatherinfo = aJson.getArrayItem(amsg,0);
   aJsonObject * aqi = aJson.getObjectItem(weatherinfo, "aqi"); 
   aJsonObject * cityi = aJson.getObjectItem(aqi, "city"); 
   aJsonObject * pm25 = aJson.getObjectItem(cityi, "pm25"); 
   Serial.println(pm25->valuestring);
   aJsonObject * daily_forecast = aJson.getObjectItem(weatherinfo, "daily_forecast"); 
   aJsonObject * forecast = aJson.getArrayItem(daily_forecast,0);
   aJsonObject * tmp = aJson.getObjectItem(forecast, "tmp"); 
   aJsonObject * dmax = aJson.getObjectItem(tmp, "max"); 
   aJsonObject * dmin = aJson.getObjectItem(tmp, "min"); 
   Serial.print("max:");
   Serial.print(dmax->valuestring);
   Serial.print("min:");
   Serial.println(dmin->valuestring);
   


    int a = analogRead(A0);    //读取A0口的电压值
    float R = 1023.0 / ((float)a) - 1.0;
    R = 100000.0 * R;
    float temperature = 1.0 / (log(R / 100000.0) / 4275 + 1 / 298.15) - 273.15; //convert to temperature via datasheet ;
   display.setFont(ArialMT_Plain_16);
   display.drawString(85, 16, dmax->valuestring);
   display.drawString(103, 16, "~");
   display.drawString(110, 16, dmin->valuestring);
   display.drawString(85, 32, String(temperature).c_str());
   display.drawString(73, 0, "PM:");
   display.drawString(100, 0, pm25->valuestring);

}
