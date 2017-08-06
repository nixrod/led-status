#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
const char* ssid = "<your wifi ssid>";
const char* password = "<your wifi password>";
// change this value to the ip of the server running the server code
const char* ENDPOINT = "http://192.168.1.20:3000/status";

const int LED_PIN =  16;  // node mcu d0
const int BUTTON_PIN = 5; // node mcu d1

// initialized as -1 so it needs to be updated on the first pass
int virtualLedState = -1;
 
void setup () {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  Serial.begin(115200);
  delay(10);
  
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
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    updateLedState();
    updateVirtualLedState();
  }
 
  delay(3000);    //Send a request every 3 seconds
}

void updateLedState() {
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin(ENDPOINT);  //Specify request destination
    int httpCode = http.GET(); //Send the request
 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);             //Print the response payload

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      boolean led = root["led"];

      if(led) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
 
    http.end();   //Close connection
}

void updateVirtualLedState() {
  int buttonState = digitalRead(BUTTON_PIN);
  Serial.print("ButtonState: ");
  Serial.println(buttonState);

  // we only update the virtual led when the button was switched
  if (buttonState != virtualLedState) {
    Serial.println("update required");
    Serial.println("updating...");

    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    // sets it to true if buttonState is 1, false otherwise
    root["virtualLed"] = (buttonState == 1);
    root.printTo(Serial);

    char jsonMessageBuffer[200];
    root.printTo(jsonMessageBuffer, sizeof(jsonMessageBuffer));
    
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin(ENDPOINT);      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(jsonMessageBuffer);   //Send the request
    String payload = http.getString(); //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
    
    // update the virtualLed state if the update was successful
    if (httpCode == 200) {
      Serial.println("Update Success, updating virtualLedState");
      virtualLedState = buttonState;  
    }
    return;
  }

  Serial.println("No update required");
}

