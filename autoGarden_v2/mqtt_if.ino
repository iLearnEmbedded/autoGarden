#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID "xxxx"
#define WLAN_PASS "xxxx@123"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"

// Using port 8883 for MQTTS
#define AIO_SERVERPORT  8883

// Adafruit IO Account Configuration
// (to obtain these values, visit https://io.adafruit.com and click on Active Key)
#define AIO_USERNAME  "xxxx"
#define AIO_KEY       "xxxx"


/************ Global State (you don't need to change this!) ******************/

// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// io.adafruit.com root CA
const char* adafruitio_root_ca = \
      "-----BEGIN CERTIFICATE-----\n"
      "MIIEjTCCA3WgAwIBAgIQDQd4KhM/xvmlcpbhMf/ReTANBgkqhkiG9w0BAQsFADBh\n"
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
      "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
      "MjAeFw0xNzExMDIxMjIzMzdaFw0yNzExMDIxMjIzMzdaMGAxCzAJBgNVBAYTAlVT\n"
      "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
      "b20xHzAdBgNVBAMTFkdlb1RydXN0IFRMUyBSU0EgQ0EgRzEwggEiMA0GCSqGSIb3\n"
      "DQEBAQUAA4IBDwAwggEKAoIBAQC+F+jsvikKy/65LWEx/TMkCDIuWegh1Ngwvm4Q\n"
      "yISgP7oU5d79eoySG3vOhC3w/3jEMuipoH1fBtp7m0tTpsYbAhch4XA7rfuD6whU\n"
      "gajeErLVxoiWMPkC/DnUvbgi74BJmdBiuGHQSd7LwsuXpTEGG9fYXcbTVN5SATYq\n"
      "DfbexbYxTMwVJWoVb6lrBEgM3gBBqiiAiy800xu1Nq07JdCIQkBsNpFtZbIZhsDS\n"
      "fzlGWP4wEmBQ3O67c+ZXkFr2DcrXBEtHam80Gp2SNhou2U5U7UesDL/xgLK6/0d7\n"
      "6TnEVMSUVJkZ8VeZr+IUIlvoLrtjLbqugb0T3OYXW+CQU0kBAgMBAAGjggFAMIIB\n"
      "PDAdBgNVHQ4EFgQUlE/UXYvkpOKmgP792PkA76O+AlcwHwYDVR0jBBgwFoAUTiJU\n"
      "IBiV5uNu5g/6+rkS7QYXjzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG\n"
      "AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMDQGCCsGAQUFBwEB\n"
      "BCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEIGA1Ud\n"
      "HwQ7MDkwN6A1oDOGMWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEds\n"
      "b2JhbFJvb3RHMi5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEW\n"
      "HGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQELBQADggEB\n"
      "AIIcBDqC6cWpyGUSXAjjAcYwsK4iiGF7KweG97i1RJz1kwZhRoo6orU1JtBYnjzB\n"
      "c4+/sXmnHJk3mlPyL1xuIAt9sMeC7+vreRIF5wFBC0MCN5sbHwhNN1JzKbifNeP5\n"
      "ozpZdQFmkCo+neBiKR6HqIA+LMTMCMMuv2khGGuPHmtDze4GmEGZtYLyF8EQpa5Y\n"
      "jPuV6k2Cr/N3XxFpT3hRpt/3usU/Zb9wfKPtWpoznZ4/44c1p9rzFcZYrWkj3A+7\n"
      "TNBJE0GmP2fhXhP1D/XVfIW/h0yCJGEiV9Glm/uGOa3DXHlmbAcxSyCRraG+ZBkA\n"
      "7h4SeM6Y8l/7MBRpPCz6l8Y=\n"
      "-----END CERTIFICATE-----\n";



/****************************** Feeds ***************************************/

// Setup a feed called 'test' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish current_a_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/current-a");
Adafruit_MQTT_Publish flow_state_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water-flow-indicator");
Adafruit_MQTT_Publish link_state_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/link-state");
Adafruit_MQTT_Subscribe motor_switch_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/motor-state");
Adafruit_MQTT_Publish motor_switch_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/motor-state");

/*************************** Sketch Code ************************************/
extern uint32_t prev_flow_state;
extern uint32_t adc_value;
void mqtt_setup(void)
{
  delay(10);

  Serial.println(F("Adafruit IO MQTTS (SSL/TLS) Example"));

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

  // Set Adafruit IO's root CA
  client.setCACert(adafruitio_root_ca);

  mqtt.subscribe(&motor_switch_sub);
}

uint32_t x=0;
uint32_t motor_state;
void mqtt_task(void)
{
    // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();  
  mqtt_update_status();
  mqtt_read_switch_state();  
  // wait a couple seconds to avoid rate limit
  delay(2000);
  
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

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

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}

void mqtt_sens_SafetyStop(void)
{
  int retval = motor_switch_pub.publish("OFF");
  Serial.println("MQTT mqtt_sens_SafetyStop sent!");
}

void mqtt_send_flowMsg(uint32_t state)
{ 
  if(flow_state_pub.publish(state))
  {
    Serial.println("MQTT mqtt_send_flowMsg success");
  }
  else
  {
    Serial.println("MQTT mqtt_send_flowMsg fail");
  } 
}

void mqtt_read_switch_state(void)
{
  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000)))
  {
    if(subscription == &motor_switch_sub)
    {
      if (strcmp((const char*)motor_switch_sub.lastread, "ON") == 0)
      {
        btn_activate_start();
      }
      else
      {
        btn_activate_stop();
      }
       Serial.println((char *)motor_switch_sub.lastread);
    }
  }
}

void mqtt_send_adcValue(uint32_t value)
{
  if(current_a_pub.publish(value))
  {
      Serial.println("MQTT mqtt_send_adcValue success");
  }
  else
  {
    Serial.println("MQTT mqtt_send_adcValue fail");
  }
}

void mqtt_send_motor_state(void)
{
  static int mqtt_init_motor_report = 0xFF;
  if(mqtt_init_motor_report)
  {
    if(motor_switch_pub.publish("OFF"))
    {
      mqtt_init_motor_report = 0;
    }
  }  
}

void mqtt_send_alive_signal(void)
{
  uint32_t link_state_val = 3;
  if(link_state_pub.publish(link_state_val))
  {
    Serial.println("MQTT Link State True");
  }
}
void mqtt_update_status(void)
{
  mqtt_send_alive_signal();
  mqtt_send_motor_state();
  mqtt_send_flowMsg(prev_flow_state);
  mqtt_send_adcValue(adc_value);
}
