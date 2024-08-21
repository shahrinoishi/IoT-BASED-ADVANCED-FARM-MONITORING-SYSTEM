/* Connections 
Relay. D3 
Btn.   D7 
Soil.  A0 
PIR.   D5 
SDA.   D2 
SCL.   D1 
Temp.  D4 
*/ 
 
//Include the library files 
#include <LiquidCrystal_I2C.h> 
#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
#include <DHT.h> 
 
 
 
char auth[] = "tGauZtpociWSI8T-zhnjD2ue2XfcFAGU";  //Enter your Blynk Auth token 
char ssid[] = "Shahrin";  //Enter your WIFI SSID 
char pass[] = "12345678";  //Enter your WIFI Password 
 
DHT dht(D4, DHT11);//(DHT sensor pin,sensor type)  D4 DHT11 Temperature Sensor 
BlynkTimer timer; 
 
//Define component pins 
#define soil A0     //A0 Soil Moisture Sensor 
#define PIR D5      //D5 PIR Motion Sensor 
int PIR_ToggleValue; 
 
void checkPhysicalButton(); 
 
#define relay1 D0 
#define RELAY_PIN_1       D3   //D3 Relay 
 
 
BLYNK_WRITE(V2) { 
  bool value1 = param.asInt(); 
  // Check these values and turn the relay1 ON and OFF        // ttttttttttttttttttttttttttttttttttttttttttttttttt 
  if (value1 == 1) { 
    digitalWrite(relay1, LOW); 
  } else { 
    digitalWrite(relay1, HIGH); 
  } 
} 
 
//Create three variables for pressure 
double T, P; 
char status; 
 
 
 
void setup() { 
  Serial.begin(9600); 
  
  pinMode(PIR, INPUT); 
 
  //Set the relay pins as output pins 
  pinMode(relay1, OUTPUT); 
 
  // Turn OFF the relay 
  digitalWrite(relay1, HIGH); 
 
  //Initialize the Blynk library 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); 
 
 
 pinMode(RELAY_PIN_1, OUTPUT); 
 digitalWrite(RELAY_PIN_1, LOW); 
  
 
 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); 
  dht.begin(); 
 
   
} 
 
 
//Get the DHT11 sensor values 
void DHT11sensor() { 
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 
 
  if (isnan(h) || isnan(t)) { 
    Serial.println("Failed to read from DHT sensor!"); 
    return; 
  } 
  Blynk.virtualWrite(V0, t); 
  Blynk.virtualWrite(V1, h); 
 
  
} 
 
 
//Get the soil moisture values 
void soilMoistureSensor() { 
  int value = analogRead(soil); 
  value = map(value, 0, 1024, 0, 100); 
  value = (value - 100) * -1; 
 
  Blynk.virtualWrite(V3, value); 
   
 
} 
 
//Get the PIR sensor values 
void PIRsensor() { 
  bool value = digitalRead(PIR); 
  if (value) { 
    Blynk.logEvent("pirmotion","WARNNG! Motion Detected!"); //Enter your Event Name 
    WidgetLED LED(V5); 
    LED.on(); 
  } else { 
    WidgetLED LED(V5); 
    LED.off(); 
  }   
  } 
 
BLYNK_WRITE(V6) 
{ 
 PIR_ToggleValue = param.asInt();   
} 
 
} 
void loop() { 
    if (PIR_ToggleValue == 1) 
    { 
    lcd.setCursor(5, 1); 
    lcd.print("M:ON "); 
      PIRsensor(); 
      } 
     else 
     { 
    lcd.setCursor(5, 1); 
    lcd.print("M:OFF"); 
    WidgetLED LED(V5); 
    LED.off(); 
     } 
  Blynk.run();//Run the Blynk library 
  timer.run();//Run the Blynk timer 
 
  }
