#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
#define CHAT_ID "XXXXXXXXXX"

String chat_id;
int ANALOG_PIN = 34;
extern int prev_flow_state;
extern int time_available;
#define CAL_FACTOR 0.0012 //Derived out of experiments with current meter

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
      if(time_available)
      {
        bot.sendMessage(chat_id,"Turning Motor ON");
        bot.sendMessage(CHAT_ID,"Turning Motor ON");
        btn_activate_start();
      }
      else
      {
        bot.sendMessage(chat_id,"Time not available. Please resend the command");
        bot.sendMessage(CHAT_ID,"Time not available. Please resend the command");      
      }
      
    }

    else if (text == "stop") 
    {
      bot.sendMessage(chat_id,"Turning Motor OFF");
      bot.sendMessage(CHAT_ID,"Turning Motor OFF");
      btn_activate_stop();
    } 

     else if (text == "status") 
    {
      bot.sendMessage(chat_id,"Status");
      bot.sendMessage(CHAT_ID,"Status");
      telegram_send_statusMsg();
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
    bot.sendMessage(chat_id, "Water Stagnant");  
  }

  else
  {
    bot.sendMessage(CHAT_ID, "Water Flowing");  
    bot.sendMessage(chat_id, "Water Flowing");  
  }
}

void telegram_send_adcMsg(int adc_value)
{
  float current_A = (float) adc_value * CAL_FACTOR;
  String adcMsg = "Motor Current (A) = " + String(current_A);
  bot.sendMessage(CHAT_ID, adcMsg);  
  bot.sendMessage(chat_id, adcMsg);  
}

void telegram_send_statusMsg(void)
{
  int adc_value = analogRead(ANALOG_PIN);
  telegram_send_adcMsg(adc_value);
  telegram_send_flowMsg(prev_flow_state);
}

void telegram_send_SafetyStop(void)
{
    bot.sendMessage(chat_id,"Safety Turning Motor OFF");
    bot.sendMessage(CHAT_ID,"Safety Turning Motor OFF");
}
