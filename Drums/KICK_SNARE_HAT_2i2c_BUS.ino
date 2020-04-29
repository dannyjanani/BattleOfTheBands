#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>
//audio samples
#include "AudioSampleSnare.h"        
#include "AudioSampleHihat.h"
#include "AudioSampleKick.h"

//audio setup
AudioPlayMemory          sound_kick;
AudioPlayMemory          sound_snare;
AudioPlayMemory           sound_hat;
AudioPlaySdWav           playSnare;
AudioPlaySdWav           playKick;
AudioPlaySdWav           playHat;
AudioOutputI2S           audioOutput;
AudioMixer4              mixer1;         
//for wav sd playback
//AudioConnection          patchCord1(playKick, 0, mixer1, 0);
//AudioConnection          patchCord2(playSnare, 0, mixer1, 1);
//AudioConnection          patchCord3(playHat, 0, mixer1, 2);

//for sample playback
AudioConnection          patchCord1(sound_kick, 0, mixer1, 0);
AudioConnection          patchCord2(sound_snare, 0, mixer1, 1);
AudioConnection          patchCord3(sound_hat, 0, mixer1, 2);
AudioControlSGTL5000     sgtl5000_1;
AudioConnection          patchCord5(mixer1, 0, audioOutput, 0);
AudioConnection          patchCord6(mixer1, 0, audioOutput, 1);

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
//response tuning parameters
#define KICK_ON_TRIGGER 150
#define KICK_OFF_TRIGGER 100
#define SNARE_ON_TRIGGER 7  
#define SNARE_OFF_TRIGGER 2
#define HAT_ON_TRIGGER 7
#define HAT_OFF_TRIGGER 2
#define INTERVALSNARE 10
#define INTERVALKICK 10
#define INERVALHAT  10

/* Assign a unique ID to this sensor at the same time */
/* Uncomment following line for default Wire bus      */
//Adafruit_ADXL343 accel_snare = Adafruit_ADXL343(12346);
//try this line for alt adress for second sensor
Adafruit_ADXL343 accel_snare = Adafruit_ADXL343(29);
Adafruit_ADXL343 accel_hat = Adafruit_ADXL343(12346, &Wire1);


//Global Variables
//snare
unsigned long PreviousMillisSnare = 0;
int snare_just_hit=0;
int z_trigger_snare;
//kick
int fsrAnalogPin = 7; // FSR is connected to analog 0
int kick_trigger;      // the analog reading from the FSR resistor divider
unsigned long PreviousMillisKick = 0;

int kick_just_hit=0;
//hat
unsigned long PreviousMillisHat = 0;
int hat_just_hit=0;
int z_trigger_hat;
//general
unsigned long CurrentMillis;





void setup(void)
{
   Serial.begin(9600);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);
  digitalWrite(12,HIGH);
  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
//
 SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  //channel gains
  //kick gain
  mixer1.gain(0,1);
   //snare gain
  mixer1.gain(1,1);
  //hat gain
  mixer1.gain(2,0.3);
  
  while (!Serial);
  Serial.println("accel_snare Test"); Serial.println("");

  /* Check for snare sensor */
  if(!accel_snare.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

/* Check for hihat sensor */
if(!accel_hat.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }
  
  /* Set the range for both accel sensors */
  accel_snare.setRange(ADXL343_RANGE_16_G);
  accel_hat.setRange(ADXL343_RANGE_16_G);

 
  
}


//functions
void debounce_accel_snare(int *z_trig){
    int i=0;
    sensors_event_t event;
    int prev_trig = 0;//init prev state to null
    while (i<4){ //try get 3 readings in a row that are same
        accel_snare.getEvent(&event);
        *z_trig = event.acceleration.z;
        if (*z_trig==prev_trig){
            i++;
           }
        
        if (*z_trig!=prev_trig){
               i=0;
               }
        prev_trig=*z_trig;
        delay(1);
}
    
}

void debounce_accel_hat(int *z_trig){
    int i=0;
    sensors_event_t event;
    int prev_trig = 0;//init prev state to null
    while (i<3){ //try get 5 readings in a row that are same
        accel_hat.getEvent(&event);
        *z_trig = event.acceleration.z;
        if (*z_trig==prev_trig){
            i++;
           }
        
        if (*z_trig!=prev_trig){
               i=0;
               }
        prev_trig=*z_trig;
        delay(1);
}
    
}


void loop(void){
  CurrentMillis = millis();
  //snare drum
  debounce_accel_snare(&z_trigger_snare);
  if(z_trigger_snare>SNARE_ON_TRIGGER && CurrentMillis-PreviousMillisSnare>INTERVALSNARE && !snare_just_hit){
  PreviousMillisSnare=CurrentMillis;
//  playSnare.play("NEWSNARE.WAV");
  sound_snare.play(AudioSampleSnare);
  snare_just_hit=1;
  }
 
  //check accel again
 debounce_accel_snare(&z_trigger_snare);
 if(z_trigger_snare<SNARE_OFF_TRIGGER){
  //if the accel is low, stick must have been raised, clear just_hit flag
  snare_just_hit=0;
  
 }

//kick drum
CurrentMillis = millis();
kick_trigger = analogRead(fsrAnalogPin);
//  Serial.print("Trigger reading = ");
//  Serial.println(kick_trigger);

  if(kick_trigger>KICK_ON_TRIGGER && CurrentMillis-PreviousMillisKick>INTERVALKICK && !kick_just_hit){
  PreviousMillisKick=CurrentMillis;
  sound_kick.play(AudioSampleKick);
//  playKick.play("NEWKICK.WAV");
  
  kick_just_hit=1;
  //Serial.print("kick triggerd");
  }
 
  //check kick again
 kick_trigger = analogRead(fsrAnalogPin);
 if(kick_trigger<KICK_OFF_TRIGGER){
  //if the value is low, kick is not hit clear just_hit flag
  kick_just_hit=0;
  
 }

//hihat
  CurrentMillis = millis();
  debounce_accel_hat(&z_trigger_hat);
  if(z_trigger_hat>HAT_ON_TRIGGER && CurrentMillis-PreviousMillisHat>INERVALHAT && !hat_just_hit){
  PreviousMillisHat=CurrentMillis;
  sound_hat.play(AudioSampleHihat);
//  playHat.play("NEWHAT.WAV");
  hat_just_hit=1;
  }

  debounce_accel_hat(&z_trigger_hat);
 if(z_trigger_hat<HAT_OFF_TRIGGER){
  //if the accel is low, stick must have been raised, clear just_hit flag
  hat_just_hit=0;
  CurrentMillis = millis();
 }
//Serial.print("snare flag value");Serial.print(snare_just_hit);
//Serial.println("");
//Serial.print("kick flag value");Serial.print(kick_just_hit);
//Serial.println("");
//Serial.print("hat flag value");Serial.print(hat_just_hit);
//Serial.println("");
//
// 
// 
// 
//  delay(500);
}
