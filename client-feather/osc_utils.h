/*---------------------------------------------------------------------------------------------

    osc_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    ENsemble de fonctions de réception et d'envoie de bundles OSC

  --------------------------------------------------------------------------------------------- */
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 2390;        // local port to listen for UDP packets for OSC messages

// --------------------------------------------------------------------------------------
//  Setting up UDP for Osc Send and Receive
// --------------------------------------------------------------------------------------
void setup_osc() {
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port : ");
  Serial.println(Udp.localPort());
  Serial.println("");
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
      //bundle.dispatch("/position", mySuperCallBack);


    } else {
      // Errors, print them
      OSCErrorCode error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
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


