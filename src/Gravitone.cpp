#include "Gravitone.h"
//#include "util/chords.h"
#include "util/volume.h"
#include "util/battery.h"


// get other libraries instantiated
ICM_20948_I2C Gravitone::myICM = ICM_20948_I2C();
Adafruit_MCP23017 Gravitone::mcp = Adafruit_MCP23017();
Adafruit_SSD1306 Gravitone::display = Adafruit_SSD1306(-1);


GravitoneMode::~GravitoneMode() {

}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
bool Gravitone::begin()
{
  analogReadAveraging(32);

  pinMode(OLED_CONTROL, OUTPUT);
  digitalWrite(OLED_CONTROL, HIGH); // turn screen on
  pinMode(BATTERY_STAT, INPUT_PULLUP);
  pinMode(BATTERY_SENSE, INPUT);
  
  // start imu
  Serial.println("starting imu...");
  bool ok = initImu();
  //Serial.println("imu initialized");
  
  
  Serial.println("starting buttons");
  initButtons();
  //Serial.println("buttons initialized");
  
  Serial.println("Starting screen");
  initScreen();
  //Serial.println("done");
  
  if( ok ){
    display.print(" OK");
    display.display();
    
    updateBattery();
  } else {
    display.print("IMU init failed");
    display.display();
  }
  
  imuUpdateInterval = 20;
  buttonUpdateInterval = 50;
  displayUpdateInterval = 175;
  batteryUpdateInterval = 5000;
  
  return ok;
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/

void Gravitone::setMode(GravitoneMode *m) {
  mode = m;
  mode->start(display);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
GravitoneMode* Gravitone::getMode() {
  return mode;
}


/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::eventLoop() {
  unsigned long now = millis();
  
  if ( now - lastDisplayUpdate > displayUpdateInterval ){
    display.display();
    lastDisplayUpdate = now;
  }
  
  now = millis();
  if ( now - lastBatteryUpdate > batteryUpdateInterval ){
    updateBattery();
    lastBatteryUpdate = now;
  }
  
//  now = millis();
//  if ( now - lastImuUpdate > imuUpdateInterval ){
    if( updateOrientation() ) {
      mode->onUpdateOrientation(yaw, pitch, roll);
    }
//    lastImuUpdate = now;
//  }
  
  now = millis();
  if ( now - lastButtonUpdate > buttonUpdateInterval ){
    updateButtons();
    if( buttonsAvailable() ){
      int bid = lastButtonPress();
      
      if( bid == GB1 ){
        Serial.print("button 1");
      }
      
      if( bid == GB2 ){
        Serial.print("button 2");
        if( getButtonState(bid) == BUTTON_PRESSED ){
          Serial.println("b2 pressed");
          cycleVolume(VOL_DOWN);
        }
      }
      
      if( bid == GB3 ){
        Serial.print("button 3");
        if( getButtonState(bid) == BUTTON_PRESSED ){
          Serial.println("b3 pressed");
          cycleVolume(VOL_UP);
        }
      }
      
      if( bid == GB4 ){
        mode->button4(getButtonState(bid), display);
      }
      if( bid == GB5 ){
        mode->button5(getButtonState(bid), display);
      }
      if( bid == GB6 ){
        mode->button6(getButtonState(bid), display);
      }
      if( bid == GB7 ){
        mode->button7(getButtonState(bid), display);
      }
      if( bid == GB8 ){
        mode->button8(getButtonState(bid), display);
      }
      if( bid == GB9 ){
        mode->button9(getButtonState(bid), display);
      }
      if( bid == GB10 ){
        mode->button10(getButtonState(bid), display);
      }
      if( bid == GB11 ){
        mode->button11(getButtonState(bid), display);
      }
      if( bid == GB12 ){
        mode->button12(getButtonState(bid), display);
      }
      
      lastButtonUpdate = now;
    }
  }
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::updateBattery() {
  uint16_t rawBattery = analogRead(BATTERY_SENSE);
  float batVoltage = (rawBattery / 1023.0) * 3.3 * 2.0;

  int lvl = getBatteryLevelFromVoltage(batVoltage);

  // if not charging but still on USB, show full icon instead of charging
  if ( digitalRead(BATTERY_STAT) && lvl == 5) {
    display.drawBitmap(112, 0,  drawable_16x8_battery_100, 16, 8, 1, 0);
  } else {
    display.drawBitmap(112, 0,  getDrawableForBatteryLevel(lvl), 16, 8, 1, 0);
  }
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::drawVolume() {
  display.drawBitmap(102, 0,  getDrawableForVolumeLevel(volume), 8, 8, 1, 0);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::cycleVolume(int dir) {
  if ( dir > 0) {
    if ( volume >= 4 ) return;
    volume = (volume + 1) % 5;
  } else if (dir < 0) {
    if ( volume <= 0 ) return;
    volume = (volume - 1) % 5;
  }
  setVolume();
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::setVolume() {
  setVolume(volume);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::setVolume(int vol) {
  if ( vol > 0 ) {
    setSpeakerState(true);
    switch ( vol ) {
      case 1: 
        mode->amp1.gain(0.2);
        break;
      case 2: 
        mode->amp1.gain(0.5);
        break;
      case 3: 
        mode->amp1.gain(0.7);
        break;
      case 4: 
        mode->amp1.gain(0.9);
        break;
    }
  } else {
    setSpeakerState(false);
  }
  drawVolume();
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
bool Gravitone::initImu()
{
  bool initialized = false;
  int tries = 0;
  while ( !initialized && tries < 10) {

    myICM.begin( Wire, 0 ); // i2c coms, address bit=0

    delay(50);
    if ( myICM.status != ICM_20948_Stat_Ok ) {
      delay(100); 
      tries++;  
      continue;
    } else {
      initialized = true;
    }

    // make sure we are not sleeping
    imuWake();
  }
  // The ICM-20948 is awake and ready but hasn't been configured. Let's step through the configuration
  // sequence from InvenSense's _confidential_ Application Note "Programming Sequence for DMP Hardware Functions".

  bool success = true; // Use success to show if the configuration was successful

  // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
  success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);

  // DMP sensor options are defined in ICM_20948_DMP.h
  //    INV_ICM20948_SENSOR_ACCELEROMETER               (16-bit accel)
  //    INV_ICM20948_SENSOR_GYROSCOPE                   (16-bit gyro + 32-bit calibrated gyro)
  //    INV_ICM20948_SENSOR_RAW_ACCELEROMETER           (16-bit accel)
  //    INV_ICM20948_SENSOR_RAW_GYROSCOPE               (16-bit gyro + 32-bit calibrated gyro)
  //    INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED (16-bit compass)
  //    INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED      (16-bit gyro)
  //    INV_ICM20948_SENSOR_STEP_DETECTOR               (Pedometer Step Detector)
  //    INV_ICM20948_SENSOR_STEP_COUNTER                (Pedometer Step Detector)
  //    INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR        (32-bit 6-axis quaternion)
  //    INV_ICM20948_SENSOR_ROTATION_VECTOR             (32-bit 9-axis quaternion + heading accuracy)
  //    INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR (32-bit Geomag RV + heading accuracy)
  //    INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD           (32-bit calibrated compass)
  //    INV_ICM20948_SENSOR_GRAVITY                     (32-bit 6-axis quaternion)
  //    INV_ICM20948_SENSOR_LINEAR_ACCELERATION         (16-bit accel + 32-bit 6-axis quaternion)
  //    INV_ICM20948_SENSOR_ORIENTATION                 (32-bit 9-axis quaternion + heading accuracy)

  // Enable the DMP orientation sensor
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);
  //success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) == ICM_20948_Stat_Ok);

  // Enable any additional sensors / features
  //success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE) == ICM_20948_Stat_Ok);
  //success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);
  //success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED) == ICM_20948_Stat_Ok);


  // Configuring DMP to output data at multiple ODRs:
  // DMP is capable of outputting multiple sensor data at different rates to FIFO.
  // Setting value can be calculated as follows:
  // Value = (DMP running rate / ODR ) - 1
  // E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat9, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Accel, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Gyro, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Gyro_Calibr, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  //success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 0) == ICM_20948_Stat_Ok); // Set to the maximum


    // Configure the B2S Mounting Matrix
  const float m0[3] = {.998, 0.054, 0.0};
  const float m1[3] = {-0.054, .998, 0.0};
  const float m2[3] = {0.0, 0.0, 1.0};
  uint8_t mmerr = 0;
  uint8_t result = 0, worstResult = 0;
  const unsigned char b2sMountMultiplierZero[4] = {0x00, 0x00, 0x00, 0x00};
  const unsigned char b2sMountMultiplierPlus[4] = {0x40, 0x00, 0x00, 0x00}; // Value taken from InvenSense Nucleo example
  const unsigned char b2sMountMultiplierMinus[4] = {0xF6, 0x66, 0x66, 0x67};
  result = myICM.writeDMPmems(B2S_MTX_00, 4, &b2sMountMultiplierPlus[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_01, 4, &b2sMountMultiplierMinus[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_02, 4, &b2sMountMultiplierMinus[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_10, 4, &b2sMountMultiplierZero[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_11, 4, &b2sMountMultiplierPlus[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_12, 4, &b2sMountMultiplierMinus[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_20, 4, &b2sMountMultiplierZero[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_21, 4, &b2sMountMultiplierZero[0]); if (result > worstResult) worstResult = result;
  result = myICM.writeDMPmems(B2S_MTX_22, 4, &b2sMountMultiplierPlus[0]); if (result > worstResult) worstResult = result;
  /*mmerr = myICM.writeDMPmems(B2S_MTX_00, 4, (const unsigned char *)&m0[0]); 
  mmerr = myICM.writeDMPmems(B2S_MTX_01, 4, (const unsigned char *)&m0[1]);
  mmerr = myICM.writeDMPmems(B2S_MTX_02, 4, (const unsigned char *)&m0[2]);
  mmerr = myICM.writeDMPmems(B2S_MTX_10, 4, (const unsigned char *)&m1[0]);
  mmerr = myICM.writeDMPmems(B2S_MTX_11, 4, (const unsigned char *)&m1[1]);
  mmerr = myICM.writeDMPmems(B2S_MTX_12, 4, (const unsigned char *)&m1[2]);
  mmerr = myICM.writeDMPmems(B2S_MTX_20, 4, (const unsigned char *)&m2[0]);
  mmerr = myICM.writeDMPmems(B2S_MTX_21, 4, (const unsigned char *)&m2[1]);
  mmerr = myICM.writeDMPmems(B2S_MTX_22, 4, (const unsigned char *)&m2[2]);*/

  SERIAL_PORT.print("worstResult: ");SERIAL_PORT.println(worstResult);


  // Enable the FIFO
  success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

  // Enable the DMP
  success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

  // Reset DMP
  success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

  // Reset FIFO
  success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

  // Check success
  if (success)
  {
#ifndef QUAT_ANIMATION
    SERIAL_PORT.println(F("DMP enabled!"));
#endif
  }
  else
  {
    SERIAL_PORT.println(F("Enable DMP failed!"));
    SERIAL_PORT.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
    while (1)
      ; // Do nothing more
  }
  return success;
}


/********************************************************************************************
 *
 *
 *******************************************************************************************/
bool Gravitone::updateOrientation()
{
  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
  {
    //SERIAL_PORT.print(F("Received data! Header: 0x")); // Print the header in HEX so we can see what data is arriving in the FIFO
    //if ( data.header < 0x1000) SERIAL_PORT.print( "0" ); // Pad the zeros
    //if ( data.header < 0x100) SERIAL_PORT.print( "0" );
    //if ( data.header < 0x10) SERIAL_PORT.print( "0" );
    //SERIAL_PORT.println( data.header, HEX );

    if ((data.header & DMP_header_bitmap_Quat6) > 0) 
    {
      q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
      q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
      q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

      // Convert the quaternions to Euler angles (roll, pitch, yaw)
      // https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

      q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

      double q2sqr = q2 * q2;

      // roll (x-axis rotation)
      double t0 = +2.0 * (q0 * q1 + q2 * q3);
      double t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
      roll = atan2(t0, t1) * 180.0 / PI;

      // pitch (y-axis rotation)
      double t2 = +2.0 * (q0 * q2 - q3 * q1);
      t2 = t2 > 1.0 ? 1.0 : t2;
      t2 = t2 < -1.0 ? -1.0 : t2;
      pitch = asin(t2) * 180.0 / PI;

      // yaw (z-axis rotation)
      double t3 = +2.0 * (q0 * q3 + q1 * q2);
      double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
      yaw = atan2(t3, t4) * 180.0 / PI;

      /*SERIAL_PORT.print(F("Roll:"));
      SERIAL_PORT.print(roll, 1);
      SERIAL_PORT.print(F(" Pitch:"));
      SERIAL_PORT.print(pitch, 1);
      SERIAL_PORT.print(F(" Yaw:"));
      SERIAL_PORT.println(yaw, 1);*/
    }

    if ((data.header & DMP_header_bitmap_Quat9) > 0) // We have asked for orientation data so we should receive Quat9
    {
      // Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
      // In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
      // The quaternion data is scaled by 2^30.

      //SERIAL_PORT.printf("Quat9 data is: Q1:%ld Q2:%ld Q3:%ld Accuracy:%d\r\n", data.Quat9.Data.Q1, data.Quat9.Data.Q2, data.Quat9.Data.Q3, data.Quat9.Data.Accuracy);

      // Scale to +/- 1
      q1 = ((double)data.Quat9.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
      q2 = ((double)data.Quat9.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
      q3 = ((double)data.Quat9.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
      q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

      // Convert the quaternions to Euler angles (roll, pitch, yaw)
      // https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

      double q2sqr = q2 * q2;

      // roll (x-axis rotation)
      double t0 = +2.0 * (q0 * q1 + q2 * q3);
      double t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
      roll = atan2(t0, t1) * 180.0 / PI;

      // pitch (y-axis rotation)
      double t2 = +2.0 * (q0 * q2 - q3 * q1);
      t2 = t2 > 1.0 ? 1.0 : t2;
      t2 = t2 < -1.0 ? -1.0 : t2;
      pitch = asin(t2) * 180.0 / PI;

      // yaw (z-axis rotation)
      double t3 = +2.0 * (q0 * q3 + q1 * q2);
      double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
      yaw = atan2(t3, t4) * 180.0 / PI;
      
      
      /*SERIAL_PORT.print(F("Roll:"));
      SERIAL_PORT.print(roll, 1);
      SERIAL_PORT.print(F(" Pitch:"));
      SERIAL_PORT.print(pitch, 1);
      SERIAL_PORT.print(F(" Yaw:"));
      SERIAL_PORT.println(yaw, 1);*/
      
      
      // Output the Quaternion data in the format expected by ZaneL's Node.js Quaternion animation tool
      SERIAL_PORT.print(F("{\"quat_w\":"));
      SERIAL_PORT.print(q0, 3);
      SERIAL_PORT.print(F(", \"quat_x\":"));
      SERIAL_PORT.print(q1, 3);
      SERIAL_PORT.print(F(", \"quat_y\":"));
      SERIAL_PORT.print(q2, 3);
      SERIAL_PORT.print(F(", \"quat_z\":"));
      SERIAL_PORT.print(q3, 3);
      SERIAL_PORT.println(F("}"));
      
      return true;
    }
  } 
  return false;
} 
 
/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::initButtons()
{
  // SET INTERRUPT PINS AS INPUTS
  pinMode(MCP23017_INTA, INPUT);
  pinMode(MCP23017_INTB, INPUT);

  mcp.begin();
  
  // GPIO DIRECTION AND PULLUP CONFIG
  for( int i=0; i<12; i++ ){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);
    mcp.setupInterruptPin(i,CHANGE); 
  }
  
  // INTA=INTB, NOT FLOATING, ACTIVE LOW
  //mcp.setupInterrupts(true, false, LOW);

  // INITIALIZE STATES
  for( int i=0; i<12; i++ )
  {
      buttonStates[i] = BUTTON_OPEN;
  }
  
  // FIRST BUTTON READ
  mcp.readGPIOAB();
  updateButtons();
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::initScreen()
{
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::updateButtons()
{
    // READ CURRENT STATE
    uint16_t i, curPinVals = mcp.readGPIOAB();
    bool iVal;

    for( i=0; i<12; i++ )
    {
        // READ ITH BUTTON VALUE AND INVERT FOR NEG LOGIC
        iVal = !((curPinVals & (1 << i)) > 0);
        
        // update button states
        if( iVal && ((buttonStates[i] == BUTTON_OPEN) || (buttonStates[i] == BUTTON_RELEASED)) ){
          buttonStatesChanged[i] = true;
          buttonStates[i] = BUTTON_PRESSED;
        }
        else if( !iVal && ((buttonStates[i] == BUTTON_PRESSED) || (buttonStates[i] == BUTTON_HELD)) ){
          buttonStatesChanged[i] = true;
          buttonStates[i] = BUTTON_RELEASED;
        }
        else {          
          buttonStatesChanged[i] = false;
          if( !iVal && (buttonStates[i] == BUTTON_RELEASED) ){
            buttonStates[i] = BUTTON_OPEN;
          }
          if( iVal && (buttonStates[i] == BUTTON_PRESSED) ){
            buttonStates[i] = BUTTON_HELD;
          }
        }
        
    }
}


/********************************************************************************************
 *
 *
 *******************************************************************************************/
bool Gravitone::buttonsAvailable()
{
  for( int i=0; i<12; i++ ){
    if( buttonStatesChanged[i] ) return true;
  }
  return false;
}


/********************************************************************************************
 *
 *
 *******************************************************************************************/
int Gravitone::lastButtonPress()
{
  for( int i=0; i<12; i++ ){
    if( buttonStatesChanged[i] ) return i;
  }
  return -1;
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::setSpeakerState(bool state) {
  if ( state ) {
    pinMode(MAX98357_MODE, INPUT);
  } else {
    pinMode(MAX98357_MODE, OUTPUT);
    digitalWrite(MAX98357_MODE, LOW);
  }
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::imuSleep() {
  myICM.lowPower(true);
  myICM.sleep(true);
}

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::imuWake() {
  myICM.sleep(false);
  myICM.lowPower(false);
}

/*
*/
