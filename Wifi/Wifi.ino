#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "NOME DA REDE";
const char* password =  "SENHA DA REDE";
 
void setup() {
 
  Serial.begin(9600);
  delay(4000);   //Delay needed before calling the WiFi.begin
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
   String var = "{\"name\":\"Marco\",\"email\":\"marco@mail.com\"}";
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin("http://site.com/post");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/json");             //Specify content-type header
 
   int httpResponseCode = http.POST(var);   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(10000);  //Send a request every 10 seconds
 
}
