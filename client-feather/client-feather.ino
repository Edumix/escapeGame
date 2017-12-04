/*---------------------------------------------------------------------------------------------

    Client Feather pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    
  --------------------------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------------------
    Configuration globale du feather
    Décommenter les feathersWings pris en charge

    @TODO : Mettre tout ça dans un fichier de conf config.h et commiter un config.h.sample
------------------------------------------------------------------------------------------------*/
//#define OSC_DRIVING  // Some code to receive and send OSC blundles
//#define MP3_READER
#define SENSOR_MODE
//#define ACTUATOR_MODE
//#define MIDI_MODE     // Some Midi interface to get notes and doing an actions with this sequence
//-----------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include "serial_utils.h"
#include "featherConf_utils.h"
#include "functions_utils.h"

#ifdef MP3_READER
#include "sound_utils.h"
#endif

#include "wlan_utils.h"
#include "www_utils.h"

#ifdef OSC_DRIVING
#include "osc_utils.h"
#endif

#ifdef SENSOR_MODE
#include "sensors_mode_utils.h"
#endif

#ifdef MIDI_MODE
#include "midi_utils.h"
#endif

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

  // Setting up the sound feather wings
#ifdef MP3_READER
  setup_sound();
#endif

  // Connect to WiFi network
  setup_wlan(ssid, pass);

  // Configuring UDP for OSC dialog
#ifdef OSC_DRIVING
  setup_osc();
#endif

  // Mounting web server
  setup_www();

  // Setting up buttons framwork
#ifdef SENSOR_MODE
  setup_sensor();
#endif

  // Print info
  //Serial.print(printFeatherInfo());
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
#ifdef OSC_DRIVING
    readOSCBundle();
#endif

    // -------------------------------------------------------
    // Reading command on serial
    // -------------------------------------------------------
    readSerialCommand();

#ifdef MIDI_MODE
    // -------------------------------------------------------
    // Reaing midi messages
    // -------------------------------------------------------
    readMidiMessages();
#endif

    // -------------------------------------------------------
    // Handle Web server
    // -------------------------------------------------------
    server.handleClient();

    // -------------------------------------------------------
    // Reading sensors
    // -------------------------------------------------------
    // @TODO : Comment rendre cette logique générique ?
#ifdef SENSOR_MODE
    if (digitalRead(PIN2) == LOW) {
      wwwSend(IPAddress(192, 168, 10, 101), "/play");
    } else {
      wwwSend(IPAddress(192, 168, 10, 101), "/stop");
    }
#endif

    // -------------------------------------------------------
    // Playing with actionners
    // -------------------------------------------------------
    
    // -------------------------------------------------------
    // FeatherWing MP3 Player
    // -------------------------------------------------------
#ifdef MP3_READER
    soundDriver();
#endif
  } // end else Wifi.status
  ledBlink(BUILDIN_LED, 1000);
}





