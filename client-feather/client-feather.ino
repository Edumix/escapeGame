/*---------------------------------------------------------------------------------------------

    Client Feather pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    
  --------------------------------------------------------------------------------------------- */
#include <ESP8266WiFi.h>
#include "serial_utils.h"
#include "featherConf_utils.h"
#include "functions_utils.h"
#include "wlan_utils.h"
#include "www_utils.h"
#include "osc_utils.h"

// GPIO For feather Huzzah 4, 5, 2, 16, 0, 15
#define BUILDIN_LED 0 // This the build-in led

// --------------------------------------------------------------------------------------
// Wifi parameters
char ssid[] = "escapegame";          // your network SSID (name)
char pass[] = "echappeebelle";          // your network password

// An Http Client to get info from server
WiFiClient client;
const int serverIP = IPAddress(192, 168, 10, 200);
const int serverPort = 5000;

// --------------------------------------------------------------------------------------
// Setup
// --------------------------------------------------------------------------------------
void setup()
{

  setup_serial();

  // Connect to WiFi network
  setup_wlan(ssid, pass);

  // Configuring UDP for OSC dialog
  setup_osc();

  // Mounting web server
  setup_www();
  
  Serial.print(printFeatherInfo());
}

// --------------------------------------------------------------------------------------
// Loop
// --------------------------------------------------------------------------------------
void loop()
{
  // -------------------------------------------------------
  // Verifying wifi connection
  // -------------------------------------------------------
  if (WiFi.status() != WL_CONNECTED) {
    // not connected => Message + Blink Short
    Serial.println("Wifi Not Connected :(");
    ledBlink(BUILDIN_LED, 100);
  } else {
    // -------------------------------------------------------
    // Reading OSC Bundles : /position & /adjust
    // -------------------------------------------------------
    readOSCBundle();

    // -------------------------------------------------------
    // Reading command on serial
    // -------------------------------------------------------
    readSerialCommand();

    // -------------------------------------------------------
    // Handle Web server
    // -------------------------------------------------------
    server.handleClient();

    // -------------------------------------------------------
    // Reading sensors
    // -------------------------------------------------------

    // -------------------------------------------------------
    // Playing with actionners
    // -------------------------------------------------------

  } // end else Wifi.status
}





