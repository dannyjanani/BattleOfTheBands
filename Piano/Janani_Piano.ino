#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// Sounds from https: https://freesound.org/people/dxe10/
// Audio trimmed by: https://audiotrimmer.com

// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=267,147
AudioPlaySdWav           playSdWav2;     //xy=267,193
AudioPlaySdWav           playSdWav3;     //xy=267,238
AudioPlaySdWav           playSdWav4;     //xy=267,284
AudioMixer4              mixer1;         //xy=467,197
AudioOutputI2S           i2s1;           //xy=685,200
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord3(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=537,392
// GUItool: end automatically generated code

const int button1 = 3;
const int button2 = 2;
const int button3 = 6;
const int button4 = 4;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 70;    // the debounce time (ms)

int isPressed1 = 0;
int oldIsPressed1 = 1;
int isPressed2 = 0;
int oldIsPressed2 = 1;
int isPressed3 = 0;
int oldIsPressed3 = 1;
int isPressed4 = 0;
int oldIsPressed4 = 1;

const char * filelist[4] = {
  "C_MAJOR.WAV", "A_MINOR.WAV", "F_MAJOR.WAV", "G_MAJOR.WAV"
};  

int buttonState1 = 0;      // variables for reading the button1 status
int buttonState2 = 0;      // variables for reading the button2 status
int buttonState3 = 0;      // variables for reading the button3 status
int buttonState4 = 0;      // variables for reading the button4 status

void setup() {
  // initialize the Serial Monitor @ 9600
  Serial.begin(9600);  
  
  // initialize the keypad pin(s) as an input:
  GPIOD_PDDR &= ~(1U << 0);  // set PD0 (pin 2) as an input 
  GPIOD_PDDR &= ~(1U << 4);  // set PD4 (pin 6) as an input
  GPIOA_PDDR &= ~(1U << 12); // set PA12 (pin 3) as an input
  GPIOA_PDDR &= ~(1U << 13); // set PA13 (pin 4) as an input

  //Enable GPIO and Pull-Down Resistors bits 1 and 8
  PORTD_PCR0  |= 0x00000102; // portD pin0 
  PORTD_PCR4  |= 0x00000102; // portD pin4
  PORTA_PCR12 |= 0x00000102; // portA pin12
  PORTA_PCR13 |= 0x00000102; // portA pin13
  
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);

  mixer1.gain(0, 0.1);
  mixer1.gain(1, 0.1);
  mixer1.gain(2, 0.1);
  mixer1.gain(3, 0.1);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  
  // check if SD card is being read
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);
}

void loop(){
  // play the piano
  playPiano();
}

void playPiano() {
  // read the state of the keypad value making sure we debounce the keypad
  // signifying the state of which the button is in by reading the appropriate pin #
  if ((millis() - lastDebounceTime) > debounceDelay){
    isPressed1 = GPIOA_PDIR & 0x00001000;  // digitalRead(Pin3) - port A, bit 12
    isPressed2 = GPIOD_PDIR & 0x00000001;  // digitalRead(Pin2) - port D, bit 0
    isPressed3 = GPIOD_PDIR & 0x00000010;  // digitalRead(Pin6) - port D, bit 4
    isPressed4 = GPIOA_PDIR & 0x00002000;  // digitalRead(Pin4) - port A, bit 13
  }
  // checking if it's pressed and changed status (it wasnt pressed before)
  if (isPressed1 == LOW && (oldIsPressed1 != isPressed1)) {    
    const char *filename = filelist[0]; 
    determineChannel(filename);
    // taking the time to reset the prev. debouncement time
    lastDebounceTime = millis();
  }
  if (isPressed2 == LOW && (oldIsPressed2 != isPressed2)) {
    const char *filename = filelist[1];
    determineChannel(filename);
    lastDebounceTime = millis();
  }
  if (isPressed3 == LOW && (oldIsPressed3 != isPressed3)) {    
    const char *filename = filelist[2]; 
    determineChannel(filename);
    lastDebounceTime = millis();
  }
  if (isPressed4 == LOW && (oldIsPressed4 != isPressed4)) {
    const char *filename = filelist[3]; 
    determineChannel(filename);
    lastDebounceTime = millis();
  }
  // variables to ensure changed status (it wasnt pressed before)
  oldIsPressed1 = isPressed1;
  oldIsPressed2 = isPressed2;
  oldIsPressed3 = isPressed3;
  oldIsPressed4 = isPressed4;
}

// check which channel of the mixer to use to play the desired note
void determineChannel(const char *filename) {
  if(playSdWav1.isPlaying() == false) {
    playSdWav1.play(filename);
  }
  else if(playSdWav2.isPlaying() == false){
    playSdWav2.play(filename);
  }
  else if(playSdWav3.isPlaying() == false) {
    playSdWav3.play(filename);
}
  else if(playSdWav4.isPlaying() == false) {
    playSdWav4.play(filename);
  }
  else{
    playSdWav1.stop();
    playSdWav1.play(filename);
  }
}
