#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup_wifi(void) 
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
