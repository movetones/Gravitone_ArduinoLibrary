/* Gravitone Library
 * Copyright (c) 2022, Matt Ruffner <matthew.ruffner@movetones.com>
 * MoveTones, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "GravitoneHardware.h"

// get other libraries instantiated
ICM_20948_I2C GravitoneHardware::myICM = ICM_20948_I2C();
Adafruit_MCP23017 GravitoneHardware::mcp = Adafruit_MCP23017();
Adafruit_SSD1306 GravitoneHardware::display = Adafruit_SSD1306(-1);

/**
 * @brief GravitoneHardware::begin
 * @return
 */
bool GravitoneHardware::begin()
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
  
  buttonUpdateInterval = 20;
  displayUpdateInterval = 200;
  batteryUpdateInterval = 5000;
  
  Serial1.begin(115200);
  pinMode(ESP_GPIO0, INPUT); // should be pulled high by resistor meaning enabled
  pinMode(ESP_EN, INPUT);    // should be pulled high by resistor meaning enabled
  
  Serial.println("Resetting ESP");  
  pinMode(ESP_RST, OUTPUT);
  digitalWrite(ESP_RST, LOW);
  delay(500);
  pinMode(ESP_RST, INPUT); // pulled high by external resistor
  
  
  return ok;
}

/**
 * @brief GravitoneHardware::update
 * @param newButtons
 * @param newImu
 * @param newDisplay
 */
void GravitoneHardware::update(bool &newButtons, bool &newImu, bool &newDisplay) {
  unsigned long now = millis();
  
  if ( now - lastDisplayUpdate > displayUpdateInterval ){
    display.display();
    newDisplay = true;
    lastDisplayUpdate = now;
  }
  
  now = millis();
  if ( now - lastBatteryUpdate > batteryUpdateInterval ){
    updateBattery();
    lastBatteryUpdate = now;
  }
  
  if( updateOrientation() ) {
    newImu = true;
    lastImuUpdate = now;
  }

  now = millis();
  if ( now - lastButtonUpdate > buttonUpdateInterval ){
    updateButtons();
    if( buttonsAvailable() ){
      newButtons = true;
      lastButtonUpdate = now;
    }
  }
  
  if( handleEsp ){
    while( Serial.available() ){
      Serial1.write(Serial.read());
    }
    while( Serial1.available() ){
      Serial.write(Serial1.read());
    }
  }
}

/**
 * @brief GravitoneHardware::updateBattery
 */
void GravitoneHardware::updateBattery() {
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

/**
 * @brief GravitoneHardware::initImu
 * @return
 */
bool GravitoneHardware::initImu()
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

  // Enable the DMP orientation sensor
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);

  // Enable any additional sensors / features
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);

  // Configuring DMP to output data at multiple ODRs:
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Accel, 0) == ICM_20948_Stat_Ok); // Set to the maximum
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat9, 0) == ICM_20948_Stat_Ok); // Set to the maximum

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
    SERIAL_PORT.println(F("DMP enabled!"));
  }
  else
  {
    SERIAL_PORT.println(F("Enable DMP failed!"));
    SERIAL_PORT.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
    while (1)
      ; // Do nothing more
  }
  return success && initialized;
}

/**
 * @brief GravitoneHardware::updateOrientation
 * @return
 */
bool GravitoneHardware::updateOrientation()
{
  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
  {
    if ((data.header & DMP_header_bitmap_Quat9) > 0) // We have asked for orientation data so we should receive Quat9
    {
      // Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
      // In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
      // The quaternion data is scaled by 2^30.

      // Scale to +/- 1
      q1 = ((double)data.Quat9.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
      q2 = ((double)data.Quat9.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
      q3 = ((double)data.Quat9.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
      q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

      // New method of determining yaw pitch and roll values
      //
      // Use quaternion to transform a unit vector pointing out of the front of the device 
      //
      // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Advantages_of_quaternions
      
      Geometry::Quaternion quat( q1, q2, q3, q0);
      //Geometry::Quaternion quatConj( -1.0*q1, -1.0*q2, -1.0*q3, q0);

      BLA::Matrix<3,3> rot = quat.to_rotation_matrix();
      
      // find the axes representing the rotated reference frame
      xPointer = rot * xOrigin; 
      yPointer = rot * yOrigin; 
      zPointer = rot * zOrigin; 
      
      // convert to spherical coordinates
      double pitchTheta = atan2(sqrt(yPointer(0) * yPointer(0) + yPointer(1) * yPointer(1)),  yPointer(2));
      double rollTheta  = atan2(sqrt(xPointer(0) * xPointer(0) + xPointer(1) * xPointer(1)),  xPointer(2));
      double phi = atan2(yPointer(1), yPointer(0) );

      // euler angles with no singularities!
      heading = phi * 57.2958;
      pitch   = pitchTheta * 57.2958;
      roll    = rollTheta * 57.2958;
    
      //
      // old method of computing y/p/r values
      //
      /*double q2sqr = q2 * q2;
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
      
      
      SERIAL_PORT.print(F("Roll:"));
      SERIAL_PORT.print(roll, 1);
      SERIAL_PORT.print(F(" Pitch:"));
      SERIAL_PORT.print(pitch, 1);
      SERIAL_PORT.print(F(" Yaw:"));
      SERIAL_PORT.println(heading, 1);*/
      
      
      // Output the Quaternion data in the format expected by ZaneL's Node.js Quaternion animation tool
      
      /*SERIAL_PORT.print(F("{\"quat_w\":"));
      SERIAL_PORT.print(q0, 3);
      SERIAL_PORT.print(F(", \"quat_x\":"));
      SERIAL_PORT.print(q1, 3);
      SERIAL_PORT.print(F(", \"quat_y\":"));
      SERIAL_PORT.print(q2, 3);
      SERIAL_PORT.print(F(", \"quat_z\":"));
      SERIAL_PORT.print(q3, 3);
      SERIAL_PORT.println(F("}"));
      
      SERIAL_PORT.print(F("{\"x\":"));
      SERIAL_PORT.print(pointer(0), 3);
      SERIAL_PORT.print(F(", \"y\":"));
      SERIAL_PORT.print(pointer(1), 3);
      SERIAL_PORT.print(F(", \"z\":"));
      SERIAL_PORT.print(pointer(2), 3);
      SERIAL_PORT.println(F("}"));
      
      SERIAL_PORT.print(F("{\"r\":"));
      SERIAL_PORT.print(r, 3);
      SERIAL_PORT.print(F(", \"theta\":"));
      SERIAL_PORT.print(theta, 3);
      SERIAL_PORT.print(F(", \"phi\":"));
      SERIAL_PORT.print(phi, 3);
      SERIAL_PORT.println(F("}"));*/
      
      
      
      
    }
     if ((data.header & DMP_header_bitmap_Accel) > 0) // We have asked raw accel
    {
      // get the raw acclerometer values out of the DMP buffer
      
      // Scale to +/- 1
      ax = (double)data.Raw_Accel.Data.X / 32767.0; // Convert to double. Divide by 2^30
      ay = (double)data.Raw_Accel.Data.Y / 32767.0; // Convert to double. Divide by 2^30
      az = (double)data.Raw_Accel.Data.Z / 32767.0; // Convert to double. Divide by 2^30
      
      
      /*SERIAL_PORT.print(ax);
      SERIAL_PORT.print(F(", "));
      SERIAL_PORT.print(ay, 1);
      SERIAL_PORT.print(F(", "));
      SERIAL_PORT.println(az, 1);*/
      
      
      // Output the Quaternion data in the format expected by ZaneL's Node.js Quaternion animation tool
      /*
      SERIAL_PORT.print(F("{\"quat_w\":"));
      SERIAL_PORT.print(q0, 3);
      SERIAL_PORT.print(F(", \"quat_x\":"));
      SERIAL_PORT.print(q1, 3);
      SERIAL_PORT.print(F(", \"quat_y\":"));
      SERIAL_PORT.print(q2, 3);
      SERIAL_PORT.print(F(", \"quat_z\":"));
      SERIAL_PORT.print(q3, 3);
      SERIAL_PORT.println(F("}"));
      */
      
    } 
    return true;
  } else {
    return false;
  }
} 

/**
 * @brief GravitoneHardware::initButtons
 */
void GravitoneHardware::initButtons()
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

/**
 * @brief GravitoneHardware::initScreen
 */
void GravitoneHardware::initScreen()
{
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

/**
 * @brief GravitoneHardware::updateButtons
 */
void GravitoneHardware::updateButtons()
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

/**
 * @brief GravitoneHardware::buttonsAvailable
 * @return
 */
bool GravitoneHardware::buttonsAvailable()
{
  for( int i=0; i<12; i++ ){
    if( buttonStatesChanged[i] ) return true;
  }
  return false;
}

/**
 * @brief GravitoneHardware::buttonsAvailable
 * @param i
 * @return
 */
bool GravitoneHardware::buttonsAvailable(int i)
{
  if( buttonStatesChanged[i] ) return true;
  else return false;
}

/**
 * @brief GravitoneHardware::lastButtonPress
 * @return
 */
int GravitoneHardware::lastButtonPress()
{
  for( int i=0; i<12; i++ ){
    if( buttonStatesChanged[i] ) return i;
  }
  return -1;
}

/**
 * @brief GravitoneHardware::setAmpState
 * @param state
 */
void GravitoneHardware::setAmpState(bool state) {
  if ( state ) {
    pinMode(MAX98357_MODE, INPUT);
  } else {
    pinMode(MAX98357_MODE, OUTPUT);
    digitalWrite(MAX98357_MODE, LOW);
  }
}

/**
 * @brief GravitoneHardware::imuSleep
 */
void GravitoneHardware::imuSleep() {
  myICM.lowPower(true);
  myICM.sleep(true);
}

/**
 * @brief GravitoneHardware::imuWake
 */
void GravitoneHardware::imuWake() {
  myICM.sleep(false);
  myICM.lowPower(false);
}
