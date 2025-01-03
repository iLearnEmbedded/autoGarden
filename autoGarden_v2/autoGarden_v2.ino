#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() 
{ 
  wifi_setup();
  telegram_setup();
  flowsensor_setup();
  time_setup();
  btn_setup();
}

void loop() 
{
    wifi_task();  
    telegram_task();
    flowsensor_task();    
    time_task();
    btn_task();      
}
