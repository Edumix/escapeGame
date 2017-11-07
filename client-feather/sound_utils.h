// Specifically for use with the Adafruit Feather, the pins are pre-set here!

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

// Feather M0 or 32u4
#if defined(__AVR__) || defined(ARDUINO_SAMD_FEATHER_M0)
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

// Feather ESP8266
#elif defined(ESP8266)
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

// Feather ESP32
#elif defined(ESP32)
#define VS1053_CS      32     // VS1053 chip select pin (output)
#define VS1053_DCS     33     // VS1053 Data/command select pin (output)
#define CARDCS         14     // Card chip select pin
#define VS1053_DREQ    15     // VS1053 Data request, ideally an Interrupt pin

// Feather Teensy3
#elif defined(TEENSYDUINO)
#define VS1053_CS       3     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          8     // Card chip select pin
#define VS1053_DREQ     4     // VS1053 Data request, ideally an Interrupt pin

// WICED feather
#elif defined(ARDUINO_STM32_FEATHER)
#define VS1053_CS       PC7     // VS1053 chip select pin (output)
#define VS1053_DCS      PB4     // VS1053 Data/command select pin (output)
#define CARDCS          PC5     // Card chip select pin
#define VS1053_DREQ     PA15    // VS1053 Data request, ideally an Interrupt pin

#elif defined(ARDUINO_FEATHER52)
#define VS1053_CS       30     // VS1053 chip select pin (output)
#define VS1053_DCS      11     // VS1053 Data/command select pin (output)
#define CARDCS          27     // Card chip select pin
#define VS1053_DREQ     31     // VS1053 Data request, ideally an Interrupt pin
#endif


Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

// --------------------------------------------------------------------------------------
//  Setting up the MP3/Music Maker Feather Wing
// --------------------------------------------------------------------------------------
void setup_sound() {
  // if you're using Bluefruit or LoRa/RFM Feather, disable the BLE interface
  //pinMode(8, INPUT_PULLUP);

  Serial.println("Adafruit VS1053 Feather : MP3 Reader/Recorder");

  if (! musicPlayer.begin()) { // initialise the music player
    //sendJSONMsg("ERROR : Couldn't find VS1053, do you have the right pins defined?");
    Serial.println(F("ERROR : Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }

  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
    //sendJSONMsg("ERROR : SD failed, or not present");
    Serial.println(F("ERROR : SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(5, 5);

#if defined(__AVR_ATmega32U4__)
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  // but we don't have them on the 32u4 feather...
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
#elif defined(ESP32)
  // no IRQ! doesn't work yet :/
#else
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
#endif
}

boolean started = false;
String soundCmd = "";

// --------------------------------------------------------------------------------------
//  Playing sound Callback
// --------------------------------------------------------------------------------------
void playSoundCallback() {
  soundCmd = "play";
}

// --------------------------------------------------------------------------------------
//  Pausig sound Callback
// --------------------------------------------------------------------------------------
void pauseSoundCallback() {
  soundCmd = "pause";
}

// --------------------------------------------------------------------------------------
//  Stopping sound Callback
// --------------------------------------------------------------------------------------
void stopSoundCallback() {
  soundCmd = "stop";
}

// --------------------------------------------------------------------------------------
//  Driving Sound
// --------------------------------------------------------------------------------------
void soundDriver() {
  if (soundCmd == "play") {
    if (!started) {
      // Start reading...
      Serial.println("Start reading...");
      musicPlayer.startPlayingFile("TRACK001.MP3");
      started = true;
    } else {
      if (!musicPlayer.stopped()) {
      // continue reading...
      Serial.println("Resume reading...");
      musicPlayer.pausePlaying(false);
      } else {
        // The mp3 is finished, but the feather still receive a "play" order.
        // Re-init for the next loop !
        started = false;
      }
    }
  }

  if (soundCmd == "pause") {
    if (! musicPlayer.paused()) {
      Serial.println("Paused");
      musicPlayer.pausePlaying(true);
    }
  }

  if (soundCmd == "stop") {
      Serial.println("stop");
      musicPlayer.stopPlaying();
      started = false;
  }

  if (musicPlayer.stopped()) {
    Serial.println("Sound has ended !");
    musicPlayer.pausePlaying(false);
    started = false;
  }
  soundCmd = ""; // Command has been treated.
}

