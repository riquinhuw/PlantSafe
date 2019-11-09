#include <Bridge.h>
#include <HttpClient.h>

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);
  while (!Serial); 
}

void loop() {

  String var = "";
  String value1 = "value1";
  String value2 = "2";
  String value3 = "3";

  var = "{\"name\":\"AGORA\",\"email\":\"VAI@mado.doceu\"}";
  Serial.println(var);

  HttpClient client;
  client.setHeader("Content-Type: application/json");
  String httpBody = var;
  String httpDestination = "http://plantsafe.herokuapp.com/users/"; //URL Sever 
  client.post(httpDestination, httpBody);

  if (client.read() == 123) {
    Serial.println("OK");
  } else {
    Serial.println("Error");
  }
  
  Serial.flush();
  delay(10000);
}
