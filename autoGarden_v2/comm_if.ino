#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID "xxxx"
#define WLAN_PASS "xxxx@123"

/************************* Telegram Setup *********************************/

#define BOTtoken "xxxx"  // your Bot Token (Get from Botfather)
#define CHAT_ID "xxxx" //google for @myidbot


/* Variables */
String chat_id;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
uint32_t x=0;
uint32_t motor_state;
extern int time_available;
int device_restart_flag;
extern int adc_cal_factor;


/*************************** Sketch Code ************************************/
extern uint32_t prev_flow_state;
extern uint32_t adc_value;
void comm_setup(void)
{
  delay(10);

  Serial.println(F("Telegram Setup connecting ... "));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  delay(1000);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org  
}


void comm_task(void)
{
  comm_connect();  
  comm_check_new_messages();
}


// Function to connect and reconnect as necessary to the telegram server.
// Should be called in the loop function and it will take care if connecting.
void comm_connect() {
  
  // Check if the device is already connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to Wi-Fi...");

    // Start Wi-Fi connection
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("\nWi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());    
  }  
}

void comm_check_new_messages(void)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) 
    {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }  

    if(!device_restart_flag)
    {
      device_restart_flag = 1;
      comm_send_message("Device Restarted..");
      comm_update_status();
    }
  }
}

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
    text.toLowerCase();
    text.trim();    
    Serial.println(text);    
    
    comm_send_message(text);

    if (text == "start") 
    {
      if(time_available)
      {
        btn_activate_start();
        delay(5000);
        comm_update_status();
      }
      else
      {
        comm_send_message("Time not available. Please resend the command");        
      }      
    }

    else if (text == "stop") 
    {
      btn_activate_stop();
      delay(5000);
      comm_update_status();
    } 

    else if (text == "status") 
    {      
      comm_update_status();
    }

    int pos1 = text.indexOf("routine ");
    if (pos1 != -1) {
       String numberString = text.substring(pos1 + 8); // "routine " has 8 characters
       int hh = numberString.toInt();
       routine_write(hh);
       comm_send_message("Routine update successful");
       comm_update_status();
    }

    int pos2 = text.indexOf("thresh ");
    if (pos2 != -1) {
       String numberString = text.substring(pos2 + 7); // "thresh " has 7 characters
       int thresh = numberString.toInt();
       thresh_write(thresh);
       comm_send_message("Thresh update successful");
       comm_update_status();
    }    

    int pos3 = text.indexOf("cal ");
    if(pos3 != -1)
    {
      String numberString = text.substring(pos3 + 4); //"cal " has 4 chars
      adc_cal_factor = numberString.toInt();
      adc_cal_write(adc_cal_factor);
      comm_send_message("cal update successful");
      comm_update_status();
    }
  } 
}

void comm_send_SafetyStop(void)
{
 comm_send_message("Safety Stop");
 Serial.println("MQTT comm_sens_SafetyStop sent!");
 delay(5000);
 comm_update_status();
}

String construct_flowMsg(uint32_t state)
{
  String flow_state;
  if(state)
  {
    flow_state = "Water Flowing";
  }
  else
  {
    flow_state = "Water not flowing";
  }  
  return flow_state;
}

void comm_send_flowMsg(uint32_t state)
{   
  comm_send_message(construct_flowMsg(state));
}

String construct_adc_data(uint32_t value)
{
  float current = (float)(value * adc_cal_factor)/1000000;
  String adc_data_s = "Motor Current = " + String(current) + "A " +"ADC Value = " + String(value);
  return adc_data_s;
}
void comm_send_adcValue(uint32_t value)
{
  comm_send_message(construct_adc_data(value));
}

void comm_update_status(void)
{
 String status_update =  "Flow: "+construct_flowMsg(prev_flow_state) + " | " + construct_adc_data(adc_value) + " | " + " ADC cal = " + String(cal_read()) + " | " + " Threshold mins = " + String(thresh_read()); 
 comm_send_message(status_update);
}

void comm_send_message(String text)
{
  bot.sendMessage(chat_id,text);
  if(chat_id != String(CHAT_ID))
  {
    bot.sendMessage(CHAT_ID,text);    
  }  
}
