#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
#define CHAT_ID "XXXXXXXXXX"



WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void telegram_setup(void)
{
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}

void telegram_task(void)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    bot.sendMessage(CHAT_ID, "Hi Pawan");  
  }  
}

void telegram_send_flowMsg(int state)
{
  if(WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  
  if (state == 0)
  {
    bot.sendMessage(CHAT_ID, "Water Stagnant");  
  }

  else
  {
    bot.sendMessage(CHAT_ID, "Water Flowing");  
  }
}
