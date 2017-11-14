/*---------------------------------------------------------------------------------------------

    buttons_utils.h pour les dispositifs "escape game"
    Erasme - Pierre-Gilles Levallois - Novembre 2017
    Ensemble de fonctions de gestion d'entrées par boutons (bi-stables et mono-stables)

  --------------------------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------------------------

            -------------
  PINSDA 4  | --------- | PIN9  1 x
  PINSCL 5  | |       | | PIN10  3
  PIN2   2  | |ESP8266| | 
  PIN3  16  | |       | | 
  PIN4   0  | --------- | 
  PIN5  15  |           |
  PIN6  13  |           |
  PIN7  12  |           |
  PIN8  14  |           |
            |           |
            |    ___    |
            |    | |    |
            -------------

  --------------------------------------------------------------------------------------------- */

// Feather ESP8266 I/O
// 4 et 5 sont réservées pour la communication I2C avec les feathersWings
#define PINSDA 4
#define PINSCL 5

#define PIN2 2
#define PIN3 16 
#define PIN4 0  // Buildin LED
#define PIN5 15
#define PIN6 13
#define PIN7 12
#define PIN8 14 

//#define PIN9 1  // Mettre un niveau High sur cette pin bloque tout ???
#define PIN10 3
//#define PIN11 12
//#define PIN12 13
//#define PIN13 14

#define NUM_PINS 8
const int PINS[NUM_PINS] = {PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN10};

// --------------------------------------------------------------------------------------
//  Initialisation
// --------------------------------------------------------------------------------------
void setup_sensor() {
  // @TODO : Charger une conf I/O depuis un répo central.
  Serial.println("Init I/O...");
   for (int i = 0; i < NUM_PINS; i++) {
    Serial.print("  Init pin #"); Serial.print(i); Serial.print(" -> "); Serial.print(PINS[i]);
    pinMode(PINS[i], INPUT);
    digitalWrite(PINS[i], HIGH);
    Serial.println("... Okay !");
    delay(100);
  }
   Serial.println("Init I/O done !");
}

