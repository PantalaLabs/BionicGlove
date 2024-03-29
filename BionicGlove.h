/********************************************************************
* This is a library for the Bionic Glove.
*
* You'll find an example which should enable you to use the library.
*
* You are free to use it, change it or build on it. In case you like
* it, it would be cool if you give it a star.
*
* If you find bugs, please open an Issue  https://github.com/PantalaLabs/BionicGlove
* If you want new features , please open a Issue with title Feature Request at https://github.com/PantalaLabs/BionicGlove
*
* Gibran Curtiss Salomão 2023
* http://www.pantalalabs.com
*
TODO :

 * Bionic Glove.cpp
 * Author: Gibran Curtiss Salomão 2023


DATAPACK
0->

1-finger[0].fingerRead
2-finger[1].fingerRead
3-finger[2].fingerRead
4-finger[3].fingerRead

X
5-accel[0].raw
6-accel[0].g
7-accel[0].ang

Y
8-accel[1].raw
9-accel[1].g
10-accel[1].ang

Z
11-accel[2].raw
12-accel[2].g
13-accel[2].ang

14-smoothness

15-<*


Never use Serial.begin(9600) with boudrate above 38400!!!!!!!!!!!

**********************************************************************/

#ifndef BionicGlove_h
#define BionicGlove_h

#define BULTINLED 2

#define MAXBTDATAPACK 14

#define DATA_F_INDEX 0
#define DATA_F_MIDDLE 1
#define DATA_F_RING 2
#define DATA_F_LITTLE 3
#define DATA_A_X 4
#define DATA_A_X_G 5
#define DATA_A_X_ANG 6
#define DATA_A_Y 7
#define DATA_A_Y_G 8
#define DATA_A_Y_ANG 9
#define DATA_A_Z 10
#define DATA_A_Z_G 11
#define DATA_A_Z_ANG 12
#define DATA_SMOOTHFACTOR 13

#define FINGER_INDEX 0
#define FINGER_MIDDLE 1
#define FINGER_RING 2
#define FINGER_LITTLE 3

#define AXL_X 0
#define AXL_Y 1
#define AXL_Z 2

#define AZGOFFSET 10.0

#define MAXFINGERCHANNELS 4
#define MAXACCELCHANNELS 3

#define MINPERCENTAGE 10          // closed fingers
#define MAXPERCENTAGE 90          // opened fingers
#define DEFTHRESHOLDPERCENTAGE 15 // default for all fingers
#define DEFTHRESHOLDANGLE 30      // default for all axles

#define MINANGLE 10
#define MAXANGLE 170

#define MAXKNOCKLOG 10                               // 10 samples @ 10ms master Sample Rate = 100ms to take a picture of the mountain ** preciselly adjusted , do not touch
#define DEFKNOCKDEBOUNCEINTERVAL 300                 // time in ms to consider next knock
#define MAXANGLEKNOCKLOG 4                           // 4 samples @ 10ms
#define MINANGLEKNOCKTHRESHOLD 20.0                  // min value expected out of LR calcs
#define MAXANGLEKNOCKTHRESHOLD 150.0                 // max value expected out of LR calcs
#define DEFANGLEKNOCKVERTICALPOSITIVETHRESHOLD 90    //  ** preciselly adjusted , do not touch
#define DEFANGLEKNOCKVERTICALNEGATIVETHRESHOLD 100   //  ** preciselly adjusted , do not touch
#define DEFANGLEKNOCKHORIZONTALPOSITIVETHRESHOLD 100 //  ** preciselly adjusted , do not touch
#define DEFANGLEKNOCKHORIZONTALNEGATIVETHRESHOLD 95  //  ** preciselly adjusted , do not touch

// D E P R E C A T E D
// #define MINLRKNOCKTHRESHOLD 2.0                    // min value expected out of LR calcs
// #define MAXLRKNOCKTHRESHOLD 50.0                   // max value expected out of LR calcs
// #define DEFLRKNOCKVERTICALPOSITIVETHRESHOLD 15.0   //  ** preciselly adjusted , do not touch
// #define DEFLRKNOCKVERTICALNEGATIVETHRESHOLD 15.0   //  ** preciselly adjusted , do not touch
// #define DEFLRKNOCKHORIZONTALPOSITIVETHRESHOLD 13.0 //  ** preciselly adjusted , do not touch
// #define DEFLRKNOCKHORIZONTALNEGATIVETHRESHOLD 15.0 //  ** preciselly adjusted , do not touch

// D E P R E C A T E D
// #define MINSIMPLEKNOCKTHRESHOLD 0.2                   // min value expected out of LR calcs
// #define MAXSIMPLEKNOCKTHRESHOLD 3.0                   // max value expected out of LR calcs
// #define DEFSIMPLEKNOCKVERTICALPOSITIVETHRESHOLD 1.5   //  ** preciselly adjusted , do not touch
// #define DEFSIMPLEKNOCKVERTICALNEGATIVETHRESHOLD 1.8   //  ** preciselly adjusted , do not touch
// #define DEFSIMPLEKNOCKHORIZONTALPOSITIVETHRESHOLD 0.8 //  ** preciselly adjusted , do not touch
// #define DEFSIMPLEKNOCKHORIZONTALNEGATIVETHRESHOLD 0.8 //  ** preciselly adjusted , do not touch
// #define MAXKNOCKLINEARREGRESSIONLEARNS 4              // 90us each LR round

#define FIXEDSMOOTHCOEFFTOKNOCK 0.03

#define MAXFLICKLOG 9                    // 9 samples @ 10ms master Sample Rate = 90ms to climb the flick  ** preciselly adjusted , do not touch
#define MINFLICKTHRESHOLD 10.5           // min value expected out of flick calcs
#define MAXFLICKTHRESHOLD 40.0           // max value expected out of flick calcs
#define DEFFLICKOPENEDTHRESHOLD 27.0     //  ** preciselly adjusted , do not touch
#define DEFFLICKCLOSEDTHRESHOLD 31.0     //  ** preciselly adjusted , do not touch
#define DEFFLICKDEBOUNCEINTERVAL 400     // time in ms to considert next flick
#define MAXFLICKLINEARREGRESSIONLEARNS 3 // 90us each LR round

#define CLOSED 0
#define OPENED 1

#define MAXRES 0b111111111111

#define SCHMITTTRIGGERPERCENTAGE 5
#define SCHMITTTRIGGERANGLE 5

#define BIONICEURO_D1_PIN 5
#define BIONICEURO_D2_PIN 18
#define BIONICEURO_D3_PIN 19
#define BIONICEURO_D4_PIN 21

#define BIONICEURO_A1_PIN 12
#define BIONICEURO_A2_PIN 27
#define BIONICEURO_A3_PIN 26
#define BIONICEURO_A4_PIN 32

#define BIONICEURO_A5_PIN 13
#define BIONICEURO_A6_PIN 14
#define BIONICEURO_A7_PIN 25
#define BIONICEURO_A8_PIN 33

#define BIONICEURO_D5_PIN 15
#define BIONICEURO_D6_PIN 16
#define BIONICEURO_D7_PIN 17

#if defined DO_DEBUG
#define BIONICEURO_D8_PIN 17
#else
#define BIONICEURO_D8_PIN 3
#endif

#define BIONICEURO_D9_PIN 2
#define BIONICEURO_D10_PIN 4
#if defined DO_DEBUG
#define BIONICEURO_D11_PIN 22
#else
#define BIONICEURO_D11_PIN 1
#endif
#define BIONICEURO_D12_PIN 22

#define BIONICEURO_POT1PIN 35
#define BIONICEURO_POT2PIN 34

#define BIONICEURO_SWITCH1PIN 36
#define BIONICEURO_SWITCH2PIN 39

#define BIONICEURO_PWMFREQ 0b1111111111111
#define BIONICEURO_PWMRES 12

#define BIONICEURO_A1_CHANNEL 0
#define BIONICEURO_A2_CHANNEL 1
#define BIONICEURO_A3_CHANNEL 2
#define BIONICEURO_A4_CHANNEL 3
#define BIONICEURO_A5_CHANNEL 4
#define BIONICEURO_A6_CHANNEL 5
#define BIONICEURO_A7_CHANNEL 6
#define BIONICEURO_A8_CHANNEL 7
#define BIONICEURO_D1_CHANNEL BIONICEURO_D1_PIN
#define BIONICEURO_D2_CHANNEL BIONICEURO_D2_PIN
#define BIONICEURO_D3_CHANNEL BIONICEURO_D3_PIN
#define BIONICEURO_D4_CHANNEL BIONICEURO_D4_PIN
#define BIONICEURO_D5_CHANNEL BIONICEURO_D5_PIN
#define BIONICEURO_D6_CHANNEL BIONICEURO_D6_PIN
#define BIONICEURO_D7_CHANNEL BIONICEURO_D7_PIN
#define BIONICEURO_D8_CHANNEL BIONICEURO_D8_PIN
#define BIONICEURO_D9_CHANNEL BIONICEURO_D9_PIN
#define BIONICEURO_D10_CHANNEL BIONICEURO_D10_PIN
#define BIONICEURO_D11_CHANNEL BIONICEURO_D11_PIN
#define BIONICEURO_D12_CHANNEL BIONICEURO_D12_PIN

#define ALPHAFILTER(tar, amt, alpha)                                            \
  {                                                                             \
    tar = (((float)(alpha) * (float)(amt)) + ((1.0 - (alpha)) * (float)(tar))); \
  }

#define GETITEM(idx) btDataPack[idx].toFloat()

class BionicGlove
{
public:
  BionicGlove();

  void start();                       // begin BT communication
  void end();                         // end BT communication
  bool read();                        // read BT serial
  bool send(String s);                // BT serial response
  bool active();                      // return if BT is active
  void freeze(uint32_t n);            // freeze any callback for n ms
  void setBuiltInLed(bool status);    // use built in led to visual info
  String getSerialData();             // return one line of serial data pack
  float getRaw(uint8_t raw);          // get raw value from each one value at BT pack
  float getSmooth(uint8_t raw);       // get smoothed value from each one value at BT pack
  float getUnit(uint8_t raw);         // get any token from -1 to +1
  float getUnitSmoothed(uint8_t raw); // get any smoothed token from -1 to +1
  // bool getGloveId();                                                                                   // get glove id
  uint16_t getF(uint8_t f);                                                                            // get expanded finger value
  void setAllFingersThresholdPercentage(uint8_t pct);                                                  // set all closed and opened thresholds
  void setAllClosedFingersThresholdPercentage(uint8_t pct);                                            // set all closed Percentage thresholds for all fingers
  void setAllOpenedFingersThresholdPercentage(uint8_t pct);                                            // set all opened Percentage thresholds for all fingers
  void setClosedFingerThresholdPercentage(uint8_t f, uint8_t pct);                                     // set closed Percentage for individual finger
  void setOpenedFingerThresholdPercentage(uint8_t f, uint8_t pct);                                     // set opened Percentage for individual finger
  bool getFingerClosedStatus(uint8_t f);                                                               // return if the finger is still inside closed area
  bool getFingerOpenedStatus(uint8_t f);                                                               // return if the finger is still inside opened area
  void setAngleKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg); // set new angle knock treshold
  void setKnockDebounceInterval(uint32_t val);                                                         // set new knock debounce interval
  void setFlickAllThreshold(float trs);                                                                // set all new flick treshold
  void setFlickOpenedThreshold(uint8_t f, float trs);                                                  // set new finger positive flick treshold
  void setFlickClosedThreshold(uint8_t f, float trs);                                                  // set new  finger negative flick treshold
  void setFlickAllOpenedThreshold(float trs);                                                          // set new finger positive flick treshold
  void setFlickAllClosedThreshold(float trs);                                                          // set new  finger negative flick treshold
  void setFlickDebounceInterval(uint32_t val);                                                         // set new flick  debounce interval
  void setAllAxleThresholdAngle(uint8_t ang);                                                          // set all axels threshold angles
  void setAxleMinThresholdAngle(uint8_t axl, uint8_t ang);                                             // set axel min threshold angle
  void setAxleMaxThresholdAngle(uint8_t axl, uint8_t ang);                                             // set axel min threshold angle
  bool getAxleMinStatus(uint8_t axl);                                                                  // get axel min status
  bool getAxleMaxStatus(uint8_t axl);                                                                  // get axel max status
  // float getFaccel(uint8_t f);                                                                          // DEPRECATED return acceleration based on linear regression coeff
  // void setSimpleKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg); // DEPRECATED set new simple knock treshold
  // void setLrKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg);     // DEPRECATED set new linear regression knock treshold

  // attach and detach
  void attachCallOnWideClosedFingerLittle(void (*onRise)(void));
  void detachCallOnWideClosedFingerLittle();
  void attachCallOnWideClosedFingerRing(void (*onRise)(void));
  void detachCallOnWideClosedFingerRing();
  void attachCallOnWideClosedFingerMiddle(void (*onRise)(void));
  void detachCallOnWideClosedFingerMiddle();
  void attachCallOnWideClosedFingerIndex(void (*onRise)(void));
  void detachCallOnWideClosedFingerIndex();

  void attachCallOnWideOpenedFingerLittle(void (*onRise)(void));
  void detachCallOnWideOpenedFingerLittle();
  void attachCallOnWideOpenedFingerRing(void (*onRise)(void));
  void detachCallOnWideOpenedFingerRing();
  void attachCallOnWideOpenedFingerMiddle(void (*onRise)(void));
  void detachCallOnWideOpenedFingerMiddle();
  void attachCallOnWideOpenedFingerIndex(void (*onRise)(void));
  void detachCallOnWideOpenedFingerIndex();

  void attachCallOnMinXangle(void (*onRise)());
  void detachCallOnMinXangle();
  void attachCallOnMaxXangle(void (*onRise)());
  void detachCallOnMaxXangle();
  void attachCallOnMinYangle(void (*onRise)());
  void detachCallOnMinYangle();
  void attachCallOnMaxYangle(void (*onRise)());
  void detachCallOnMaxYangle();

  void attachCallOnFlickClosedFingerLittle(void (*onRise)(void));
  void detachCallOnFlickClosedFingerLittle();
  void attachCallOnFlickClosedFingerRing(void (*onRise)(void));
  void detachCallOnFlickClosedFingerRing();
  void attachCallOnFlickClosedFingerMiddle(void (*onRise)(void));
  void detachCallOnFlickClosedFingerMiddle();
  void attachCallOnFlickClosedFingerIndex(void (*onRise)(void));
  void detachCallOnFlickClosedFingerIndex();

  void attachCallOnFlickOpenedFingerLittle(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerLittle();
  void attachCallOnFlickOpenedFingerRing(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerRing();
  void attachCallOnFlickOpenedFingerMiddle(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerMiddle();
  void attachCallOnFlickOpenedFingerIndex(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerIndex();

  void attachCallOnVerticalPositiveKnock(void (*onRise)(void));
  void detachCallOnVerticalPositiveKnock();
  void attachCallOnVerticalNegativeKnock(void (*onRise)(void));
  void detachCallOnVerticalNegativeKnock();

  void attachCallOnHorizontalPositiveKnock(void (*onRise)(void));
  void detachCallOnHorizontalPositiveKnock();
  void attachCallOnHorizontalNegativeKnock(void (*onRise)(void));
  void detachCallOnHorizontalNegativeKnock();

  void startEurorack();
  int8_t eurorackAnalogOutChannels[8] = {BIONICEURO_A1_CHANNEL, BIONICEURO_A2_CHANNEL, BIONICEURO_A3_CHANNEL, BIONICEURO_A4_CHANNEL, BIONICEURO_A5_CHANNEL, BIONICEURO_A6_CHANNEL, BIONICEURO_A7_CHANNEL, BIONICEURO_A8_CHANNEL};
  int8_t eurorackDigitalOutChannels[12] = {BIONICEURO_D1_CHANNEL, BIONICEURO_D2_CHANNEL, BIONICEURO_D3_CHANNEL, BIONICEURO_D4_CHANNEL, BIONICEURO_D5_CHANNEL, BIONICEURO_D6_CHANNEL, BIONICEURO_D7_CHANNEL, BIONICEURO_D8_CHANNEL, BIONICEURO_D9_CHANNEL, BIONICEURO_D10_CHANNEL, BIONICEURO_D11_CHANNEL, BIONICEURO_D12_CHANNEL};

private:
  const float zerof = 0.0;
  uint32_t frozen = 0;
  uint32_t ts_frozen = 0;                // last time a event happen
  uint32_t ts_nextLed;                   // last time led was turned on
  bool builtInLedActive = false;         // flags if built is working
  String serialData = "";                // receive the BT string line on each READ
  String btDataPack[MAXBTDATAPACK];      // receive splitted dataSerial string
  float smoothedDataPack[MAXBTDATAPACK]; // all smoothed datatpack tokens
  String message;
  bool on = false; // flags if BT is active
  const char *pin = "1234A";                                                          // default slave pin
  String device_name = "BIONIC_BT";                                                // default slave name
  float logF[MAXFINGERCHANNELS][MAXFLICKLOG] = {0};                                  // log finger readings to apply  offset removal
  float flickThreshold[MAXFINGERCHANNELS][2] = {0};                                  // flick activation limiar
  float knockVerticalPositiveThreshold = DEFANGLEKNOCKVERTICALPOSITIVETHRESHOLD;     // positive knock activation limiar
  float knockVerticalNegativeThreshold = DEFANGLEKNOCKVERTICALNEGATIVETHRESHOLD;     // negative knock activation limiar
  float knockHorizontalPositiveThreshold = DEFANGLEKNOCKHORIZONTALPOSITIVETHRESHOLD; // positive knock activation limiar
  float knockHorizontalNegativeThreshold = DEFANGLEKNOCKHORIZONTALNEGATIVETHRESHOLD; // negative knock activation limiar
  float knockPower[MAXACCELCHANNELS] = {0};                                          // last G value when knock was unlocked
  float smoothFactor = 0;                                                            // smooth factor received from MASTER
  float logKnock[MAXANGLEKNOCKLOG] = {0};                                            // last Y angle readings to compute knock
  // float logAZG[MAXKNOCKLOG] = {0};                                                   // last Accel Z G readings to compute knock
  float logAZGorientation[MAXKNOCKLOG] = {0};                // smoothed accel Z G readings to define if the hand is in normal or twisted posiction
  float lastAGfixedSmooth[MAXACCELCHANNELS] = {0};           // smoothed RAW accels
  float lastAAngFixedSmooth[MAXACCELCHANNELS] = {0};         // smoothed RAW accels
  uint32_t knockDebounceInterval = DEFKNOCKDEBOUNCEINTERVAL; // time in ms between to allowed knocks
  uint32_t ts_lastKnock = 0;                                 // millis() + knockInterval
  uint32_t flickDebounceInterval = DEFFLICKDEBOUNCEINTERVAL; // time in ms between to allowed knocks
  uint32_t ts_lastFlick = 0;                                 // millis() + knockInterval
  void init();                                               // start procedures
  void ledOnAsync();                                         // led turn on
  void ledOffAsync();                                        // async led turn off
  void detachAll();                                          // detach all callbacks
  bool receiveDataPack();                                    // receive BT serial string and split
  void callbackClosedFinger();                               // check if any finger reached closed area and callback them
  void callbackOpenedFinger();                               // check if any finger reached opened area and callback them
  void logFingers();                                         // log all fingers
  void callbackFlick();                                      // remove offset accumullating the difference btween x - (x-1)
  void callbackAxles();                                      // check if any finger reached closed area and callback them
  void updateNewLimits();                                    // compare if new readings are outside preset area and update to new ones
  void logAZGforHandOrientation();                           // put new finger read into knock array
  float getKnockCriteria();                                  // compute knock criteria
  void feedKnockCriteria(float item);                        // update knock array
  void callbackAngleKnock();                                 // calculate knock based on last 3 axle angle readings
  float getKnockPower(uint8_t axl);                          // return last max knock value
  void updateClosedThreshold(uint8_t f);                     // update individual closed finger threshold and recalculate all limits
  void updateOpenedThreshold(uint8_t f);                     // update individual opened finger threshold and recalculate all limits
  void updateAxleMinThreshold(uint8_t axl);                  // update axle min threshold
  void updateAxleMaxThreshold(uint8_t axl);                  // update axle max threshold
  void logFclear(uint8_t f);                                 // clear all last finger readings
  bool doneMs(uint32_t t0, uint32_t dt);                     // time rollover
  bool doneUs(uint32_t t0, uint32_t dt);                     // time rollover
  bool done(uint32_t t, uint32_t t0, uint32_t dt);           // time rollover
  // void logAGremoveOffset();                               // DEPRECATED stores last MAXLOGs values of 3 G accell axle to eventually remove its offsets
  // void logAZGclear();                                     // DEPRECATED clear log for knock calculations
  // float getAAngsmoothed(uint8_t axl);                     // DEPRECATED get smoothed accel Angle values
  // float getlastAGfixedSmooth(uint8_t axl);                // DEPRECATED get last smoothed accel G raw values
  // float getAGsmoothed(uint8_t axl);                       // DEPRECATED get smoothed accel G raw values
  // LinearRegression lr = LinearRegression();               // DEPRECATED
  // double values[2];                                       // DEPRECATED
  // void callbackFlickLr();                                 // DEPRECATED apply linear regression to 4 readings
  // float logAG[MAXACCELCHANNELS][MAXKNOCKLOG] = {0};       // DEPRECATED log all accel axles G readings to aplly offset removal
  // void callbackSimpleKnock();                             // DEPRECATED integrate ZG signal to find knock condition
  // void callbackKnockLr();                                 // DEPRECATED integrate ZG signal to find knock condition

  void eurorackSetup(); // start euroack interface
  int8_t eurorackAnalogOutPins[8] = {BIONICEURO_A1_PIN, BIONICEURO_A2_PIN, BIONICEURO_A3_PIN, BIONICEURO_A4_PIN, BIONICEURO_A5_PIN, BIONICEURO_A6_PIN, BIONICEURO_A7_PIN, BIONICEURO_A8_PIN};
  int8_t eurorackDigitalOutPins[12] = {BIONICEURO_D1_PIN, BIONICEURO_D2_PIN, BIONICEURO_D3_PIN, BIONICEURO_D4_PIN, BIONICEURO_D5_PIN, BIONICEURO_D6_PIN, BIONICEURO_D7_PIN, BIONICEURO_D8_PIN, BIONICEURO_D9_PIN, BIONICEURO_D10_PIN, BIONICEURO_D11_PIN, BIONICEURO_D12_PIN};

  // callbacks
  void (*callClosedLittle)(void);
  void (*callClosedRing)(void);
  void (*callClosedMiddle)(void);
  void (*callClosedIndex)(void);

  void (*callOpenedLittle)(void);
  void (*callOpenedRing)(void);
  void (*callOpenedMiddle)(void);
  void (*callOpenedIndex)(void);

  void (*callMinX)(void);
  void (*callMaxX)(void);
  void (*callMinY)(void);
  void (*callMaxY)(void);

  void (*callFlickClosedLittle)(void);
  void (*callFlickClosedRing)(void);
  void (*callFlickClosedMiddle)(void);
  void (*callFlickClosedIndex)(void);

  void (*callFlickOpenedLittle)(void);
  void (*callFlickOpenedRing)(void);
  void (*callFlickOpenedMiddle)(void);
  void (*callFlickOpenedIndex)(void);

  void (*callVerticalPositiveKnock)(void);
  void (*callVerticalNegativeKnock)(void);
  void (*callHorizontalPositiveKnock)(void);
  void (*callHorizontalNegativeKnock)(void);

  static void isrDefaultUnused(); // to dettach callbacks

  typedef struct
  {
    int16_t fingerRead = 0;                                     // raw finger read value
    uint8_t closedThresholdPercentage = DEFTHRESHOLDPERCENTAGE; // percent value set by user to calculate internal closed critical area
    uint8_t openedThresholdPercentage = DEFTHRESHOLDPERCENTAGE; // percent value set by user to calculate internal iopened critical area
    uint16_t closedThresholdIn = 0;                             // finger closed critical zone in
    uint16_t closedThresholdOut = 0;                            // finger closed critical zone out
    uint16_t openedThresholdIn = 0;                             // finger opened critical zone in
    uint16_t openedThresholdOut = 0;                            // finger opened critical zone out
    bool closedFingerStatus = false;                            // status flagging that you are inside critical area
    bool openedFingerStatus = false;                            // status flagging that you are inside critical area
    float accel = 0;
  } record_finger;
  record_finger finger[MAXFINGERCHANNELS];

  typedef struct
  {
    float raw = 0;                                 // raw accel value
    float g = 0;                                   // g accel value
    float ang = 0;                                 // corrected angle accel value
    uint8_t minThresholdAngle = DEFTHRESHOLDANGLE; // x axle critical zone point
    uint8_t minThresholdIn = 0;                    // x axle critical zone in
    uint8_t minThresholdOut = 0;                   // x axle critical zone out
    bool minThresholdStatus = false;               // status flagging that you are inside critical area
    uint8_t maxThresholdAngle = DEFTHRESHOLDANGLE; // x axle critical zone point
    uint8_t maxThresholdIn = 0;                    // x axle critical zone in
    uint8_t maxThresholdOut = 0;                   // x axle critical zone out
    bool maxThresholdStatus = false;               // status flagging that you are inside critical area
  } record_accel;
  record_accel accel[MAXACCELCHANNELS];
  bool firstReading = true;
  bool knockAllowed = true;
  uint32_t turnOffLed = 0;
};

#endif
