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
  detachAll();
  setAllFingersThresholdPercentage(DEFTHRESHOLDPERCENTAGE); // set all critical area to 20%
  setAllAxleThresholdAngle(DEFTHRESHOLDANGLE);              // set all critical area to 30 degrees
  updateNewLimits();
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    setFlickAllThreshold(DEFFLICKTHRESHOLD);

  setAngleKnockThreshold(DEFANGLEKNOCKVERTICALPOSITIVETHRESHOLD, DEFANGLEKNOCKVERTICALNEGATIVETHRESHOLD, DEFANGLEKNOCKHORIZONTALPOSITIVETHRESHOLD, DEFANGLEKNOCKHORIZONTALNEGATIVETHRESHOLD);
  // D E P R E C A T E D !!!!!!!!!!!!!!!!!!!!!!!!!
  // setSimpleKnockThreshold(DEFSIMPLEKNOCKVERTICALPOSITIVETHRESHOLD, DEFSIMPLEKNOCKVERTICALNEGATIVETHRESHOLD, DEFSIMPLEKNOCKHORIZONTALPOSITIVETHRESHOLD, DEFSIMPLEKNOCKHORIZONTALNEGATIVETHRESHOLD);
  // setLrKnockThreshold(DEFLRKNOCKVERTICALPOSITIVETHRESHOLD, DEFLRKNOCKVERTICALNEGATIVETHRESHOLD, DEFLRKNOCKHORIZONTALPOSITIVETHRESHOLD, DEFLRKNOCKHORIZONTALNEGATIVETHRESHOLD);
}

// begin BT communication
void BionicGlove::start()
{
  setBuiltInLed(true);
  ledOnAsync();
  delay(500); // wait a little bit to start BT. avoid high inrush
  SerialBT.setPin(pin);
  SerialBT.begin(device_name);
  on = true;
  ledOffAsync();
}

// read BT serial
bool BionicGlove::read()
{
  ledOffAsync();
  if (SerialBT.available()) //@ each 10ms - MASTER defined
  {
    if (doneMs(ts_nextLed, 3000))
    {
      ts_nextLed = millis();
      ledOnAsync();
    }
    if (receiveDataPack()) // valid and imported datapack
    {
      // updateNewLimits();
      logFingers();
      logAZGknock();
      if (doneMs(ts_frozen, frozen))
      {
        callbackClosedFinger();
        callbackOpenedFinger();
        // callbackFlickLr();     // deprecated
        callbackFlick();
        callbackAngleKnock();
        // callbackSimpleKnock(); // deprecated
        // callbackKnockLr();     // deprecated
        callbackAxles();
      }
      return true;
    }
  }
  return false;
}

bool BionicGlove::receiveDataPack()
{
  // receive datapack
  int8_t thisSeparator = 1, nextSeparator = 1, separatorCounter = 0;

  serialData = SerialBT.readStringUntil('<');                    // get all buffer chars up to EXCLUDING first '<'
  SerialBT.flush();                                              // flush extra chars
  if (serialData.indexOf('>', 0) == -1)                          // if incomplete message
    return false;                                                // discard read
  serialData = serialData.substring(2, serialData.length() - 1); // trim edges
  serialData += " ";
  do // count token separators
  {
    nextSeparator = serialData.indexOf(' ', thisSeparator);
    thisSeparator = nextSeparator + 1;
    separatorCounter++;
  } while (nextSeparator != -1);
  if (separatorCounter == (MAXBTDATAPACK + 1)) // message with accepted number of tokens
  {
    thisSeparator = 0;
    nextSeparator = 0;
    for (uint8_t i = 0; i < MAXBTDATAPACK; i++)
    {
      nextSeparator = serialData.indexOf(' ', thisSeparator);
      btDataPack[i] = serialData.substring(thisSeparator, nextSeparator);
      thisSeparator = nextSeparator + 1;
      switch (i)
      {
      case 0:
        finger[0].fingerRead = constrain(btDataPack[i].toInt(), 0, MAXRES);
        if ((finger[0].fingerRead < 0 || finger[0].fingerRead > MAXRES)) // helps to keep a reeliable data structure
          return false;
        break;
      case 1:
        finger[1].fingerRead = constrain(btDataPack[i].toInt(), 0, MAXRES);
        if ((finger[1].fingerRead < 0 || finger[1].fingerRead > MAXRES)) // helps to keep a reeliable data structure
          return false;
        break;
      case 2:
        finger[2].fingerRead = constrain(btDataPack[i].toInt(), 0, MAXRES);
        if ((finger[2].fingerRead < 0 || finger[2].fingerRead > MAXRES)) // helps to keep a reeliable data structure
          return false;
        break;
      case 3:
        finger[3].fingerRead = constrain(btDataPack[i].toInt(), 0, MAXRES);
        if ((finger[3].fingerRead < 0 || finger[3].fingerRead > MAXRES)) // helps to keep a reeliable data structure
          return false;
        break;
      case 4:
        accel[AXL_X].raw = btDataPack[i].toFloat();
        break;
      case 5:
        accel[AXL_X].g = btDataPack[i].toFloat();
        ALPHAFILTER(lastAGsmoothed[AXL_X], accel[AXL_X].g, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 6:
        accel[AXL_X].ang = btDataPack[i].toFloat();
        ALPHAFILTER(lastAAngsmoothed[AXL_X], accel[AXL_X].ang, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 7:
        accel[AXL_Y].raw = btDataPack[i].toFloat();
        break;
      case 8:
        accel[AXL_Y].g = btDataPack[i].toFloat();
        ALPHAFILTER(lastAGsmoothed[AXL_Y], accel[AXL_Y].g, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 9:
        accel[AXL_Y].ang = btDataPack[i].toFloat();
        feedKnockCriteria(accel[AXL_Y].ang);
        ALPHAFILTER(lastAAngsmoothed[AXL_Y], accel[AXL_Y].ang, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 10:
        accel[AXL_Z].raw = btDataPack[i].toFloat();
        break;
      case 11:
        accel[AXL_Z].g = btDataPack[i].toFloat();
        ALPHAFILTER(lastAGsmoothed[AXL_Z], accel[AXL_Z].g, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 12:
        accel[AXL_Z].ang = btDataPack[i].toFloat();
        ALPHAFILTER(lastAAngsmoothed[AXL_Z], accel[AXL_Z].ang, FIXEDSMOOTHCOEFFTOKNOCK);
        break;
      case 13:
        smoothFactor = btDataPack[i].toFloat();
        break;
      }
    }
    return true;
  }
  return false;
}

// get raw value from each one token at BT pack
float BionicGlove::getRaw(uint8_t raw)
{
  if ((raw >= 0) && (raw < MAXBTDATAPACK))
    return btDataPack[raw].toFloat();
  else
    return 0;
}

// get any token from -1 to +1
float BionicGlove::getUnit(uint8_t raw)
{
  if ((raw >= 0) && (raw < MAXBTDATAPACK))
  {
    switch (raw)
    {
    case 0:
    case 1:
    case 2:
    case 3:
      return ((getF(raw) - 256.0) / 256.0);
      break;
    case 4:
      return accel[AXL_X].raw / 512.0;
      break;
    case 5:
      return accel[AXL_X].g / 2.0;
      break;
    case 6:
      return (accel[AXL_X].ang - 90.0) / 90.0;
      break;
    case 7:
      return accel[AXL_Y].raw / 512.0;
      break;
    case 8:
      return accel[AXL_Y].g / 2.0;
      break;
    case 9:
      return (accel[AXL_Y].ang - 90.0) / 90.0;
      break;
    case 10:
      return accel[AXL_Z].raw / 512.0;
      break;
    case 11:
      return accel[AXL_Z].g / 2.0;
      break;
    case 12:
      return (accel[AXL_Z].ang - 90.0) / 90.0;
      break;
    case 13:
      return smoothFactor;
      break;
    default:
      return 0;
      break;
    }
  }
  else
    return 0;
}

// return one line of already received serial data pack
String BionicGlove::getSerialData()
{
  return serialData;
}

// get smoothed accel G raw values
float BionicGlove::getAGsmoothed(uint8_t axl)
{
  return lastAGsmoothed[axl];
}

// get last smoothed accel G raw values
float BionicGlove::getLastAGsmoothed(uint8_t axl)
{
  return lastAGsmoothed[axl];
}

// get smoothed accel Angle values
float BionicGlove::getAAngsmoothed(uint8_t axl)
{
  return lastAAngsmoothed[axl];
}

// end BT communication
void BionicGlove::end()
{
  SerialBT.end();
  on = false;
}

// return if BT is active
bool BionicGlove::active()
{
  return on;
}

// get expanded finger value
uint16_t BionicGlove::getF(uint8_t f)
{
  return finger[f].fingerRead;
}

// DEPRECATED
// float BionicGlove::getFaccel(uint8_t f)
// {
//   return finger[f].accel;
// }

// void BionicGlove::logAGremoveOffset()
// {
//   for (uint8_t j = 0; j < MAXACCELCHANNELS; j++)
//   {
//     for (uint8_t i = 0; i < (MAXKNOCKLOG - 2); i++)
//     {
//       logAG[j][i] = logAG[j][i + 1];
//     }
//     logAG[j][(MAXKNOCKLOG - 2)] = GETITEM(DATA_A_X_G);
//   }
// }

//log last finger readings
void BionicGlove::logFingers()
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    for (uint8_t i = 0; i < (MAXFLICKLOG - 2); i++)
    {
      logF[f][i] = logF[f][i + 1];
    }
    logF[f][(MAXFLICKLOG - 2)] = GETITEM(f) / 100; // leave logF[f][(MAXFLICKLOG - 1)] empty !!!!
  }
}

// linear regression to the 4 items array finger readings ~ 350us
// linear regression to the 3 items array finger readings ~ 270us
// void BionicGlove::callbackFlickLr()
// {
//   if ((doneMs(ts_lastFlick, flickDebounceInterval)))
//   {
//     // uint32_t now = micros();
//     for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
//     {
//       for (uint8_t i = 0; i < MAXFLICKLINEARREGRESSIONLEARNS; i++) // learn
//         lr.learn(i, logF[f][i]);
//       lr.correlation();
//       lr.parameters(values);
//       finger[f].accel = values[0];
//       // Serial.println(finger[f].accel);
//       lr.reset();

//       if ((finger[f].accel > 0) && (abs(finger[f].accel) > flickThreshold[f][OPENED]))
//       {
//         switch (f)
//         {
//         case DATA_F_INDEX:
//           callFlickOpenedIndex();
//           break;
//         case DATA_F_MIDDLE:
//           callFlickOpenedMiddle();
//           break;
//         case DATA_F_RING:
//           callFlickOpenedRing();
//           break;
//         case DATA_F_LITTLE:
//           callFlickOpenedLittle();
//           break;
//         }
//         logFclear(f);
//         ledOnAsync();
//         ts_lastFlick = millis();
//         break;
//       }
//       else if ((finger[f].accel < 0) && (abs(finger[f].accel) > flickThreshold[f][CLOSED]))
//       {
//         switch (f)
//         {
//         case DATA_F_INDEX:
//           callFlickClosedIndex();
//           break;
//         case DATA_F_MIDDLE:
//           callFlickClosedMiddle();
//           break;
//         case DATA_F_RING:
//           callFlickClosedRing();
//           break;
//         case DATA_F_LITTLE:
//           callFlickClosedLittle();
//           break;
//         }
//         logFclear(f);
//         ledOnAsync();
//         ts_lastFlick = millis();
//         break;
//       }
//     }
//     // Serial.println(micros() - now);
//   }
// }

// calculate flick possibility based on the difference between logF[f][2] - logF[f][0] readings
void BionicGlove::callbackFlick()
{
  float diff;

  if ((doneMs(ts_lastFlick, flickDebounceInterval)))
  {
    for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    {
      diff = logF[f][2] - logF[f][0];
      if ((diff > 0) && (abs(diff) > flickThreshold[f][OPENED]))
      {
        // for (uint8_t k = 0; k < 9; k++)
        // {
        //   Serial.print(logF[f][k]);
        //   Serial.println(",");
        // }
        // Serial.println(diff);
        // Serial.println("_");

        switch (f)
        {
        case DATA_F_INDEX:
          callFlickOpenedIndex();
          break;
        case DATA_F_MIDDLE:
          callFlickOpenedMiddle();
          break;
        case DATA_F_RING:
          callFlickOpenedRing();
          break;
        case DATA_F_LITTLE:
          callFlickOpenedLittle();
          break;
        }
        logFclear(f);
        ledOnAsync();
        ts_lastFlick = millis();
        break;
      }
      else if ((diff < 0) && (abs(diff) > flickThreshold[f][CLOSED]))
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
        case DATA_F_INDEX:
          callFlickClosedIndex();
          break;
        case DATA_F_MIDDLE:
          callFlickClosedMiddle();
          break;
        case DATA_F_RING:
          callFlickClosedRing();
          break;
        case DATA_F_LITTLE:
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

//log axel Z g to calculate knock orientation
void BionicGlove::logAZGknock()
{
  knockAllowed = !knockAllowed; // slow down Slave SR to (bionic glove master SR)/2
  if (knockAllowed)
  {
    for (uint8_t i = 0; i < (MAXKNOCKLOG - 1); i++)
    {
      logAZG[i] = logAZG[i + 1];
      logAZGsmoothed[i] = logAZGsmoothed[i + 1];
    }
    logAZG[MAXKNOCKLOG - 1] = GETITEM(DATA_A_Z_G) + AZGOFFSET;
    logAZGsmoothed[MAXKNOCKLOG - 1] = lastAGsmoothed[AXL_Z];
  }
}

//calculate knock based on last 3 axle angle readings
void BionicGlove::callbackAngleKnock()
{
  float result;
  bool hit = false;

  if (doneMs(ts_lastKnock, knockDebounceInterval) && knockAllowed)
  {
    // simple angle subtraction method
    result = getKnockCriteria();
    // Serial.println(result);

    if ((logAZGsmoothed[0] > 0.6)) // vertical
    {
      if (result > knockVerticalPositiveThreshold)
      {
        callVerticalPositiveKnock();
        hit = true;
      }
      else if (-result > knockVerticalNegativeThreshold)
      {
        callVerticalNegativeKnock();
        hit = true;
      }
    }
    else // horizontal
    {
      if (result > knockHorizontalPositiveThreshold)
      {
        callHorizontalPositiveKnock();
        hit = true;
      }
      else if (-result > knockHorizontalNegativeThreshold)
      {
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

float BionicGlove::getKnockCriteria()
{
  return logKnock[3] - logKnock[0];
}

void BionicGlove::feedKnockCriteria(float item)
{
  for (uint8_t i = 0; i < 3; i++)
    logKnock[i] = logKnock[i + 1];
  logKnock[3] = item;
}
// D E P R E C A T E D !!!!!!!!!!!!!!!!!!!!!!!!!
// void BionicGlove::callbackSimpleKnock()
// {
//   float result;
//   bool hit = false;

//   if (doneMs(ts_lastKnock, knockDebounceInterval) && knockAllowed)
//   {
//     // simple subtraction method

//     result = getKnockCriteria();
//     // Serial.println(result);

//     if ((logAZGsmoothed[0] > 0.6)) // vertical
//     {
//       if (result > knockVerticalPositiveThreshold)
//       {
//         callVerticalPositiveKnock();
//         hit = true;
//       }
//       else if (-result > knockVerticalNegativeThreshold)
//       {
//         callVerticalNegativeKnock();
//         hit = true;
//       }
//     }
//     else // horizontal
//     {
//       if (result > knockHorizontalPositiveThreshold)
//       {
//         callHorizontalPositiveKnock();
//         hit = true;
//       }
//       else if (-result > knockHorizontalNegativeThreshold)
//       {
//         callHorizontalNegativeKnock();
//         hit = true;
//       }
//     }

//     if (hit)
//     {
//       ts_lastKnock = millis();
//       lastKnockAZG = logAZG[MAXKNOCKLOG - 1];
//       // logAZGclear(); //do not clear - as soon as the array becomes filled again, theLR will hit another false knock
//       ledOnAsync();
//       hit = false;
//     }
//   }
// }

// D E P R E C A T E D !!!!!!!!!!!!!!!!!!!!!!!!!
// void BionicGlove::callbackKnockLr()
// {
//   float rlResult;
//   bool hit = false;

//   if (doneMs(ts_lastKnock, knockDebounceInterval) && knockAllowed)
//   {
//     // linear regression ----------------------------------------------
//     for (uint8_t i = 0; i < MAXKNOCKLINEARREGRESSIONLEARNS; i++) // learn
//       lr.learn(i, logAZG[i] * 100);
//     lr.correlation();
//     lr.parameters(values);
//     rlResult = values[0];
//     lr.reset();
//     // end linear regression ----------------------------------------------

//     // // rlResult = getRaw(DATA_A_X_G) - lastAGsmoothed[AXL_X];
//     // // Serial.print(logAZGsmoothed[0]);
//     // // Serial.print(",");
//     // // Serial.println(rlResult);
//     if ((logAZGsmoothed[0] > 0.6)) // vertical
//     {
//       if (rlResult > knockVerticalPositiveThreshold)
//       {
//         callVerticalPositiveKnock();
//         hit = true;
//       }
//       else if (-rlResult > knockVerticalNegativeThreshold)
//       {
//         callVerticalNegativeKnock();
//         hit = true;
//       }
//     }
//     else // horizontal
//     {
//       if (rlResult > knockHorizontalPositiveThreshold)
//       {
//         callHorizontalPositiveKnock();
//         hit = true;
//       }
//       else if (-rlResult > knockHorizontalNegativeThreshold)
//       {
//         callHorizontalNegativeKnock();
//         hit = true;
//       }
//     }

//     if (hit)
//     {
//       ts_lastKnock = millis();
//       lastKnockAZG = logAZG[MAXKNOCKLOG - 1];
//       // logAZGclear(); //do not clear - as soon as the array becomes filled again, theLR will hit another false knock
//       ledOnAsync();
//       hit = false;
//     }
//   }
// }

void BionicGlove::updateNewLimits()
{
  int16_t croped;
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
  {
    updateClosedThreshold(f);
    updateOpenedThreshold(f);
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
    if (!finger[f].closedFingerStatus && (finger[f].fingerRead < finger[f].closedThresholdIn))
    {
      finger[f].closedFingerStatus = true;
      switch (f)
      {
      case DATA_F_INDEX:
        callClosedIndex();
        break;
      case DATA_F_MIDDLE:
        callClosedMiddle();
        break;
      case DATA_F_RING:
        callClosedRing();
        break;
      case DATA_F_LITTLE:
        callClosedLittle();
        break;
      }
      ledOnAsync();
    }
    else if (finger[f].closedFingerStatus && (finger[f].fingerRead > finger[f].closedThresholdOut))
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
    if (!finger[f].openedFingerStatus && (finger[f].fingerRead > finger[f].openedThresholdIn))
    {
      finger[f].openedFingerStatus = true;
      switch (f)
      {
      case DATA_F_INDEX:
        callOpenedIndex();
        break;
      case DATA_F_MIDDLE:
        callOpenedMiddle();
        break;
      case DATA_F_RING:
        callOpenedRing();
        break;
      case DATA_F_LITTLE:
        callOpenedLittle();
        break;
      }
      ledOnAsync();
    }
    else if (finger[f].openedFingerStatus && (finger[f].fingerRead < finger[f].openedThresholdOut))
    {
      finger[f].openedFingerStatus = false;
    }
  }
}

// set all closed and opened thresholds
void BionicGlove::setAllFingersThresholdPercentage(uint8_t pct)
{
  setAllClosedFingersThresholdPercentage(pct);
  setAllOpenedFingersThresholdPercentage(pct);
}

// set all closed Percentage thresholds for all fingers
void BionicGlove::setAllClosedFingersThresholdPercentage(uint8_t pct)
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    setClosedFingerThresholdPercentage(f, pct);
}
// set closed Percentage threshold for one finger
void BionicGlove::setClosedFingerThresholdPercentage(uint8_t f, uint8_t pct)
{
  finger[f].closedThresholdPercentage = constrain(pct, MINPERCENTAGE, MAXPERCENTAGE);
  updateClosedThreshold(f);
}

// set all opened Percentage thresholds for all fingers
void BionicGlove::setAllOpenedFingersThresholdPercentage(uint8_t pct)
{
  for (uint8_t f = 0; f < MAXFINGERCHANNELS; f++)
    setOpenedFingerThresholdPercentage(f, pct);
}

// set opened Percentage threshold for one finger
void BionicGlove::setOpenedFingerThresholdPercentage(uint8_t f, uint8_t pct)
{
  finger[f].openedThresholdPercentage = constrain(pct, MINPERCENTAGE, MAXPERCENTAGE);
  updateOpenedThreshold(f);
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
// update individual closed finger threshold and recalculate all limits
void BionicGlove::updateClosedThreshold(uint8_t f) // smallest values on scale
{
  finger[f].closedThresholdIn = ((MAXRES * (finger[f].closedThresholdPercentage - SCHMITTTRIGGERPERCENTAGE)) / 100);
  finger[f].closedThresholdOut = ((MAXRES * (finger[f].closedThresholdPercentage + SCHMITTTRIGGERPERCENTAGE)) / 100);
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
// update individual opened finger threshold and recalculate all limits
void BionicGlove::updateOpenedThreshold(uint8_t f)
{
  finger[f].openedThresholdIn = MAXRES - ((MAXRES * (finger[f].openedThresholdPercentage - SCHMITTTRIGGERPERCENTAGE)) / 100);
  finger[f].openedThresholdOut = MAXRES - ((MAXRES * (finger[f].openedThresholdPercentage + SCHMITTTRIGGERPERCENTAGE)) / 100);
}

// return if the finger is still inside closed area
bool BionicGlove::getFingerClosedStatus(uint8_t f)
{
  return finger[f].closedFingerStatus;
}

// return if the finger is still inside opened area
bool BionicGlove::getFingerOpenedStatus(uint8_t f)
{
  return finger[f].openedFingerStatus;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
//                                                                  ACCEL
//
//------------------------------------------------------------------------------------------------------------------------------------
// analyze if there was any axle red line
void BionicGlove::callbackAxles()
{
  for (uint8_t a = 0; a < (MAXACCELCHANNELS - 1); a++)
  {
    // top angles
    if (!accel[a].maxThresholdStatus && (accel[a].ang > accel[a].maxThresholdIn))
    {
      accel[a].maxThresholdStatus = true;
      switch (a)
      {
      case AXL_X:
        callMaxX();
        break;
      case AXL_Y:
        callMaxY();
        break;
      }
      ledOnAsync();
    }
    else if (accel[a].maxThresholdStatus && (accel[a].ang < accel[a].maxThresholdOut))
    {
      accel[a].maxThresholdStatus = false;
    }
    // lower angles
    if (!accel[a].minThresholdStatus && (accel[a].ang < accel[a].minThresholdIn))
    {
      accel[a].minThresholdStatus = true;
      switch (a)
      {
      case AXL_X:
        callMinX();
        break;
      case AXL_Y:
        callMinY();
        break;
      }
      ledOnAsync();
    }
    else if (accel[a].minThresholdStatus && (accel[a].ang > accel[a].minThresholdOut))
    {
      accel[a].minThresholdStatus = false;
    }
  }
}

// set all axels threshold angles
void BionicGlove::setAllAxleThresholdAngle(uint8_t ang)
{
  setAxleMinThresholdAngle(AXL_X, ang);
  setAxleMaxThresholdAngle(AXL_X, ang);
  setAxleMinThresholdAngle(AXL_Y, ang);
  setAxleMaxThresholdAngle(AXL_Y, ang);
}

// set axel min threshold angle
void BionicGlove::setAxleMinThresholdAngle(uint8_t axl, uint8_t ang)
{
  accel[axl].minThresholdAngle = constrain(ang, MINANGLE, MAXANGLE);
  updateAxleMinThreshold(axl);
}

// set axel max threshold angle
void BionicGlove::setAxleMaxThresholdAngle(uint8_t axl, uint8_t ang)
{
  accel[axl].maxThresholdAngle = constrain(180 - ang, MINANGLE, MAXANGLE);
  updateAxleMaxThreshold(axl);
}

/*
this is called when user sets by code new axle critical area
smallest angles
       \                /
        \              /
         \            /
          \          / _ _ _ _ out
in _ _ _ _ \       //
            \\    //
             =====
*/
void BionicGlove::updateAxleMinThreshold(uint8_t axl)
{
  accel[axl].minThresholdIn = constrain(accel[axl].minThresholdAngle - ((SCHMITTTRIGGERANGLE * 180) / 100), 1, 180);
  accel[axl].minThresholdOut = constrain(accel[axl].minThresholdAngle + ((SCHMITTTRIGGERANGLE * 180) / 100), 1, 180);
}

/*
this is called when user sets by code new axle critical area
biggest angles
             =====
in _ _ _ _  //    \\
           /       \\ _ _ _ _ out
          /          \
         /            \
        /              \
       /                \
*/
void BionicGlove::updateAxleMaxThreshold(uint8_t axl)
{
  accel[axl].maxThresholdIn = constrain(accel[axl].maxThresholdAngle + ((SCHMITTTRIGGERANGLE * 180) / 100), 1, 180);
  accel[axl].maxThresholdOut = constrain(accel[axl].maxThresholdAngle - ((SCHMITTTRIGGERANGLE * 180) / 100), 1, 180);
}

// get axel min status
bool BionicGlove::getAxleMinStatus(uint8_t axl)
{
  return accel[axl].minThresholdStatus;
}

// get axel max status
bool BionicGlove::getAxleMaxStatus(uint8_t axl)
{
  return accel[axl].maxThresholdStatus;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
//                                                                  LED MONITOR
//
//------------------------------------------------------------------------------------------------------------------------------------

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

// accel -----------------------------------------------------------
void BionicGlove::attachCallOnMinXangle(void (*onRise)())
{
  callMinX = onRise;
}
void BionicGlove::detachCallOnMinXangle()
{
  attachCallOnMinXangle(isrDefaultUnused);
}

void BionicGlove::attachCallOnMaxXangle(void (*onRise)())
{
  callMaxX = onRise;
}
void BionicGlove::detachCallOnMaxXangle()
{
  attachCallOnMaxXangle(isrDefaultUnused);
}

void BionicGlove::attachCallOnMinYangle(void (*onRise)())
{
  callMinY = onRise;
}
void BionicGlove::detachCallOnMinYangle()
{
  attachCallOnMinYangle(isrDefaultUnused);
}

void BionicGlove::attachCallOnMaxYangle(void (*onRise)())
{
  callMaxY = onRise;
}
void BionicGlove::detachCallOnMaxYangle()
{
  attachCallOnMaxYangle(isrDefaultUnused);
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

// detach all callbacks
void BionicGlove::detachAll()
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
  detachCallOnMaxXangle();
  detachCallOnMaxYangle();
  detachCallOnMinXangle();
  detachCallOnMinYangle();
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

// use built in led to visual info
void BionicGlove::setBuiltInLed(bool status)
{
  ledBuiltInActive = status;
  if (status)
    pinMode(BULTINLED, OUTPUT);
}

void BionicGlove::setAngleKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg)
{
  // discard out of range values
  if ((val_verPos > MINANGLEKNOCKTHRESHOLD) && (val_verPos < MINANGLEKNOCKTHRESHOLD))
    knockVerticalPositiveThreshold = constrain(val_verPos, MINANGLEKNOCKTHRESHOLD, MAXANGLEKNOCKTHRESHOLD);
  if ((val_verNeg > MINANGLEKNOCKTHRESHOLD) && (val_verNeg < MINANGLEKNOCKTHRESHOLD))
    knockVerticalNegativeThreshold = constrain(val_verNeg, MINANGLEKNOCKTHRESHOLD, MAXANGLEKNOCKTHRESHOLD);
  if ((val_horPos > MINANGLEKNOCKTHRESHOLD) && (val_horPos < MINANGLEKNOCKTHRESHOLD))
    knockHorizontalPositiveThreshold = constrain(val_horPos, MINANGLEKNOCKTHRESHOLD, MAXANGLEKNOCKTHRESHOLD);
  if ((val_horNeg > MINANGLEKNOCKTHRESHOLD) && (val_horNeg < MINANGLEKNOCKTHRESHOLD))
    knockHorizontalNegativeThreshold = constrain(val_horNeg, MINANGLEKNOCKTHRESHOLD, MAXANGLEKNOCKTHRESHOLD);
}

// D E P R E C A T E D !!!!!!!!!!!!!!!!!!!!!!!!!
//  void BionicGlove::setSimpleKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg)
//  {
//    // discard out of range values
//    if ((val_verPos > MINSIMPLEKNOCKTHRESHOLD) && (val_verPos < MINSIMPLEKNOCKTHRESHOLD))
//      knockVerticalPositiveThreshold = constrain(val_verPos, MINSIMPLEKNOCKTHRESHOLD, MAXSIMPLEKNOCKTHRESHOLD);
//    if ((val_verNeg > MINSIMPLEKNOCKTHRESHOLD) && (val_verNeg < MINSIMPLEKNOCKTHRESHOLD))
//      knockVerticalNegativeThreshold = constrain(val_verNeg, MINSIMPLEKNOCKTHRESHOLD, MAXSIMPLEKNOCKTHRESHOLD);
//    if ((val_horPos > MINSIMPLEKNOCKTHRESHOLD) && (val_horPos < MINSIMPLEKNOCKTHRESHOLD))
//      knockHorizontalPositiveThreshold = constrain(val_horPos, MINSIMPLEKNOCKTHRESHOLD, MAXSIMPLEKNOCKTHRESHOLD);
//    if ((val_horNeg > MINSIMPLEKNOCKTHRESHOLD) && (val_horNeg < MINSIMPLEKNOCKTHRESHOLD))
//      knockHorizontalNegativeThreshold = constrain(val_horNeg, MINSIMPLEKNOCKTHRESHOLD, MAXSIMPLEKNOCKTHRESHOLD);
//  }

// D E P R E C A T E D !!!!!!!!!!!!!!!!!!!!!!!!!
//  void BionicGlove::setLrKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg)
//  {
//    // discard out of range values
//    if ((val_verPos > MINLRKNOCKTHRESHOLD) && (val_verPos < MINLRKNOCKTHRESHOLD))
//      knockVerticalPositiveThreshold = constrain(val_verPos, MINLRKNOCKTHRESHOLD, MAXLRKNOCKTHRESHOLD);
//    if ((val_verNeg > MINLRKNOCKTHRESHOLD) && (val_verNeg < MINLRKNOCKTHRESHOLD))
//      knockVerticalNegativeThreshold = constrain(val_verNeg, MINLRKNOCKTHRESHOLD, MAXLRKNOCKTHRESHOLD);
//    if ((val_horPos > MINLRKNOCKTHRESHOLD) && (val_horPos < MINLRKNOCKTHRESHOLD))
//      knockHorizontalPositiveThreshold = constrain(val_horPos, MINLRKNOCKTHRESHOLD, MAXLRKNOCKTHRESHOLD);
//    if ((val_horNeg > MINLRKNOCKTHRESHOLD) && (val_horNeg < MINLRKNOCKTHRESHOLD))
//      knockHorizontalNegativeThreshold = constrain(val_horNeg, MINLRKNOCKTHRESHOLD, MAXLRKNOCKTHRESHOLD);
//  }

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

void BionicGlove::setKnockDebounceInterval(uint32_t val)
{
  knockDebounceInterval = val;
}

void BionicGlove::setFlickDebounceInterval(uint32_t val)
{
  flickDebounceInterval = val;
}

float BionicGlove::getAZGlastKnock()
{
  return lastKnockAZG;
}

// freeze any callback for n ms
void BionicGlove::freeze(uint32_t n)
{
  ts_frozen = millis();
  frozen = n;
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
