#include <HTTPClient.h>

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#define ADC_VREF_mV 3300.0
#define ADC_RESOLUTION 4096.0
#define lm35 32
int salon=4,ch1=0,ch2=15,ch3=14,cloir=12,dehors=33,buz=32,photo=26,ch4=13;
int lightInit;  // initial value
int lightVal;   // light reading
//192.168.93.1
const char* ssid = "NCB"; 
const char* password =  "ncbukasa1";
String  ClientRequest;
// 192.168.180.1
IPAddress staticIP604_10(192,168,180,10);
IPAddress gateway604_10(192,168,180,10);
IPAddress subnet604_10(255,255,255,0);

WiFiServer server(80);
WiFiClient client;

String myresultat;
String myData;
String ReadIncomingRequest(){
     while(client.available()) {
           ClientRequest = (client.readStringUntil('\r'));
           if ((ClientRequest.indexOf("HTTP/1.1")>0)&&(ClientRequest.indexOf("/favicon.ico")<0)){
           myresultat = ClientRequest;
     }
   }
  return myresultat;
 }
String lienvalide ="";


void setup() {
  // put your setup code here, to run once:
   ClientRequest = "";
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("...");
    }
    Serial.println("Connected");
    WiFi.config(staticIP604_10, gateway604_10, subnet604_10);
    Serial.println("Your IP is");
    Serial.println((WiFi.localIP()));
    server.begin();
    lightInit = analogRead(photo);
    pinMode(salon,OUTPUT);
    pinMode(ch1,OUTPUT);
    pinMode(ch2,OUTPUT);
    pinMode(ch3,OUTPUT);
    pinMode(ch4,OUTPUT);
    pinMode(cloir,OUTPUT);
    pinMode(dehors,OUTPUT);
    
}

void loop() {
  // put your main code here, to run repeatedly:
    int acv=analogRead(lm35);
    float millivol=acv*( ADC_VREF_mV / ADC_RESOLUTION);
    float tempC=millivol/10;
    float tempF=tempC*(9/5)+32;
    cel(tempC);
    fa(tempF);

    lightVal = analogRead(photo); // read the current light levels

     //si la valeur de la photoresistance est inferieur à 50, c'est la nuit.
     if(lightVal <  50)
     {
        digitalWrite (dehors, HIGH);
        envoi("allumedehors"); 
      // turn on light
     }
    else
    {
      digitalWrite (dehors, LOW);
      envoi("eteintDhors");
    }
    call();
    if(myData=="allumeSalon"){
       digitalWrite(salon,HIGH);
       envoi("allumeSalon");
    }
    if(myData=="eteindreTout"){
      envoi("eteindreTout");
      digitalWrite(salon,LOW);
      digitalWrite(ch1,LOW);
      digitalWrite(ch2,LOW);
      digitalWrite(ch3,LOW);
      digitalWrite(ch4,LOW);
      digitalWrite(dehors,LOW);
      digitalWrite(cloir,LOW);
    }
    if(myData=="allumeTout"){
      envoi("allumeTout");
      digitalWrite(salon,HIGH);
      digitalWrite(ch1,HIGH);
      digitalWrite(ch2,HIGH);
      digitalWrite(ch3,HIGH);
      digitalWrite(ch4,HIGH);
      digitalWrite(dehors,HIGH);
      digitalWrite(cloir,HIGH);
    }
    if(myData=="eteintSalon"){
      envoi("eteintSalon");
      digitalWrite(salon,LOW);
    }
    if(myData=="allumeCouloir"){
      envoi("allumeCouloir");
      digitalWrite(cloir,HIGH);
    }
    if(myData=="eteintCouloir"){
      envoi("eteintCouloir");
      digitalWrite(cloir,LOW);
    }
    if(myData=="eteintChambre1"){
      envoi("eteintChambre1");
      digitalWrite(ch1,LOW);
    }
    if(myData=="eteintChambre2"){
      envoi("eteintChambre2");
      digitalWrite(ch2,LOW);
    }
    if(myData=="eteintChambre3"){
      envoi("eteintChambre3");
      digitalWrite(ch3,LOW);
    }
    if(myData=="eteintChambre4"){
      envoi("eteintChambre4");
      digitalWrite(ch4,LOW);
    }
    if(myData=="eteintdehors"){
      envoi("eteintdehors");
      digitalWrite(dehors,LOW);
    }
    if(myData=="allumeChambre1"){
      envoi("allumeChambre1");
      digitalWrite(ch1,HIGH);
    }
    if(myData=="allumeChambre2"){
          envoi("allumeChambre2");
          digitalWrite(ch2,HIGH);
    }
    if(myData=="allumeChambre3"){
          envoi("allumeChambre3");
          digitalWrite(ch3,HIGH);
    }
    if(myData=="allumeChambre4"){
      envoi("allumeChambre4");
      digitalWrite(ch4,HIGH);
    }
    if(myData=="allumedehors"){
      envoi("allumedehors");
      digitalWrite(dehors,HIGH);
    }
    myData="";
    delay(10);
}
void call(){
   
    client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    ClientRequest = (ReadIncomingRequest());
    ClientRequest.remove(0, 5);
    ClientRequest.remove(ClientRequest.length()-9,9);
    Serial.println("Message reçu : ");
    Serial.println(ClientRequest);
    myData = ClientRequest;
    client.flush(); 
}

void envoi(String donne){
     lienvalide="http://192.168.180.1/text/text.php?page=";
     HTTPClient http;
     String lien = lienvalide+String(donne);
     http.begin(lien);
     int httpCode = http.GET();                                       
  if (httpCode > 0){ 
     String payload = http.getString();         
     //Serial.print("code serveur : ");
     //Serial.print(httpCode);
  }
}
void cel(float donne){
     lienvalide ="http://192.168.180.1/text/text2.php?val1=";
     HTTPClient http;
     String lien = lienvalide+float(donne);
     http.begin(lien);
     int httpCode = http.GET();                                       
  if (httpCode > 0){ 
     String payload = http.getString();         
     //Serial.print("code serveur : ");
     //Serial.print(httpCode);
  }
}//192.168.171.1
void fa(float donne){
     lienvalide =  "http://192.168.180.1/text/texte3.php?val2=";
     HTTPClient http;
     String lien = lienvalide+float(donne);
     http.begin(lien);
     int httpCode = http.GET();                                       
  if (httpCode > 0){ 
     String payload = http.getString();         
     //Serial.print("code serveur : ");
     //Serial.print(httpCode);
  }
}

