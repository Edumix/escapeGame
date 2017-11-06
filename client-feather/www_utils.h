/*---------------------------------------------------------------------------------------------

    www_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    Ensemble de fonctions de gestion d'un serveur web embarqué

  --------------------------------------------------------------------------------------------- */
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Web server
const unsigned int wwwPort = 80;            // www server port
ESP8266WebServer server(wwwPort);

// --------------------------------------------------------------------------------------
//     return the server host
// --------------------------------------------------------------------------------------
String getHost(String server, String port) {
  return "http://" + server + ":" + port;
}

// --------------------------------------------------------------------------------------
//     Handle to 404 Error
// --------------------------------------------------------------------------------------
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
//     Handle to root for infos
// --------------------------------------------------------------------------------------
void handleRoot() {
  Serial.println("Requested '/'");
  String content = "{ value : 0.0, ";
  content += "message : '";
  content += printFeatherInfo();
  content +=  "' }";
  server.send(200, "text/json", content);
}

// --------------------------------------------------------------------------------------
//     Handle to /play for infos
// --------------------------------------------------------------------------------------
void playSound() {
  Serial.println("Requested '/playSound'");
  String content = "{ value : 0.0, ";
  content += "message : 'Playing...' }";
  server.send(200, "text/json", content);
  playSoundCallback();
}

// --------------------------------------------------------------------------------------
//     Handle to /pause for infos
// --------------------------------------------------------------------------------------
void pauseSound() {
  Serial.println("Requested '/pauseSound'");
  String content = "{ value : 0.0, ";
  content += "message : 'Paused...' }";
  server.send(200, "text/json", content);
  pauseSoundCallback();
}

// --------------------------------------------------------------------------------------
//     Handle to /stop for infos
// --------------------------------------------------------------------------------------
void stopSound() {
  Serial.println("Requested '/stopSound'");
  String content = "{ value : 0.0, ";
  content += "message : 'Stopped !' }";
  server.send(200, "text/json", content);
}

// --------------------------------------------------------------------------------------
//     Handle to /stop for infos
// --------------------------------------------------------------------------------------
void sendJSONMsg(String msg) {
  Serial.print("Sendding message to server : %s");
  Serial.println(msg);
  String content = "{ value : 0.0, ";
  content += "message : '" + msg +  "' }";
  server.send(200, "text/json", content);
}

// --------------------------------------------------------------------------------------
//     Setting up WWW server
// --------------------------------------------------------------------------------------
void setup_www() {
  // Web server preparation
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/play", playSound);
  server.on("/pause", pauseSound);
  server.on("/stop", stopSound);
  
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.print("HTTP server started at http://");
  Serial.print(humanReadableIp(WiFi.localIP()));
  Serial.print(":");
  Serial.print(wwwPort);
  Serial.print("/");
  Serial.println("");
}

