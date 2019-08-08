
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>


#define INTPIN 2
#define LED 3
#define DHTPIN 23

char auth[]="WGZybl9hDDp2BqrIhV4Qyzl_XDm6JtvS";

DHT dht(DHTPIN,DHT22);
BlynkTimer timer;
WidgetLED led1(V1);

char ssid[] = "snapoffice_01";
char pass[] = "SnapAguascalientes";

bool ext_intFlag = 0;
bool pinChanged = 0;


BLYNK_WRITE(V0){
  int V0Data = param.asInt();
  //.assString
  //.assDouble
  Serial.print("Valor de V0 = ");
  Serial.println (V0Data);
   digitalWrite(LED,V0Data);
}
void sendSensor(){
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Blynk.virtualWrite(V5,h);
  Blynk.virtualWrite(V6,t);
}

void checkPin(){
  ext_intFlag = !ext_intFlag;
  pinChanged = 1;
}

void setup(){
  Serial.begin(115200);
  pinMode(INTPIN,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INTPIN),checkPin,RISING);
  digitalWrite(LED,LOW);
  dht.begin();
  timer.setInterval(2000L, sendSensor);
  Blynk.begin(auth,ssid,pass);
}

void loop(){
  Blynk.run();
  timer.run();
  if(pinChanged);{
    if(ext_intFlag){
      led1.on();
    }
    else{
      led1.off();
    }
    pinChanged=0;
  }
}
