#include <WiFi.h>
#include "time.h"

const char* ntpServer1 = "pool.ntp.org";
const char* time_zone = "IST-5:30";

void time_setup()
{
  configTzTime(time_zone, ntpServer1);
}

void time_task()
{
  
  printLocalTime();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  char printBuffer[100];
  sprintf(printBuffer, "Year %d, Month %d, Day %d, Hour %d, Min %d, Sec %d", 
                        timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, 
                        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial.println(printBuffer);
  strftime(printBuffer, 100, "%A, %B %d %Y %H:%M:%S", &timeinfo);
  Serial.println(printBuffer);
}
