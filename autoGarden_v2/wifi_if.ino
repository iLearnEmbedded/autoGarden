#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

int WIFI_RST_SWITCH = 23;
WiFiManager wm;
void wifi_setup(void)
{
  Serial.begin(115200);
  pinMode (WIFI_RST_SWITCH, INPUT_PULLUP);
}
void wifi_task(void) 
{
  if(WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin();     
  }  
    if(digitalRead(WIFI_RST_SWITCH) == LOW)
    {
          
      wm.resetSettings();
      bool res = wm.autoConnect("Garden_Bot"); // password protected ap
  
      if(!res) {
          Serial.println("Failed to connect");
           ESP.restart();
      } 
      else {
          //if you get here you have connected to the WiFi    
          Serial.println("connected...yeey :)");
      }  
    }
}
