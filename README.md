- # BionicGlove

## This is an early version of the readme and is constantly being updated. All content is still being organized.

Bionic Glove from Pantala Labs (http://www.pantalalabs.com) is wearable glove that can control microcontrolled or not, devices , robots, mobile apps, desktop programs (like Ableton), games, art, entertainment, etc through the interpretation of their behaviors and movements.

### Prototype

![prototype](/pictures/IMG-0911.jpg)
![prototype](/pictures/IMG-0912.jpg)
![prototype](/pictures/IMG-0913.jpg)

### Library

Here you will find usage tips and an ESP32 library to the Bionic Glove.

There are two ways to use the glove.

a) using the library in an ESP32: the user uses an ESP32 to receive the data and the library to create his sketches without worrying about the instance of communication and interpretation of the data. This is the alternative for all types of users including beginners.
The library creates an abstraction for the data received from the Bionic Glove and implements several functionalities without the headache of complicated coding.

b) without the library: the user will be responsible for reading and interpreting the data received by the glove via Serial Bluetooth. You will need an Bluetooth compatible dongle on your PC. Inside BionicGlove.h you will find the description of the data packet that the glove transmits. This is the alternative for advanced users.

#### Movement/position recognition:

These are all features coverd by glove. Many of them have parameters to ajust to your needs.
<table>

<tr>
<td>feature</td>
<td>description</td>
<td>parameter</td>
<td>response</td>
</tr>

<tr>
<td>finger position</td>
<td>each of all 4 fingers absolute position</td>
<td>n/a</td>
<td>4 - one value for each finger</td>
</tr>

<tr>
<td>finger acceleration</td>
<td>regardless of the position of the fingers, when moving, it generates an acceleration. which increases when you start the movement and decreases as the finger returns to rest</td>
<td>n/a</td>
<td>4 - one value for each finger</td>

</tr>
<td>finger flick</td>
<td>everytime a high acceleration is reached when opening or closing a finger, like a flick, you can trigger a callback</td>
<td>value</td>
<td>8 - one callback for each finger at opening or closing</td>
</tr>

<tr>
<td>finger limits</td>
<td>configurable finger wide closed and/or opened limits</td>
<td>percentage</td>
<td>8 - one callback for each finger at wide open or wide close</td>
</tr>
<tr>

<tr>
<td>hand knock</td>
<td>configurable vertical/horizontal , positive/negative hand knock</td>
<td>value</td>
<td>4 - one callback for each vertical/horizontal , positive/negative </td>
</tr>

</table>

#### Dependencies:

2 math calculations in my code:
1. EMA (https://en.wikipedia.org/wiki/Moving_average) : this one is implemented by a macro called ALPHAFILTER in the BionicGlove.h
2. Linear Regression (https://en.wikipedia.org/wiki/Linear_regression) : this one I call a library LinearRegression.h by https://github.com/cubiwan/Regressino . You will need to download and install this library too.

#### Conventions:

Methods with parameters. Every method has one kind of parameter. Be careful not to mix the types of parameters or unexpected results may happen.

1. uint32_t ms - means millisseconds , you can use any unsigned long int number
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

3. uint8_t axl - refers to any of the accelerometer axle. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "AXL_A_"

- #define AXL_A_X 0
- #define AXL_A_Y 1
- #define AXL_A_Z 2

4. uint8_t f - refers to any of the fingers. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "FINGER_"

- #define FINGER_INDEX 0
- #define FINGER_MIDDLE 1
- #define FINGER_RING 2
- #define FINGER_LITTLE 3

5. uint8_t pct - refers to any data that uses percentage. You can use any unsigned 8 bit number from 1 to 100 , if allowed
6. any datatype val - any value inside the allowed range. Must read all library.
7. float trs - refers to threshold parameters. You can use any value inside the allowed range. Must read all library.

#### Examples
All  examples are sorted by difficulty.
Even the most difficult one is a very simple esketch with a few lines.

#### Ableton
There is a Live4Max component called BionicGlove freely available. It abstracts almost total glove behavior. This component needs an ESP32 receiver attached to tour Windows/Mac.

![ableton](/pictures/bgableton.png)

I´m developing a second component called BionicGlove Lite that will be smaller than the the above, and will receive the datapack directly from Serial Bluetooth and will cover only finger position and X/Y accel angles.
