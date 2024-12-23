#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

int WIFI_RST_SWITCH = 23;
void setup() 
{
  WiFi.mode(WIFI_STA);
  pinMode (WIFI_RST_SWITCH, INPUT_PULLUP);
  Serial.begin(115200); 
  telegram_setup();
}

void loop() 
{
    wifi_setup_task();  
    telegram_task();
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected");  
    }    
    else
    {
      Serial.println("Please connect");
    }
    delay(1000);    
}
