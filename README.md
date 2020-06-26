# Battle Of The Bands
Graduate Real-Time Embedded System Project to create a band out of "air instruments".

## Project Overview

A fun, Graduate Real-Time Embedded Systems project to create a band out of "air instruments". Three people, making three different experiments, and performing a 4-chord song in front of a class. We implemented a guitar, piano, and drum. The guitar was implemented by using an [accelerometer](https://www.livescience.com/40102-accelerometers.html) (to detect motion) as the guitar pick, as well as a [1x4 keypad](https://www.instructables.com/id/1x4-Membrane-Keypad-w-Arduino/) for the different chords. The piano was implemented using a [1x4 keypad](https://www.instructables.com/id/1x4-Membrane-Keypad-w-Arduino/) to play 4 different chords. Finally, the drums were implemented using 2 [accelerometers](https://www.livescience.com/40102-accelerometers.html), and different motions will make 4 different chords. This was a teambased effort, each person getting a chance to work with each component. We chose 1x4 keypads for the guitar and the piano due to the [FSRs](https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr) (force sensitive resistors - detects pressure) being sold out on Arduino's website. Overall, this was a great project to have fun and learn tremendously at the same time. 

## Project Goals

* Audio output from each band members will be fed to a mixer using an RCA jack.
* Audio amp, speakers, mixer, and audio cables will be provided to the “band.”
* Any sensor may be used for chord implementation.
* Each band is encouraged to have a “performance” look.
* The less the project the better it looks, but  minor props may be used such as a stick for the guitar, or a drawing of a  keyboard with keys on the desk to know where keys are, etc.
* Cannot play the song from the audio file directly.
* Song cannot contain inappropriate lyrics.
* No mosh pits.
* No crowd surfing.

* One distinct instrument per group member.
  * Example: Bass Guitar and Guitar are different instruments.
  * Example: Drum Sticks may be used but not with direct strikes.
  
## Gallery

Testing                    | Presentation               
:-------------------------:|:-------------------------:
![](https://github.com/dannyjanani/BattleOfTheBands/blob/master/Gallery/Testing.png)  | ![](https://github.com/dannyjanani/BattleOfTheBands/blob/master/Gallery/Presentation.png)

## Provided Materials

* [Audio Shield](https://www.pjrc.com/audio-shield-for-teensy-4-0/) for each group member.
* [Teensy3](https://www.pjrc.com/store/teensy32.html) for each group member.
* Additional materials such as sensors, protoshields, or other components have to be purchased by the team.
* A Four-Chord song was selected from [Wikipedia](https://en.wikipedia.org/wiki/List_of_songs_containing_the_I–V–vi–IV_progression).
* Unlimited Maker space resources (3d printing, laser cutting, etc.)
* Max Budget: $20 per instrument directly from Arduino.
  * Guitar BOM: 
    * 1x [Half-Sized Breadboard](https://www.adafruit.com/product/64)
    * 1x [1by4 Keypad](https://www.adafruit.com/product/1332)
    * 1x [Accelerometer](https://www.adafruit.com/product/1120)
  * Piano BOM: 
    * 1x [Half-Sized Breadboard](https://www.adafruit.com/product/64)
    * 1x [1by4 Keypad](https://www.adafruit.com/product/1332)
  * Drums BOM: 
    * 1x [Half-Sized Breadboard](https://www.adafruit.com/product/64)
    * 2x [Accelerometer](https://www.adafruit.com/product/1120)

## Skills Used

* Prototyping:
  * Laser Cutting
  * 3D Printing
  * Soldering
  
* Software:
  * C
  * Arduino IDE
  * SolidWorks
  * Ultimaker Cura
