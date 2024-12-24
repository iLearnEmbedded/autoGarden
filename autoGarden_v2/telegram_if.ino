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
