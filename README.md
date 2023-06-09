# BionicGlove

Bionic Glove from Pantala Labs (http://www.pantalalabs.com) is wearable glove that can control microcontrolled or not, devices , robots, mobile apps, desktop programs (like Ableton), games, art, entertainment, etc through the interpretation of their behaviors and movements.

Here you will find usage tips and an ESP32 library to the Bionic Glove.

There are two ways to use the glove.

a) using the library in an ESP32: the user uses an ESP32 to receive the data and the library to create his sketches without worrying about the instance of communication and interpretation of the data. This is the alternative for all types of users including beginners. 
The library creates an abstraction for the data received from the Bionic Glove and implements several functionalities without the headache of complicated coding.

b) without the library: the user will be responsible for reading and interpreting the data received by the glove via Serial Bluetooth. Inside BionicGlove.h you will find the description of the data packet that the glove transmits. This is the alternative for advanced users.



