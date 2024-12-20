#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

int WIFI_RST_SWITCH = 23;
void setup() 
{
  WiFi.mode(WIFI_STA);
  pinMode (WIFI_RST_SWITCH, INPUT_PULLUP);
  Serial.begin(115200); 
}

void loop() 
{
    wifi_setup_task();  
    Serial.println("Hi");
    delay(1000);    
}
