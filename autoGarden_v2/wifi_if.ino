#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

int WIFI_RST_SWITCH = 23;
void wifi_setup(void)
{
  Serial.begin(115200); 
  WiFi.mode(WIFI_STA);
  pinMode (WIFI_RST_SWITCH, INPUT_PULLUP);
}
void wifi_task(void) 
{    
    if(digitalRead(WIFI_RST_SWITCH) == LOW)
    {
      WiFiManager wm;    
      wm.resetSettings();
  
      bool res = wm.autoConnect("Garden_Bot"); // password protected ap
  
      if(!res) {
          Serial.println("Failed to connect");
          // ESP.restart();
      } 
      else {
          //if you get here you have connected to the WiFi    
          Serial.println("connected...yeey :)");
      }  
    }
}
