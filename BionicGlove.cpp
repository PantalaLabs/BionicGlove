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
 *********************************************************************/

#include <BluetoothSerial.h>
#include "BionicGlove.h"

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

BionicGlove::BionicGlove()
{
  detachCallOnWideClosedFingerLittle(); // detach all
  detachCallOnWideClosedFingerIndex();
  detachCallOnWideClosedFingerMiddle();
  detachCallOnWideClosedFingerRing();
  detachCallOnWideOpenedFingerIndex();
  detachCallOnWideOpenedFingerMiddle();
  detachCallOnWideOpenedFingerRing();
  detachCallOnWideOpenedFingerLittle();
  detachCallOnFlickClosedFingerLittle();
  detachCallOnFlickClosedFingerIndex();
  detachCallOnFlickClosedFingerMiddle();
  detachCallOnFlickClosedFingerRing();
  detachCallOnFlickOpenedFingerIndex();
  detachCallOnFlickOpenedFingerMiddle();
  detachCallOnFlickOpenedFingerRing();
  detachCallOnFlickOpenedFingerLittle();
  detachCallOnVerticalPositiveKnock();
  detachCallOnVerticalNegativeKnock();
  detachCallOnHorizontalPositiveKnock();
  detachCallOnHorizontalNegativeKnock();
  setAllRedlinePercentage(DEFREDLINEPERCENTAGE); // set all critical area to 20%
  updateNewLimits();
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    setFlickOpenedThreshold(f, DEFFLICKTHRESHOLD);
    setFlickClosedThreshold(f, DEFFLICKTHRESHOLD);
  }
}

void BionicGlove::start()
{
  ledOnAsync();
  SerialBT.begin(device_name);
  SerialBT.setPin(pin);
  on = true;
  ledOffAsync();
}
uint32_t now;
uint32_t newprint;

bool BionicGlove::read()
{
  //uint32_t now;

  ledOffAsync();
  if (SerialBT.available()) //@ each 10ms - MASTER defined
  {
    receiveDataPack();
    updateNewLimits();
    // now = micros();
    logFremoveOffset();
    logAZGknock();
    if (doneMs(ts_frozen, frozen))
    {
      callbackClosedFinger();
      callbackOpenedFinger();
      callbackFlickLr();
      callbackKnockLr();
    }
    // Serial.println(micros() - now);

    return true;
  }
  return false;
}

void BionicGlove::receiveDataPack()
{
  // receive datapack
  serialData = SerialBT.readStringUntil('\n');
  uint8_t thisSeparator = 0, nextSeparator = 0;
  for (uint8_t i = 0; i < MAXBTDATAPACK; i++)
  {
    nextSeparator = serialData.indexOf(',', thisSeparator);
    btDataPack[i] = serialData.substring(thisSeparator, nextSeparator);
    thisSeparator = nextSeparator + 1;
    switch (i)
    {
    case 0:
      finger[0].fingerRead = btDataPack[0].toInt();
      break;
    case 1:
      finger[1].fingerRead = btDataPack[i].toInt();
      break;
    case 2:
      finger[2].fingerRead = btDataPack[i].toInt();
      break;
    case 3:
      finger[3].fingerRead = btDataPack[i].toInt();
      break;
    case 4:
      accel[AXL_X].raw = btDataPack[i].toFloat();
      break;
    case 5:
      accel[AXL_X].g = btDataPack[i].toFloat();
      ALPHAFILTER(lastAGsmoothed[AXL_X], accel[AXL_X].g, fixedSmoothCoeffToKnock);
      break;
    case 6:
      accel[AXL_X].ang = btDataPack[i].toFloat();
      ALPHAFILTER(lastAAngsmoothed[AXL_X], accel[AXL_X].ang, smoothFactor / 10);
      break;
    case 7:
      accel[AXL_Y].raw = btDataPack[i].toFloat();
      break;
    case 8:
      accel[AXL_Y].g = btDataPack[i].toFloat();
      ALPHAFILTER(lastAGsmoothed[AXL_Y], accel[AXL_Y].g, fixedSmoothCoeffToKnock);
      break;
    case 9:
      accel[AXL_Y].ang = btDataPack[i].toFloat();
      ALPHAFILTER(lastAAngsmoothed[AXL_Y], accel[AXL_Y].ang, smoothFactor / 10);
      break;
    case 10:
      accel[AXL_Z].raw = btDataPack[i].toFloat();
      break;
    case 11:
      accel[AXL_Z].g = btDataPack[i].toFloat();
      ALPHAFILTER(lastAGsmoothed[AXL_Z], accel[AXL_Z].g, fixedSmoothCoeffToKnock);
      break;
    case 12:
      accel[AXL_Z].ang = btDataPack[i].toFloat();
      ALPHAFILTER(lastAAngsmoothed[AXL_Z], accel[AXL_Z].ang, smoothFactor / 10);
      break;
    case 13:
      smoothFactor = btDataPack[i].toFloat();
      break;
    }
  }
}

float BionicGlove::getRaw(uint8_t raw)
{
  if ((raw >= 0) && (raw < MAXBTDATAPACK))
    return btDataPack[raw].toFloat();
  else
    return 0;
}

String BionicGlove::getSerialData()
{
  return serialData;
}

float BionicGlove::getAGsmoothed(uint8_t axl)
{
  return lastAGsmoothed[axl];
}

float BionicGlove::getAAngsmoothed(uint8_t axl)
{
  return lastAAngsmoothed[axl];
}

void BionicGlove::end()
{
  SerialBT.end();
  on = false;
}

bool BionicGlove::active()
{
  return on;
}

uint16_t BionicGlove::getF(uint8_t f) // get expanded finger value
{
  return finger[f].expanded;
}

float BionicGlove::getFaccel(uint8_t f)
{
  return finger[f].accel;
}

void BionicGlove::logAGremoveOffset()
{
  for (uint8_t j = 0; j < MAXACCELCHANNELS; j++)
  {
    for (uint8_t i = 0; i < (MAXKNOCKLOG - 2); i++)
    {
      logAG[j][i] = logAG[j][i + 1];
    }
    logAG[j][(MAXKNOCKLOG - 2)] = GETITEM(RAW_A_X_G);
  }
}

void BionicGlove::logFremoveOffset()
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    for (uint8_t i = 0; i < (MAXFLICKLOG - 2); i++)
    {
      logF[f][i] = logF[f][i + 1];
    }
    logF[f][(MAXFLICKLOG - 2)] = GETITEM(f) / 100;
  }
  // }
}

// linear regression to the 4 items array finger readings ~ 350us
// linear regression to the 3 items array finger readings ~ 270us
void BionicGlove::callbackFlickLr()
{
  if ((doneMs(ts_lastFlick, flickDebounceInterval)))
  {
    // uint32_t now = micros();
    for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    {
      for (uint8_t i = 0; i < MAXFLICKLINEARREGRESSIONLEARNS; i++) // learn
        lr.learn(i, logF[f][i]);
      lr.correlation();
      lr.parameters(values);
      finger[f].accel = values[0];
      // Serial.println(finger[f].accel);
      lr.reset();

      if ((finger[f].accel > 0) && (abs(finger[f].accel) > flickThreshold[f][OPENED]))
      {
        switch (f)
        {
        case RAW_F_INDEX:
          callFlickOpenedIndex();
          break;
        case RAW_F_MIDDLE:
          callFlickOpenedMiddle();
          break;
        case RAW_F_RING:
          callFlickOpenedRing();
          break;
        case RAW_F_LITTLE:
          callFlickOpenedLittle();
          break;
        }
        logFclear(f);
        ledOnAsync();
        ts_lastFlick = millis();
        break;
      }
      else if ((finger[f].accel < 0) && (abs(finger[f].accel) > flickThreshold[f][CLOSED]))
      {
        switch (f)
        {
        case RAW_F_INDEX:
          callFlickClosedIndex();
          break;
        case RAW_F_MIDDLE:
          callFlickClosedMiddle();
          break;
        case RAW_F_RING:
          callFlickClosedRing();
          break;
        case RAW_F_LITTLE:
          callFlickClosedLittle();
          break;
        }
        logFclear(f);
        ledOnAsync();
        ts_lastFlick = millis();
        break;
      }
    }
    // Serial.println(micros() - now);
  }
}

// old maths based on the accumulation of the difference between N and N-1 reading
void BionicGlove::callbackFlick()
{
  float acc;

  if ((doneMs(ts_lastFlick, flickDebounceInterval)))
  {
    for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    {
      acc = 0.0;
      for (uint8_t i = 1; i < MAXFLICKLOG; i++)
      {
        acc += (logF[f][i] - logF[f][i - 1]);
        if ((acc > 0) && (abs(acc) > flickThreshold[f][OPENED]))
        {
          // for (uint8_t k = 1; k <= i; k++)
          // {
          //   Serial.print(logF[f][k] - logF[f][k - 1]);
          //   Serial.println(",");
          // }
          // Serial.println(acc);
          // Serial.println("_");

          switch (f)
          {
          case RAW_F_INDEX:
            callFlickOpenedIndex();
            break;
          case RAW_F_MIDDLE:
            callFlickOpenedMiddle();
            break;
          case RAW_F_RING:
            callFlickOpenedRing();
            break;
          case RAW_F_LITTLE:
            callFlickOpenedLittle();
            break;
          }
          logFclear(f);
          ledOnAsync();
          ts_lastFlick = millis();
          break;
        }
        else if ((acc < 0) && (abs(acc) > flickThreshold[f][CLOSED]))
        {

          // for (uint8_t k = 1; k <= i; k++)
          // {
          //   Serial.print(logF[f][k] - logF[f][k - 1]);
          //   Serial.println(",");
          // }
          // Serial.println(acc);
          // Serial.println("_");

          switch (f)
          {
          case RAW_F_INDEX:
            callFlickClosedIndex();
            break;
          case RAW_F_MIDDLE:
            callFlickClosedMiddle();
            break;
          case RAW_F_RING:
            callFlickClosedRing();
            break;
          case RAW_F_LITTLE:
            callFlickClosedLittle();
            break;
          }
          logFclear(f);
          ledOnAsync();
          ts_lastFlick = millis();
          break;
        }
      }
    }
  }
}

void BionicGlove::logAZGknock() // FILO
{
  knockAllowed = !knockAllowed; // slow down Slave SR to (bionic glove master SR)/2
  if (knockAllowed)
  {
    for (uint8_t i = 0; i < (MAXKNOCKLOG - 1); i++)
    {
      logAZG[i] = logAZG[i + 1];
      logAZGsmoothed[i] = logAZGsmoothed[i + 1];
    }
    logAZG[MAXKNOCKLOG - 1] = GETITEM(RAW_A_Z_G) + AZGOFFSET;
    logAZGsmoothed[MAXKNOCKLOG - 1] = lastAGsmoothed[AXL_Z];
  }
}

void BionicGlove::callbackKnockLr()
{
  float rlResult;
  bool hit = false;

  if (doneMs(ts_lastKnock, knockDebounceInterval) && knockAllowed)
  {
    for (uint8_t i = 0; i < MAXKNOCKLINEARREGRESSIONLEARNS; i++) // learn
      lr.learn(i, logAZG[i] * 100);
    lr.correlation();
    lr.parameters(values);
    rlResult = values[0];
    lr.reset();

    // Serial.print(logAZGsmoothed[0]);
    // Serial.print(",");
    // Serial.println(rlResult);

    if ((logAZGsmoothed[0] > 0.6)) // vertical
    {
      if (rlResult > knockVerticalPositiveThreshold)
      {
        // Serial.println("vp");
        callVerticalPositiveKnock();
        hit = true;
      }
      else if (-rlResult > knockVerticalNegativeThreshold)
      {
        // Serial.println("vn");
        callVerticalNegativeKnock();
        hit = true;
      }
    }
    else // horizontal
    {
      if (rlResult > knockHorizontalPositiveThreshold)
      {
        // Serial.println("hp");
        callHorizontalPositiveKnock();
        hit = true;
      }
      else if (-rlResult > knockHorizontalNegativeThreshold)
      {
        // Serial.println("hn");
        callHorizontalNegativeKnock();
        hit = true;
      }
    }
    if (hit)
    {
      ts_lastKnock = millis();
      lastKnockAZG = logAZG[MAXKNOCKLOG - 1];
      // logAZGclear(); //do not clear - as soon as the array becomes filled again, theLR will hit another false knock
      ledOnAsync();
      hit = false;
    }
  }
}

void BionicGlove::updateNewLimits()
{
  int16_t croped;

  // if (firstReading)
  // {
  //   // if first time library was called
  //   // force some values to init properlly min and max
  //   // with real analogread values and not arbitrary development values
  //   firstReading = false;
  //   for (uint8_t i = 0; i < MAXFINGERCHANNELS; i++)
  //   {
  //     finger[i].closedMinValue = constrain(finger[i].fingerRead + 1, 0, 4095);
  //     finger[i].openedMaxValue = constrain(finger[i].fingerRead - 1, 0, 4095);
  //     // Serial.print(finger[i].closedMinValue);
  //     // Serial.print(",");
  //     // Serial.println(finger[i].openedMaxValue);
  //   }
  // }

  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    if (finger[f].fingerRead < finger[f].closedMinValue) // if read smaller than smallest
    {
      finger[f].closedMinValue = finger[f].fingerRead; // set new smallest
      updateClosedRedline(f);
    }
    else if (finger[f].fingerRead > finger[f].openedMaxValue) // if read bigger than biggest
    {
      finger[f].openedMaxValue = finger[f].fingerRead; // set new biggest
      updateOpenedRedline(f);
    }
    // expand
    croped = constrain(finger[f].fingerRead, finger[f].closedMinValue, finger[f].openedMaxValue);
    finger[f].expanded = map(finger[f].fingerRead, finger[f].closedMinValue, finger[f].openedMaxValue, 0, 4095);
  }
}

/*
smallest values on scale

fingerRead
       \                /
        \              /
         \            /
          \          / _ _ _ _ out
in _ _ _ _ \       //
            \\    //
             =====
*/
void BionicGlove::callbackClosedFinger()
{
  // closed pinch ------------------------------------------------------
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    if (!finger[f].closedFingerStatus && (finger[f].fingerRead < finger[f].closedRedLineIn))
    {
      finger[f].closedFingerStatus = true;
      switch (f)
      {
      case RAW_F_INDEX:
        callClosedIndex();
        break;
      case RAW_F_MIDDLE:
        callClosedMiddle();
        break;
      case RAW_F_RING:
        callClosedRing();
        break;
      case RAW_F_LITTLE:
        callClosedLittle();
        break;
      }
      ledOnAsync();
    }
    else if (finger[f].closedFingerStatus && (finger[f].fingerRead > finger[f].closedRedLineOut))
    {
      finger[f].closedFingerStatus = false;
    }
  }
}

/*
biggest values on scale
             =====
in _ _ _ _  //    \\
           /       \\ _ _ _ _ out
          /          \
         /            \
        /              \
       /                \
fingerRead
*/
void BionicGlove::callbackOpenedFinger()
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    if (!finger[f].openedFingerStatus && (finger[f].fingerRead > finger[f].openedRedLineIn))
    {
      finger[f].openedFingerStatus = true;
      switch (f)
      {
      case RAW_F_INDEX:
        callOpenedIndex();
        break;
      case RAW_F_MIDDLE:
        callOpenedMiddle();
        break;
      case RAW_F_RING:
        callOpenedRing();
        break;
      case RAW_F_LITTLE:
        callOpenedLittle();
        break;
      }
      ledOnAsync();
    }
    else if (finger[f].openedFingerStatus && (finger[f].fingerRead < finger[f].openedRedLineOut))
    {
      finger[f].openedFingerStatus = false;
    }
  }
}

void BionicGlove::setAllRedlinePercentage(uint8_t pct)
{
  setAllClosedRedLinePercentage(pct);
  setAllOpenedRedLinePercentage(pct);
}

void BionicGlove::setAllClosedRedLinePercentage(uint8_t pct)
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    setClosedRedLinePercentage(f, pct);
}
void BionicGlove::setClosedRedLinePercentage(uint8_t f, uint8_t pct)
{
  finger[f].closedRedLinePercentage = constrain(pct, MINPERCENTAGE, MAXPERCENTAGE);
  updateClosedRedline(f);
}

void BionicGlove::setAllOpenedRedLinePercentage(uint8_t pct)
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    setOpenedRedLinePercentage(f, pct);
}
void BionicGlove::setOpenedRedLinePercentage(uint8_t f, uint8_t pct)
{
  finger[f].openedRedLinePercentage = constrain(pct, MINPERCENTAGE, MAXPERCENTAGE);
  updateOpenedRedline(f);
}

/*
this is called when:
a-user sets by glove new finger limits
b-user sets by code new finger percentage / critical area
smallest values on scale

fingerRead
       \                /
        \              /
         \            /
          \          / _ _ _ _ out
in _ _ _ _ \       //
            \\    //
             =====
*/
void BionicGlove::updateClosedRedline(uint8_t f) // smallest values on scale
{
  int16_t availab = (finger[f].openedMaxValue - finger[f].closedMinValue); // available  scale
  finger[f].closedRedLineIn = finger[f].closedMinValue + ((abs(availab) * (finger[f].closedRedLinePercentage - SCHMITTTRIGGERPERCENTAGE)) / 100);
  finger[f].closedRedLineOut = finger[f].closedMinValue + ((abs(availab) * (finger[f].closedRedLinePercentage + SCHMITTTRIGGERPERCENTAGE)) / 100);
}

/*
this is called when:
a-user sets by glove new finger limits
b-user sets by code new finger percentage / critical area
biggest values on scale
             =====
in _ _ _ _  //    \\
           /       \\ _ _ _ _ out
          /          \
         /            \
        /              \
       /                \
  fingerRead

*/
void BionicGlove::updateOpenedRedline(uint8_t f)
{
  int16_t availab = (finger[f].openedMaxValue - finger[f].closedMinValue); // available  scale
  finger[f].openedRedLineIn = finger[f].openedMaxValue - ((abs(availab) * (finger[f].openedRedLinePercentage - SCHMITTTRIGGERPERCENTAGE)) / 100);
  finger[f].openedRedLineOut = finger[f].openedMaxValue - ((abs(availab) * (finger[f].openedRedLinePercentage + SCHMITTTRIGGERPERCENTAGE)) / 100);
}

bool BionicGlove::getFclosedStatus(uint8_t f)
{
  return finger[f].closedFingerStatus;
}

bool BionicGlove::getFopenedStatus(uint8_t f)
{
  return finger[f].openedFingerStatus;
}

void BionicGlove::ledOnAsync()
{
  if (ledBuiltInActive)
  {
    digitalWrite(BULTINLED, HIGH);
    turnOffLed = millis();
  }
}

void BionicGlove::ledOffAsync()
{
  if (ledBuiltInActive)
  {
    if ((turnOffLed > 0) && doneMs(turnOffLed, 5))
    {
      turnOffLed = 0;
      digitalWrite(BULTINLED, LOW);
    }
  }
}

// wide  -----------------------------------------------------------
void BionicGlove::attachCallOnWideClosedFingerLittle(void (*onRise)())
{
  callClosedLittle = onRise;
}
void BionicGlove::detachCallOnWideClosedFingerLittle()
{
  attachCallOnWideClosedFingerLittle(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideClosedFingerRing(void (*onRise)())
{
  callClosedRing = onRise;
}
void BionicGlove::detachCallOnWideClosedFingerRing()
{
  attachCallOnWideClosedFingerRing(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideClosedFingerMiddle(void (*onRise)())
{
  callClosedMiddle = onRise;
}
void BionicGlove::detachCallOnWideClosedFingerMiddle()
{
  attachCallOnWideClosedFingerMiddle(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideClosedFingerIndex(void (*onRise)())
{
  callClosedIndex = onRise;
}
void BionicGlove::detachCallOnWideClosedFingerIndex()
{
  attachCallOnWideClosedFingerIndex(isrDefaultUnused);
}

void BionicGlove::attachCallOnWideOpenedFingerLittle(void (*onRise)())
{
  callOpenedLittle = onRise;
}
void BionicGlove::detachCallOnWideOpenedFingerLittle()
{
  attachCallOnWideOpenedFingerLittle(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideOpenedFingerRing(void (*onRise)())
{
  callOpenedRing = onRise;
}
void BionicGlove::detachCallOnWideOpenedFingerRing()
{
  attachCallOnWideOpenedFingerRing(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideOpenedFingerMiddle(void (*onRise)())
{
  callOpenedMiddle = onRise;
}
void BionicGlove::detachCallOnWideOpenedFingerMiddle()
{
  attachCallOnWideOpenedFingerMiddle(isrDefaultUnused);
}
void BionicGlove::attachCallOnWideOpenedFingerIndex(void (*onRise)())
{
  callOpenedIndex = onRise;
}
void BionicGlove::detachCallOnWideOpenedFingerIndex()
{
  attachCallOnWideOpenedFingerIndex(isrDefaultUnused);
}

// flick -----------------------------------------------------------
void BionicGlove::attachCallOnFlickClosedFingerLittle(void (*onRise)())
{
  callFlickClosedLittle = onRise;
}
void BionicGlove::detachCallOnFlickClosedFingerLittle()
{
  attachCallOnFlickClosedFingerLittle(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickClosedFingerRing(void (*onRise)())
{
  callFlickClosedRing = onRise;
}
void BionicGlove::detachCallOnFlickClosedFingerRing()
{
  attachCallOnFlickClosedFingerRing(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickClosedFingerMiddle(void (*onRise)())
{
  callFlickClosedMiddle = onRise;
}
void BionicGlove::detachCallOnFlickClosedFingerMiddle()
{
  attachCallOnFlickClosedFingerMiddle(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickClosedFingerIndex(void (*onRise)())
{
  callFlickClosedIndex = onRise;
}
void BionicGlove::detachCallOnFlickClosedFingerIndex()
{
  attachCallOnFlickClosedFingerIndex(isrDefaultUnused);
}

void BionicGlove::attachCallOnFlickOpenedFingerLittle(void (*onRise)())
{
  callFlickOpenedLittle = onRise;
}
void BionicGlove::detachCallOnFlickOpenedFingerLittle()
{
  attachCallOnFlickOpenedFingerLittle(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickOpenedFingerRing(void (*onRise)())
{
  callFlickOpenedRing = onRise;
}
void BionicGlove::detachCallOnFlickOpenedFingerRing()
{
  attachCallOnFlickOpenedFingerRing(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickOpenedFingerMiddle(void (*onRise)())
{
  callFlickOpenedMiddle = onRise;
}
void BionicGlove::detachCallOnFlickOpenedFingerMiddle()
{
  attachCallOnFlickOpenedFingerMiddle(isrDefaultUnused);
}
void BionicGlove::attachCallOnFlickOpenedFingerIndex(void (*onRise)())
{
  callFlickOpenedIndex = onRise;
}
void BionicGlove::detachCallOnFlickOpenedFingerIndex()
{
  attachCallOnFlickOpenedFingerIndex(isrDefaultUnused);
}

// knocks -----------------------------------------------------------
void BionicGlove::attachCallOnVerticalPositiveKnock(void (*onRise)(void))
{
  callVerticalPositiveKnock = onRise;
}
void BionicGlove::detachCallOnVerticalPositiveKnock()
{
  attachCallOnVerticalPositiveKnock(isrDefaultUnused);
}
void BionicGlove::attachCallOnVerticalNegativeKnock(void (*onRise)(void))
{
  callVerticalNegativeKnock = onRise;
}
void BionicGlove::detachCallOnVerticalNegativeKnock()
{
  attachCallOnVerticalNegativeKnock(isrDefaultUnused);
}

void BionicGlove::attachCallOnHorizontalPositiveKnock(void (*onRise)(void))
{
  callHorizontalPositiveKnock = onRise;
}
void BionicGlove::detachCallOnHorizontalPositiveKnock()
{
  attachCallOnHorizontalPositiveKnock(isrDefaultUnused);
}
void BionicGlove::attachCallOnHorizontalNegativeKnock(void (*onRise)(void))
{
  callHorizontalNegativeKnock = onRise;
}
void BionicGlove::detachCallOnHorizontalNegativeKnock()
{
  attachCallOnHorizontalNegativeKnock(isrDefaultUnused);
}

void BionicGlove::logFclear(uint8_t f)
{
  for (uint8_t i = 0; i < (MAXFLICKLOG - 1); i++)
  {
    logF[f][i] = 0;
  }
}

void BionicGlove::logAZGclear()
{
  for (uint8_t i = 0; i < (MAXKNOCKLOG - 2); i++)
    logAZG[i] = logAZG[MAXKNOCKLOG - 1];
}

void BionicGlove::isrDefaultUnused()
{
}

void BionicGlove::setLedBuiltIn()
{
  ledBuiltInActive = true;
  pinMode(BULTINLED, OUTPUT);
}

void BionicGlove::setKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg)
{
  // discard out of range values
  if ((val_verPos > MINKNOCKTHRESHOLD) && (val_verPos < MINKNOCKTHRESHOLD))
    knockVerticalPositiveThreshold = constrain(val_verPos, MINKNOCKTHRESHOLD, MAXKNOCKTHRESHOLD);
  if ((val_verNeg > MINKNOCKTHRESHOLD) && (val_verNeg < MINKNOCKTHRESHOLD))
    knockVerticalNegativeThreshold = constrain(val_verNeg, MINKNOCKTHRESHOLD, MAXKNOCKTHRESHOLD);
  if ((val_horPos > MINKNOCKTHRESHOLD) && (val_horPos < MINKNOCKTHRESHOLD))
    knockHorizontalPositiveThreshold = constrain(val_horPos, MINKNOCKTHRESHOLD, MAXKNOCKTHRESHOLD);
  if ((val_horNeg > MINKNOCKTHRESHOLD) && (val_horNeg < MINKNOCKTHRESHOLD))
    knockHorizontalNegativeThreshold = constrain(val_horNeg, MINKNOCKTHRESHOLD, MAXKNOCKTHRESHOLD);
}

void BionicGlove::setFlickAllThreshold(float trs)
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    setFlickOpenedThreshold(f, trs);
    setFlickClosedThreshold(f, trs);
  }
}

void BionicGlove::setFlickOpenedThreshold(uint8_t f, float trs)
{
  flickThreshold[f][OPENED] = constrain(trs, MINFLICKTHRESHOLD, MAXFLICKTHRESHOLD);
}

void BionicGlove::setFlickClosedThreshold(uint8_t f, float trs)
{
  flickThreshold[f][CLOSED] = constrain(trs, MINFLICKTHRESHOLD, MAXFLICKTHRESHOLD);
}

float BionicGlove::getAZGlastKnock()
{
  return lastKnockAZG;
}

void BionicGlove::setKnockDebounceInterval(uint32_t val)
{
  knockDebounceInterval = val;
}

void BionicGlove::setFlickDebounceInterval(uint32_t val)
{
  flickDebounceInterval = val;
}

void BionicGlove::freeze(uint32_t ms)
{
  ts_frozen = millis();
  frozen = ms;
}

bool BionicGlove::doneMs(uint32_t t0, uint32_t dt)
{
  return done(millis(), t0, dt);
}

bool BionicGlove::doneUs(uint32_t t0, uint32_t dt)
{
  return done(micros(), t0, dt);
}

bool BionicGlove::done(uint32_t t, uint32_t t0, uint32_t dt)
{
  return ((t >= t0) && (t - t0 >= dt)) ||     // normal case
         ((t < t0) && (t + (~t0) + 1 >= dt)); // overflow case
}
