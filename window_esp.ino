#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "FirebaseESP8266.h"
SoftwareSerial NodeMCU(D7, D8);

int t = 1;
String character;
String content;
String stat; 
String last;
// 設定網路基地台SSID跟密碼
const char* ssid      = "2019 TCA_CHT";
const char* password  = "20192019";
#define FIREBASE_HOST "https://smart-house-622bd.firebaseio.com/"
#define FIREBASE_AUTH "GOaVTdNwC2Pgos7nbYQ30niW68mbnl5QBv5RsHOX"
FirebaseData firebaseData;
void printJsonObjectContent(FirebaseData &data);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  NodeMCU.begin(9600);
  delay(10);

  pinMode(D7, INPUT);

  pinMode(D8, OUTPUT);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Firebase.enableClassicRequest(firebaseData, true);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  String path = "/house";
  FirebaseJson json;
  
  
  
  if (Firebase.getString(firebaseData, path + "/window"))
  {
    Serial.println(firebaseData.stringData());

    content = firebaseData.stringData();
    
    

    Serial.println("------------------------------------");
    Serial.println();
    if(content!= last){
    if(content=="1")
    {
      NodeMCU.print("1");
    }
    else if(content=="0")
    {
      NodeMCU.print("0");
    }
    }
  }
  
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  last==content;
}
void printJsonObjectContent(FirebaseData &data) {
  size_t tokenCount = data.jsonObject().parse(false).getJsonObjectIteratorCount();
  String key;
  String value;
  FirebaseJsonObject jsonParseResult;
  Serial.println();
  for (size_t i = 0; i < tokenCount; i++)
  {
    data.jsonObject().jsonObjectiterator(i, key, value);
    jsonParseResult = data.jsonObject().parseResult();
    Serial.print("KEY: ");
    Serial.print(key);
    Serial.print(", ");
    Serial.print("VALUE: ");
    Serial.print(value);
    Serial.print(", ");
    Serial.print("TYPE: ");
    Serial.println(jsonParseResult.type);

  }
}
