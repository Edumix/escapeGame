/*---------------------------------------------------------------------------------------------

    www_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    Ensemble de fonctions de gestion du wifi

  --------------------------------------------------------------------------------------------- */

// --------------------------------------------------------------------------------------
//     Setting up WLAN
// --------------------------------------------------------------------------------------
void setup_wlan(char* ssid, char* pass) {
  Serial.print("Connecting to SSID [");
  Serial.print(ssid);
  Serial.print("] pass [");
  Serial.print(pass);
  Serial.println("]");

  WiFi.begin(ssid, pass);

  int k = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if (k % 80 == 0) {
      Serial.println("");
    }
    k++;
    delay(500);
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.print("-> Mac address : ");
  Serial.println(humanReadableMacAddr());
  Serial.print("-> IP address : ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
