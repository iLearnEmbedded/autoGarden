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
    if(flowsensor_task() & 0x80)
    {
      telegram_send_flowMsg(flowsensor_get_state());
    }
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected");  
    }    
    else
    {
      Serial.println("Please connect");
    }

    time_task();
    delay(1000);    
}
