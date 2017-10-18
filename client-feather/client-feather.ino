/*---------------------------------------------------------------------------------------------

    Client Feather pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    
  --------------------------------------------------------------------------------------------- */
// Bunch of Ethernet, Wifi, UDP, WebServer and OSC
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// GPIO For feather Huzzah 4, 5, 2, 16, 0, 15
#define ERROR_LED 0 // This the build-in led

// --------------------------------------------------------------------------------------
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 2390;        // local port to listen for UDP packets for OSC messages
const unsigned int wwwPort = 80;            // www server port

char ssid[] = "escapegame";          // your network SSID (name)
char pass[] = "echappeebelle";          // your network password

// Web server
ESP8266WebServer server(wwwPort);

// --------------------------------------------------------------------------------------
//     Handle to /close, /open, /pause and / (root) for infos
// --------------------------------------------------------------------------------------
void handleRoot() {
  Serial.println("Requested '/'");
  String content = "{ value : 0.0, ";
  content += "message : '";
  content += printFeatherInfo();
  content +=  "' }";
  server.send(200, "text/json", content);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// --------------------------------------------------------------------------------------
//   Reading OSC Bundles, and treat them with an callback
// --------------------------------------------------------------------------------------
void readOSCBundle() {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {

    Serial.print(millis());
    Serial.print(" : ");
    Serial.println("OSC Packet as Bundle Received");

    while (size--) {
      // Read and feed the object --
      bundle.fill(Udp.read());
    }

    if (!bundle.hasError()) {
      // Dispatch from Addresses received to callback functions
      //bundle.dispatch("/position", positionChange);
      //bundle.dispatch("/state", adjustChange);

    } else {
      // Errors, print them
      OSCErrorCode error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
      // not connected => Message + Blink Lon
      ledBlink(ERROR_LED, 200);

    }
  }
}


// --------------------------------------------------------------------------------------
//   Reading OSC Bundles on the network
// --------------------------------------------------------------------------------------
void sendOSCBundle(IPAddress ip, int port, String path, float value) {
  OSCBundle bundle;

  Serial.print("Sending OSC Bundle to " + humanReadableIp(ip));
  Serial.print(" : " + path + "/");
  Serial.println(value);

  bundle.add("/position").add(value);
  Udp.beginPacket(ip, port);
  bundle.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  bundle.empty(); // empty the bundle to free room for a new one
}


// --------------------------------------------------------------------------------------
//   Displaying Feather Technical infos
// --------------------------------------------------------------------------------------
String printFeatherInfo() {
  String str = "\n----------------------------------------";
  // Lire les Infos dans l'eEPROM
//  str += "\n----------------------------------------\n";
  return str;
}

// --------------------------------------------------------------------------------------
//   Retrieving Feather infos from the server
// --------------------------------------------------------------------------------------
void getFeatherInfo() {
  // Ecrire les Infos dans l'EEPROM
}

// --------------------------------------------------------------------------------------
//   Displaying Feather Technical infos
// --------------------------------------------------------------------------------------
void setFeatherInfo() {
  // Ecrire les Infos dans l'EEPROM
}


// --------------------------------------------------------------------------------------
//   Reading Serial Command
// --------------------------------------------------------------------------------------
void readSerialCommand() {
  if (Serial.available() ) {
    int commande = Serial.read();
    if (char(commande) == '0') {
    }
    if (char(commande) == '1') {
    }
    if  (char(commande) == '2') {
    }
  }
}

// ----------------------------------------------------------
// Do blink a led without delay
// ----------------------------------------------------------
void ledBlink(unsigned int _led, unsigned int _delayMs) {
  float alternativeSignal = (millis() % _delayMs) / (float)_delayMs;
  if (alternativeSignal > 0.5) {
    digitalWrite(_led, HIGH);
  } else {
    digitalWrite(_led, LOW);
  }
}

/*************************************************
   Returns a human readable IP from a string
 **************************************************/
String humanReadableIp(IPAddress ip) {
  return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
}

String humanReadableMacAddr() {
  byte _mac[6];
  WiFi.macAddress(_mac);
  String strMac;
  strMac += String(_mac[0], HEX);
  strMac += ":";
  strMac += String(_mac[1], HEX);
  strMac += ":";
  strMac += String(_mac[2], HEX);
  strMac += ":";
  strMac += String(_mac[3], HEX);
  strMac += ":";
  strMac += String(_mac[4], HEX);
  strMac += ":";
  strMac += String(_mac[5], HEX);
  strMac.toUpperCase();
  return strMac;
}
// --------------------------------------------------------------------------------------
// Setup
// --------------------------------------------------------------------------------------
void setup()
{

  Serial.begin(115200);
  delay(100);
  Serial.println("");

  // Wifi connection
  //IPAddress gateway(192, 168, 10, 1);
  //IPAddress subnet(255, 255, 255, 0);

  // Connect to WiFi network
  Serial.print("Connecting to SSID [");
  Serial.print(ssid);
  Serial.print("] pass [");
  Serial.print(pass);
  Serial.println("]");

 // WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  int k = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if (k % 80 == 0) {
      Serial.println("");
    }
    k++;
    ledBlink(ERROR_LED, 100);
    delay(100);
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.print(" -> Mac address : ");
  Serial.println(humanReadableMacAddr());
  Serial.print("-> IP address : ");
  Serial.println(WiFi.localIP());

  // COnfiguring UDP for OSC dialog
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port : ");
  Serial.println(Udp.localPort());
  Serial.println("");

  // Web server preparation
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.print("HTTP server started at http://");
  Serial.print(humanReadableIp(WiFi.localIP()));
  Serial.print(":");
  Serial.print(wwwPort);
  Serial.print("/");
  Serial.println("");

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
    ledBlink(ERROR_LED, 100);
  } else {
    //
    // Nominal running : Status led gives a 1 sec pulse.
    //
    ledBlink(ERROR_LED, 1000);

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





