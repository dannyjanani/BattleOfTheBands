#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

#include "chords.h"

// Special thanks to Matthew Rahtz - http://amid.fish/karplus-strong/

AudioSynthKarplusStrong  string1;
AudioSynthKarplusStrong  string2;
AudioSynthKarplusStrong  string3;
AudioSynthKarplusStrong  string4;
AudioSynthKarplusStrong  string5;
AudioSynthKarplusStrong  string6;
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 0);
AudioConnection          patchCord6(string5, 0, mixer2, 1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);
AudioConnection          patchCord8(mixer2, 0, i2s1, 0);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

const int finger_delay = 10;
const int hand_delay = 500;


void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
 
void displayDataRate(void)
{
  Serial.print  ("Data Rate:    ");
 
  switch(accel.getDataRate())
  {
    case ADXL343_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL343_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL343_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL343_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL343_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL343_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL343_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL343_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL343_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL343_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL343_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL343_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL343_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL343_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL343_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL343_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}
 
void displayRange(void)
{
  Serial.print  ("Range:         +/- ");
 
  switch(accel.getRange())
  {
    case ADXL343_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL343_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL343_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL343_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}


void setup() {
  //set up keypad status
  pinMode(0,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  mixer1.gain(0, 0.15);
  mixer1.gain(1, 0.15);
  mixer1.gain(2, 0.15);
  mixer1.gain(3, 0.15);
  mixer2.gain(1, 0.15);
  mixer2.gain(2, 0.15);
  delay(700);

  Serial.begin(9600);
  while (!Serial);
  Serial.println("Accelerometer Test"); Serial.println("");
 
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);}
     /* Set the range to whatever is appropriate for your project */
     accel.setRange(ADXL343_RANGE_16_G);
  // accel.setRange(ADXL343_RANGE_8_G);
  // accel.setRange(ADXL343_RANGE_4_G);
  // accel.setRange(ADXL343_RANGE_2_G);
 
  /* Display some basic information on this sensor */
  displaySensorDetails();
  displayDataRate();
  displayRange();
  Serial.println("");

}
void strum_up(const float *chord, float velocity);

void loop() {
  const float *chord;
  //read the signal from the digital pin 
  int keypad_1=digitalRead(0);
  int keypad_2=digitalRead(1);
  int keypad_3=digitalRead(2);
  int keypad_4=digitalRead(3);
  sensors_event_t event;
  accel.getEvent(&event);
  // each time through the loop, play a different chord
  if (keypad_1 == 0) {
     if (event.acceleration.y >9){
       chord = Cmajor;
       strum_up(chord, 1.0);
       delay(hand_delay);
       Serial.println("C major");
      }
  }  if (keypad_2 == 0) {
     if (event.acceleration.y >9){
    chord = Gmajor;
    strum_up(chord, 1.0);
    delay(hand_delay);
    Serial.println("G major");
     }
  }  if (keypad_3 == 0) {
    if (event.acceleration.y >9){
    chord = Aminor;
    strum_up(chord, 1.0);
    delay(hand_delay);
    Serial.println("A minor");
    }
  }  if (keypad_4 == 0){
    if (event.acceleration.y >9){
    chord = Fmajor;
    strum_up(chord, 1.0);
    delay(hand_delay);
    Serial.println("F major");
  }
}
}

void strum_up(const float *chord, float velocity)
{
  if (chord[0] > 20.0) string1.noteOn(chord[0], velocity);
  delay(finger_delay);
  if (chord[1] > 20.0) string2.noteOn(chord[1], velocity);
  delay(finger_delay);
  if (chord[2] > 20.0) string3.noteOn(chord[2], velocity);
  delay(finger_delay);
  if (chord[3] > 20.0) string4.noteOn(chord[3], velocity);
  delay(finger_delay);
  if (chord[4] > 20.0) string5.noteOn(chord[4], velocity);
  delay(finger_delay);
  if (chord[5] > 20.0) string6.noteOn(chord[5], velocity);
  delay(finger_delay);
}
