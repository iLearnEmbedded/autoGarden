#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() 
{  
  Serial.begin(115200); 
  wifi_setup();
  telegram_setup();
  flowsensor_setup();
  time_setup();
}

void loop() 
{
    wifi_task();  
    telegram_task();
    flowsensor_task();    
    time_task();      
}
