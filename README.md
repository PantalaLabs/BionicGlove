- # BionicGlove

## This is an early version of the readme and constantly being updated.
All content is still being organized.

Please : 
* If you find bugs, please open an Issue.
* If you have a new feature in mind, please open a Issue with title "Feature Request".
![prototype](/pictures/issue.png)


Bionic Glove from Pantala Labs (http://www.pantalalabs.com) is wearable glove that can control microcontrolled or not, devices , robots, mobile apps, desktop programs (like Ableton), games, art, entertainment, etc through the interpretation of their behaviors and movements.

### Prototypes

4th proto

![prototype](/pictures/IMG-0911.jpg) |
![prototype](/pictures/IMG-0912.jpg) |
![prototype](/pictures/IMG-0913.jpg)

5th proto

![prototype](/pictures/IMG-0933-small.jpg) |
![prototype](/pictures/IMG-0934-small.jpg)

### Comunication hardware

There are two ways to use the glove.

a) with the library and an ESP32: create sketches without worrying about the instance of communication and interpretation of the data. This is the alternative for all types of users including beginners. The library creates an abstraction for the data received from the Bionic Glove and implements several functionalities without the headache of complicated coding.

All applications will read from any COM dinamically assigned to EP32. COM20 in this example:

![esp_cycle](/pictures/serial_esp_cycle.jpg) 

b) without the library: the user will be responsible for receiving and manage all the data received via Serial Bluetooth. You will need an Bluetooth compatible dongle on your computer. Inside BionicGlove.h you will find the description of the data pack sent by the glove. This is for advanced users.

All applications will read from any COM dinamically assigned to Bluetooth Dongle. COM24 or COM25 or COM26 in this example:

![esp_cycle](/pictures/serial_dongle_cycle.jpg) 

REMEMBER : there is no delay in using ESP + Library instead of Serial Bluetooth. The processing time consumed by ESP + Library are much smaller than the interval between messages sent by the glove.

### Library

Here you will find usage tips and an ESP32 library to the Bionic Glove.

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
<td>each of all 4 fingers absolute position. It tracks every position of the finger</td>
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
<td>everytime a high acceleration is reached when opening or closing a finger, like a flick, you can trigger a callback everytime a flick reaches a threshold</td>
<td>value</td>
<td>8 - one callback for each finger at opening or closing threshold</td>
</tr>

<tr>
<td>finger limits</td>
<td>everytime a finger reaches a position wide closed and/or opened, you can trigger a callback. the limits are configurable </td>
<td>percentage</td>
<td>8 - one callback for each finger at wide open or wide close</td>
</tr>
<tr>

<tr>
<td>accelerometers</td>
<td>x and y axes accelerometers</td>
<td>n/a</td>
<td>2 - one value for each axle</td>
</tr>

<tr>
<td>inclinometer</td>
<td>x and y angles</td>
<td>n/a</td>
<td>2 - one value for each axle</td>
</tr>

<tr>
<td>hand knock</td>
<td>everytime you simulate a knock, like a door knock, you create a trigger inside the library. The knock can be in a horizontal or a vertical orientation and positive (up or right) and/or negative (down or left) way</td>
<td>value</td>
<td>4 - one callback for each vertical/horizontal , positive/negative </td>
</tr>

</table>

#### Dependencies:

There are two mathematical calculations that are widely used in the library:
1. EMA (https://en.wikipedia.org/wiki/Moving_average) : this one is implemented by a macro called ALPHAFILTER in the BionicGlove.h
2. Linear Regression (https://en.wikipedia.org/wiki/Linear_regression) : this is a library LinearRegression.h by https://github.com/cubiwan/Regressino . You will need to download and install it too.

#### Conventions:

Every method has one kind of parameter. Be careful not to mix the types of parameters or unexpected results may happen.

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

3. uint8_t axl - refers to any of the accelerometer axle. You can use any unsigned 8 bit number, or any of the DEFINES available that starts with "AXL_"

- #define AXL_X 0
- #define AXL_Y 1
- #define AXL_Z 2

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
There is a Max4Live component called BionicGlove freely available. It abstracts almost total glove behavior. This component needs an ESP32 receiver attached to tour Windows/Mac.
A second component called BionicGlove Lite is in the way. It will be smaller than the the above, receive the datapack directly from Serial Bluetooth and cover only finger position and X/Y accelerometer angles.


![ableton](/pictures/bgableton.png)


##### Max4Live <-> Ableton

By default Max4Live does not map MIDI straight to Ableton. But there is a solution:

WINDOWS / LoopMIDI

In this situation the M4L component will send a MIDI message to LoopMIDI and Ableton will receive thru MIDI From/All Ins.

1. Download and install https://www.tobias-erichsen.de/software/loopmidi.html

2. Configure a new LoopMIDI port name. Ex : M4L

![midiloop](/pictures/midiloop1.png)

3. Now send the MAX4Live signal to M4L midiloop port

![midiloop](/pictures/midiloop2.png)

4. Right-click on the BionicGlove control you want to map, and choose the EDIT MIDI MAP option. Do the mapping to the slave control.

![midiloop](/pictures/midiloop3.png)

