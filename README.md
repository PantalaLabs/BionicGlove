- # BionicGlove

Bionic Glove from Pantala Labs (http://www.pantalalabs.com) is wearable glove that can control microcontrolled or not, devices , robots, mobile apps, desktop programs (like Ableton), games, art, entertainment, etc through the interpretation of their behaviors and movements.

Here you will find usage tips and an ESP32 library to the Bionic Glove.

There are two ways to use the glove.

a) using the library in an ESP32: the user uses an ESP32 to receive the data and the library to create his sketches without worrying about the instance of communication and interpretation of the data. This is the alternative for all types of users including beginners. 
The library creates an abstraction for the data received from the Bionic Glove and implements several functionalities without the headache of complicated coding.

b) without the library: the user will be responsible for reading and interpreting the data received by the glove via Serial Bluetooth. Inside BionicGlove.h you will find the description of the data packet that the glove transmits. This is the alternative for advanced users.

Library conventions:

Methods with parameters:

1. uint32_t ms - means millisseconds , you can use any long int number
2. uint8_t raw - refers to any of the raw data received in the data packet. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "RAW_" : 

- #define RAW_F_INDEX 0
- #define RAW_F_MIDDLE 1
- #define RAW_F_RING 2
- #define RAW_F_LITTLE 3
- #define RAW_A_X 4
- #define RAW_A_X_G 5
- #define RAW_A_X_DEG 6
- #define RAW_A_Y 7
- #define RAW_A_Y_G 8
- #define RAW_A_Y_DEG 9
- #define RAW_A_Z 10
- #define RAW_A_Z_G 11
- #define RAW_A_Z_DEG 12
- #define RAW_SMOOTHFACTOR 13

3. uint8_t axl - refers to any of the accelerometer axle. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "IDX_A_" 

- #define IDX_A_X 0
- #define IDX_A_Y 1
- #define IDX_A_Z 2

4. uint8_t f - refers to any of the fingers. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "FINGER_" 

- #define FINGER_INDEX 0
- #define FINGER_MIDDLE 1
- #define FINGER_RING 2
- #define FINGER_LITTLE 3

5. uint8_t pct - refers to any data that uses percentage. You can use any unsigned 8 bit number from 1 to 100

6. <any datatype> val - any value inside the allowed range. Must read all library.

7. float trs - refers to threshold parameters. You can use any value inside the allowed range. Must read all library.