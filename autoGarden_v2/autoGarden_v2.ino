#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() 
{
  Serial.begin(115200);    
  setup_wifi();

}

void loop() 
{
    Serial.println("Hi");
    delay(1000);
}
