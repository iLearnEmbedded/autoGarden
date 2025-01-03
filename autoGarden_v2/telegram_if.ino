#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
#define CHAT_ID "XXXXXXXXXX"

String chat_id;



WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void telegram_setup(void)
{
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) 
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) 
  {
    // Chat id of the requester
    chat_id = String(bot.messages[i].chat_id);
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);    

    if (text == "start") 
    {
      bot.sendMessage(chat_id,"Turning Motor ON");
      bot.sendMessage(CHAT_ID,"Turning Motor ON");
      btn_activate_start();
    }

    else if (text == "start") 
    {
      bot.sendMessage(chat_id,"Turning Motor OFF");
      bot.sendMessage(CHAT_ID,"Turning Motor OFF");
      btn_activate_stop();
    }    
  } 
}

void telegram_task(void)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) 
    {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }    
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
