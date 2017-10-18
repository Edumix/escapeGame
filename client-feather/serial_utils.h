/*---------------------------------------------------------------------------------------------

    www_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Septembre 2017
    Ensemble de fonctions de gestion de la liaison Serial

  --------------------------------------------------------------------------------------------- */
#define SERIAL_SPEED 115200

// --------------------------------------------------------------------------------------
//   Setting up Serial
// --------------------------------------------------------------------------------------
void setup_serial() {
  Serial.begin(SERIAL_SPEED);
  delay(100);
  Serial.println("");
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


