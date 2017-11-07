/*---------------------------------------------------------------------------------------------

    www_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    Ensemble de fonctions de gestion d'un serveur web embarqué

  --------------------------------------------------------------------------------------------- */
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

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

#ifdef MP3_READER
// --------------------------------------------------------------------------------------
//     Handle to /play for infos
// --------------------------------------------------------------------------------------
void playSound() {
  Serial.println("Requested '/play'");
  String content = "{ value : 0.0, ";
  content += "message : 'Playing...' }";
  server.send(200, "text/json", content);
  playSoundCallback();
}

// --------------------------------------------------------------------------------------
//     Handle to /pause for infos
// --------------------------------------------------------------------------------------
void pauseSound() {
  Serial.println("Requested '/pause'");
  String content = "{ value : 0.0, ";
  content += "message : 'Paused...' }";
  server.send(200, "text/json", content);
  pauseSoundCallback();
}

// --------------------------------------------------------------------------------------
//     Handle to /stop for infos
// --------------------------------------------------------------------------------------
void stopSound() {
  Serial.println("Requested '/stop'");
  String content = "{ value : 0.0, ";
  content += "message : 'Stopped !' }";
  server.send(200, "text/json", content);
  stopSoundCallback();
}

#endif

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
//    Send an url to someone
// --------------------------------------------------------------------------------------
void wwwSend(IPAddress host, String url) {
  Serial.print("connecting to ");
  Serial.println(humanReadableIp(host));

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(100);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

// --------------------------------------------------------------------------------------
//     Setting up WWW server
// --------------------------------------------------------------------------------------
void setup_www() {
  // Web server preparation
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

#ifdef MP3_READER
  server.on("/play", playSound);
  server.on("/pause", pauseSound);
  server.on("/stop", stopSound);
#endif
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

