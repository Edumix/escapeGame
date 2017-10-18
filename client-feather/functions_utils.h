/*---------------------------------------------------------------------------------------------

    functions_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    Ensemble de fonctions utilitaires

  --------------------------------------------------------------------------------------------- */
#include <ESP8266WebServer.h>

/*************************************************
   Returns a human readable IP from a string
 **************************************************/
String humanReadableIp(IPAddress ip) {
  return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
}

/*************************************************
   Returns a human readable Mac Address from
   an array of byte
 **************************************************/
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
