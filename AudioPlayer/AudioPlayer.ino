// IMPORTANT NOTE:
// Files in the SD Card must be in the format: "[NO][NICKNAME].WAV" 
// and subsequent translated songs in "[NO]_[TRANSLATED_NO][NICKNAME].WAV"
// and must be 8 letters in total before ".WAV" due constraints with FAT-32

#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>
#define WAV_SAMPLE_RATE 44100 // Stereo sample rate in KHz
#define SD_CARD 28 // Port for SD Card (Default: 28)
#define BUTTON_PIN 1 // Port/pin for button

void setup() {
   // DEBUG output at 115200 baud
   Serial.begin(115200);
   // while (!Serial);
   
   // Setup SD card
   Serial.print("Initializing SD card...");
   if(!SD.begin(SD_CARD)) {
      Serial.println("  failed!");
      while (true);
   }
   Serial.println("  done.");
   
   // Prints all directory in SD Card for debugging
   printDirectory(SD.open("/"), 0);
   
   // Setup random object's seed
   randomSeed(analogRead(2)); // analog pin must be empty to 'randomise' from noise

   // Setup pins for button
   pinMode(BUTTON_PIN, INPUT);
   
   Serial.println();
}

void loop() {
   // Checks if there are ANY files in root
   File root = SD.open("/");
   File file = root.openNextFile();
   if (!file) {
      Serial.println("No files found. Ending Program...");
      while (true);
   }
   
   // Randomize which track to pick
   int num = countWav() + 1;
   // Serial.println("return: " + String(num)); // DEBUG prints no. of wav files
   int choose = random(1, num); 
   // int choose = 2; // DEBUG manually pick choose
   // Serial.println("choose: " + String(choose)); // DEBUG prints chosen int

   String choose0(choose);
   File wavFile = pickWav(choose0);
   // Checks if the file was opened and is valid
   if(!wavFile) {
      // if the file didn't open, print an error and stop
      Serial.println("Error opening file");
      while (true);
   }

   // Stops program until button is pressed
   int buttonState = 0;
   while(buttonState == 0) {
      buttonState = digitalRead(BUTTON_PIN);
      // Serial.println(buttonState); // debug prints button state
   };
   
   // Plays track
   String wavFileName(wavFile.name());
   playFile(wavFile, WAV_SAMPLE_RATE, "Playing " + wavFileName + "...", "  Finished playing " + wavFileName);


   // ------------------------------------------------------------------------------------------------------------------
   String choose1(choose + "_1");
   wavFile = pickWav(choose1);
   // Checks if the file was opened and is valid
   if(!wavFile) {
      // if the file didn't open, print an error and stop
      Serial.println("Error opening file");
      while (true);
   }

   // Stops program until button is pressed
   buttonState = 0;
   while(buttonState == 0) {
      buttonState = digitalRead(BUTTON_PIN);
      // Serial.println(buttonState); // debug prints button state
   };
   
   // Plays track
   String wavFileName1(wavFile.name());
   playFile(wavFile, WAV_SAMPLE_RATE, "Playing " + wavFileName1 + "...", "  Finished playing " + wavFileName1);

   // --------------------------------------------------------------------------------------------------------------------
   String choose2(choose + "_2");
   wavFile = pickWav(choose2);
   // Checks if the file was opened and is valid
   if(!wavFile) {
      // if the file didn't open, print an error and stop
      Serial.println("Error opening file");
      while (true);
   }
   
   // Stops program until button is pressed
   buttonState = 0;
   while(buttonState == 0) {
      buttonState = digitalRead(BUTTON_PIN);
      // Serial.println(buttonState); // debug prints button state
   };
   
   // Plays track
   String wavFileName2(wavFile.name());
   playFile(wavFile, WAV_SAMPLE_RATE, "Playing " + wavFileName2 + "...", "  Finished playing " + wavFileName2);   
}

// Counts how many .wav files there are in the SD card
// (TODO: implement counting only correctly formatted files)
int countWav() {
   File root = SD.open("/");
   File file = root.openNextFile();
   int fileCount = 0;
   
   while(file) {
      String fileName(file.name());
      if (fileName.endsWith(".WAV")) { // not a wav file
         fileCount++;
      }
      // Serial.println("Loop " + String(fileCount) + String(" Name: ") + fileName); // DEBUG prints no. of times it looped & prints file its checking
      file = root.openNextFile();
   }
   file.close();
   // Serial.println(fileCount); // DEBUG prints number of wav files
   return fileCount;
}

// Checks every file for the correct file from chosen random number
// (TODO: this can probably be improved by directly opening the file using some concatenation trick)
File pickWav(String choose) {
   File root = SD.open("/");
   File file = root.openNextFile();
   while(file) {
      // Serial.println(file.name()); // DEBUG prints file name
      // Serial.println("loop"); // DEBUG prints loop
      if(!file.isDirectory()) { // not a directory
         if(String(file.name()).startsWith(String(choose))) { // if correct song
            break;
         }
      }
   file = root.openNextFile();
   }
   // Serial.println(file.name()); // DEBUG prints output file   
   return file;
}

// Prints a starting msg then plays the given file based on given sample rate
// then prints a ending msg
void playFile(File file, int sampleRate, String startMsg, String endMsg) {
   Serial.println(startMsg);
   AudioZero.begin(sampleRate);
   AudioZero.play(file);
   AudioZero.end();
   Serial.println(endMsg);
   Serial.println();
}

void printDirectory(File dir, int numTabs) {
   while (true) {
      File entry =  dir.openNextFile();
      if (!entry) {
         // no more files
         break;
      }
      for (uint8_t i = 0; i < numTabs; i++) {
         Serial.print('\t');
      }
      Serial.print(entry.name());
      if (entry.isDirectory()) {
         Serial.println("/");
         printDirectory(entry, numTabs + 1);
      } else {
         // files have sizes, directories do not
         Serial.print("\t\t");
         Serial.println(entry.size(), DEC);
      }
      entry.close();
   }
}
