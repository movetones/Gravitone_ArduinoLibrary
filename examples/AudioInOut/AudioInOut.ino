// Gravitone Example Configuration: With Chords
// Matt Ruffner 2019
// MoveTones LLC

#include <Audio.h>
#include <Gravitone.h>
#include <CircularBuffer.h>
#include <GravitoneButtonInterface.h>
//#include <ICM_20948.h>
//#include "ICM20948.h"


#define ODR_PERIOD 10

// new icm code
#include <Icm20948.h>
#include <SensorTypes.h>
#include "Icm20948MPUFifoControl.h"
// end new icm code

#include <Adafruit_GFX.h>
#include <MadgwickAHRS.h>
#include <Adafruit_SSD1306.h>

#include "strum.h"
#include "scale.h"
#include "chords.h"
#include "battery.h"
#include "volume.h"

//#define INT_PIN ICM20948_INT

////////////////////////////
///////////////////////////
/// icm stuff to eventually go into a header
#define AK0991x_DEFAULT_I2C_ADDR  0x0C  /* The default I2C address for AK0991x Magnetometers */
#define AK0991x_SECONDARY_I2C_ADDR  0x0E  /* The secondary I2C address for AK0991x Magnetometers */

#define ICM_I2C_ADDR_REVA      0x68  /* I2C slave address for INV device on Rev A board */
#define ICM_I2C_ADDR_REVB     0x69  /* I2C slave address for INV device on Rev B board */

#define AD0_VAL   0     // The value of the last bit of the I2C address.


uint8_t I2C_Address = 0x68;

char eamessage[1024];

static const uint8_t dmp3_image[] = 
{
#include <icm20948_img.dmp3a.h>
};


inv_icm20948_t icm_device;

int rc = 0;
#define THREE_AXES 3
static int unscaled_bias[THREE_AXES * 2];

/* FSR configurations */
int32_t cfg_acc_fsr = 4; // Default = +/- 4g. Valid ranges: 2, 4, 8, 16
int32_t cfg_gyr_fsr = 2000; // Default = +/- 2000dps. Valid ranges: 250, 500, 1000, 2000

/*
* Mounting matrix configuration applied for Accel, Gyro and Mag
*/

static const float cfg_mounting_matrix[9] = {
  1.f, 0, 0,
  0, -1.f, 0,
  0, 0, 1.f
};


static uint8_t convert_to_generic_ids[INV_ICM20948_SENSOR_MAX] = {
  INV_SENSOR_TYPE_ACCELEROMETER,
  INV_SENSOR_TYPE_GYROSCOPE,
  INV_SENSOR_TYPE_RAW_ACCELEROMETER,
  INV_SENSOR_TYPE_RAW_GYROSCOPE,
  INV_SENSOR_TYPE_UNCAL_MAGNETOMETER,
  INV_SENSOR_TYPE_UNCAL_GYROSCOPE,
  INV_SENSOR_TYPE_BAC,
  INV_SENSOR_TYPE_STEP_DETECTOR,
  INV_SENSOR_TYPE_STEP_COUNTER,
  INV_SENSOR_TYPE_GAME_ROTATION_VECTOR,
  INV_SENSOR_TYPE_ROTATION_VECTOR,
  INV_SENSOR_TYPE_GEOMAG_ROTATION_VECTOR,
  INV_SENSOR_TYPE_MAGNETOMETER,
  INV_SENSOR_TYPE_SMD,
  INV_SENSOR_TYPE_PICK_UP_GESTURE,
  INV_SENSOR_TYPE_TILT_DETECTOR,
  INV_SENSOR_TYPE_GRAVITY,
  INV_SENSOR_TYPE_LINEAR_ACCELERATION,
  INV_SENSOR_TYPE_ORIENTATION,
  INV_SENSOR_TYPE_B2S
};
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////





// GRAVITONE BUTTON INTERFACE
GravitoneButtonInterface buttons(doButtonAction);

// I2C DISPLAY DRIVER
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define SERIAL_PORT Serial
#define WIRE_PORT Wire


//ICM20948 myICM;
//ICM_20948_I2C myICM;
//Madgwick filter;

Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Some vars to control or respond to interrupts
volatile bool isrFired = false;
volatile bool sensorSleep = false;
volatile bool canToggle = false;


gs_Scale *scale;
uint8_t scaleIndex = 0;
uint8_t scaleTypeIndex = 5;

unsigned long lastUpdate = 0, lastDispUpdate = 0, lastBatUpdate = 0, hapticTimeout = 0, lastButtonUpdate = 0, lastIMUUpdate = 0, lastNoteUpdate = 0;
float ax, ay, az, gx, gy, gz, mx, my, mz;
float roll, pitch, heading;
float freq = 0;
float batVoltage = 0;
volatile bool hapticTrigger = false;




// detection buffers - 1 seconds worth of data @ 100HZ ODR
CircularBuffer<float, 100> axBuf;
CircularBuffer<float, 100> ayBuf;
CircularBuffer<float, 100> azBuf;
CircularBuffer<float, 100> gxBuf;
CircularBuffer<float, 100> gyBuf;
CircularBuffer<float, 100> gzBuf;


uint8_t bat_icon_state = 0;
int sus = 0;
int note = 0;
bool playing = false;

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=493,244
AudioAmplifier           amp1;           //xy=668,247
AudioEffectFade          fade1;          //xy=842,248
AudioOutputI2S           i2s1;           //xy=1048,244
AudioConnection          patchCord11(sine1, amp1);
AudioConnection          patchCord12(amp1, fade1);
//AudioConnection          patchCord3(fade1, 0, i2s1, 0);
//AudioConnection          patchCord4(fade1, 0, i2s1, 1);
// GUItool: end automatically generated code


AudioSynthKarplusStrong  string1;
AudioSynthKarplusStrong  string2;
AudioSynthKarplusStrong  string3;
AudioSynthKarplusStrong  string4;
AudioSynthKarplusStrong  string5;
AudioSynthKarplusStrong  string6;
AudioMixer4              mixer1;
AudioMixer4              mixer2;
//AudioOutputI2S           i2s1;
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 0);
AudioConnection          patchCord6(string5, 0, mixer2, 1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);

AudioConnection          patchCord10(fade1, 0, mixer2, 3);

AudioConnection          patchCord8(mixer2, 0, i2s1, 0);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);

const int finger_delay = 5;
const int hand_delay = 220;

int chordnum=0;



int batCounter = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// new icm functions

/*
* Sleep implementation for ICM20948
*/
void inv_icm20948_sleep(int ms)
{
  delay(ms);
}

void inv_icm20948_sleep_us(int us)
{
  delayMicroseconds(us);
}



int i2c_master_write_register(uint8_t address, uint8_t reg, uint32_t len, const uint8_t *data)
{
//  if (address != 0x69)
//  {
//
//    Serial.print("Odd address:");
//    Serial.println(address);
//  }
  //Serial.print("write address ");
  //Serial.println(address);
  //Serial.print("register ");
  //Serial.println(reg);
  //Serial.print("length = ");
  //Serial.println(len);
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data, len);
  Wire.endTransmission();
  return 0;
}

int i2c_master_read_register(uint8_t address, uint8_t reg, uint32_t len, uint8_t *buff)
{
//  if (address != 0x69)
//  {
//
//    Serial.print("Odd read address:");
//    Serial.println(address);
//  }
  //Serial.print("read address ");
  //Serial.println(address);
  //Serial.print("register ");
  //Serial.println(reg);
  //Serial.print("length = ");
  //Serial.println(len);

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false); // Send repeated start

  uint32_t offset = 0;
  uint32_t num_received = Wire.requestFrom(address, len);
  //Serial.print("received = ");
  //Serial.println(num_received);
  //Serial.println(buff[0]);
  if (num_received == len)
  {
    for (uint8_t i = 0; i < len; i++)
    {
      buff[i] = Wire.read();
    }
    return 0;
  }
  else
  {
    return -1;
  }
}



//---------------------------------------------------------------------
int idd_io_hal_read_reg(void *context, uint8_t reg, uint8_t *rbuffer, uint32_t rlen)
{
  return i2c_master_read_register(I2C_Address, reg, rlen, rbuffer);
}

//---------------------------------------------------------------------

int idd_io_hal_write_reg(void *context, uint8_t reg, const uint8_t *wbuffer, uint32_t wlen)
{
  return i2c_master_write_register(I2C_Address, reg, wlen, wbuffer);
}

//---------------------------------------------------------------------
inv_bool_t interface_is_SPI(void)
{
  return false;
}



//---------------------------------------------------------------------
static void icm20948_apply_mounting_matrix(void)
{
  int ii;
  for (ii = 0; ii < INV_ICM20948_SENSOR_MAX; ii++)
  {
//    if( ii == INV_SENSOR_TYPE_ACCELEROMETER || ii == INV_SENSOR_TYPE_RAW_ACCELEROMETER )
//      inv_icm20948_set_matrix(&icm_device, acc_cfg_mounting_matrix, (inv_icm20948_sensor)ii);
//      
//    else if( ii == INV_SENSOR_TYPE_GYROSCOPE || ii == INV_SENSOR_TYPE_RAW_GYROSCOPE )
//      inv_icm20948_set_matrix(&icm_device, gyr_cfg_mounting_matrix, (inv_icm20948_sensor)ii);
//      
//    else if( ii == INV_SENSOR_TYPE_MAGNETOMETER  || ii == INV_SENSOR_TYPE_UNCAL_MAGNETOMETER )
//      inv_icm20948_set_matrix(&icm_device, mag_cfg_mounting_matrix, (inv_icm20948_sensor)ii);
//      
//    else
      inv_icm20948_set_matrix(&icm_device, cfg_mounting_matrix, (inv_icm20948_sensor)ii);
  }
}

//---------------------------------------------------------------------

static void icm20948_set_fsr(void)
{
  inv_icm20948_set_fsr(&icm_device, INV_ICM20948_SENSOR_RAW_ACCELEROMETER, (const void *)&cfg_acc_fsr);
  inv_icm20948_set_fsr(&icm_device, INV_ICM20948_SENSOR_ACCELEROMETER, (const void *)&cfg_acc_fsr);
  inv_icm20948_set_fsr(&icm_device, INV_ICM20948_SENSOR_RAW_GYROSCOPE, (const void *)&cfg_gyr_fsr);
  inv_icm20948_set_fsr(&icm_device, INV_ICM20948_SENSOR_GYROSCOPE, (const void *)&cfg_gyr_fsr);
  inv_icm20948_set_fsr(&icm_device, INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED, (const void *)&cfg_gyr_fsr);
}


//--------------------------------------------------------------------


int icm20948_sensor_setup(void)
{
  int rc;
  uint8_t i, whoami = 0xff;

  /*
  * Just get the whoami
  */
  rc = inv_icm20948_get_whoami(&icm_device, &whoami);
  Serial.print("whoami = ");
  Serial.println(whoami);

  //delay(1000);

  /* Setup accel and gyro mounting matrix and associated angle for current board */
  inv_icm20948_init_matrix(&icm_device);

  Serial.print("dmp image size = ");
  Serial.println(sizeof(dmp3_image));
  rc = inv_icm20948_initialize(&icm_device, dmp3_image, sizeof(dmp3_image));
  if (rc != 0)
  {
    rc = inv_icm20948_initialize(&icm_device, dmp3_image, sizeof(dmp3_image));
    Serial.print("init got ");
    Serial.println(rc);
    //  INV_MSG(INV_MSG_LEVEL_ERROR, "Initialization failed. Error loading DMP3...");
    if( rc != 0 ){ Serial.print("init fail, returning from setup..."); return rc; }
    else { Serial.print("Success on loading dmp image"); }
    //return rc;

  }


  /* possible compasses in chip
  *  INV_ICM20948_COMPASS_ID_NONE = 0, 
   INV_ICM20948_COMPASS_ID_AK09911,  
   INV_ICM20948_COMPASS_ID_AK09912,  /**< AKM AK09912 
   INV_ICM20948_COMPASS_ID_AK09916,  /**< AKM AK09916 
   INV_ICM20948_COMPASS_ID_AK08963,  /**< AKM AK08963 
  */

  /* Initialize auxiliary sensors */
  inv_icm20948_register_aux_compass(&icm_device, INV_ICM20948_COMPASS_ID_AK09916, AK0991x_DEFAULT_I2C_ADDR);   //AK0991x_SECONDARY_I2C_ADDR); // AK0991x_DEFAULT_I2C_ADDR);

  rc = inv_icm20948_initialize_auxiliary(&icm_device);

  if (rc != 0)
  {
    Serial.print("compass not detected got ");
    Serial.println(rc);
  }
  else
  {

    Serial.println("compass detected");
  }
  icm20948_apply_mounting_matrix();

  icm20948_set_fsr();

  /* re-initialize base state structure */
  inv_icm20948_init_structure(&icm_device);
  return 0;

  Serial.println("end of sensor_setup()");
} //sensor_setup

//---------------------------------------------------------------------

uint64_t inv_icm20948_get_time_us(void)
{
  return millis(); //InvEMDFrontEnd_getTimestampUs();
}

//---------------------------------------------------------------------

static enum inv_icm20948_sensor idd_sensortype_conversion(int sensor)
{
  switch (sensor)
  {
     case INV_SENSOR_TYPE_RAW_ACCELEROMETER:
       return INV_ICM20948_SENSOR_RAW_ACCELEROMETER;
     case INV_SENSOR_TYPE_RAW_GYROSCOPE:
       return INV_ICM20948_SENSOR_RAW_GYROSCOPE;
     case INV_SENSOR_TYPE_ACCELEROMETER:
       return INV_ICM20948_SENSOR_ACCELEROMETER;
     case INV_SENSOR_TYPE_GYROSCOPE:
       return INV_ICM20948_SENSOR_GYROSCOPE;
     case INV_SENSOR_TYPE_UNCAL_MAGNETOMETER:
       return INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED;
     case INV_SENSOR_TYPE_UNCAL_GYROSCOPE:
       return INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED;
     case INV_SENSOR_TYPE_BAC:
       return INV_ICM20948_SENSOR_ACTIVITY_CLASSIFICATON;
     case INV_SENSOR_TYPE_STEP_DETECTOR:
       return INV_ICM20948_SENSOR_STEP_DETECTOR;
     case INV_SENSOR_TYPE_STEP_COUNTER:
       return INV_ICM20948_SENSOR_STEP_COUNTER;
     case INV_SENSOR_TYPE_GAME_ROTATION_VECTOR:
       return INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR;
     case INV_SENSOR_TYPE_ROTATION_VECTOR:
       return INV_ICM20948_SENSOR_ROTATION_VECTOR;
     case INV_SENSOR_TYPE_GEOMAG_ROTATION_VECTOR:
       return INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR;
     case INV_SENSOR_TYPE_MAGNETOMETER:
       return INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD;
     case INV_SENSOR_TYPE_SMD:
       return INV_ICM20948_SENSOR_WAKEUP_SIGNIFICANT_MOTION;
     case INV_SENSOR_TYPE_PICK_UP_GESTURE:
       return INV_ICM20948_SENSOR_FLIP_PICKUP;
     case INV_SENSOR_TYPE_TILT_DETECTOR:
       return INV_ICM20948_SENSOR_WAKEUP_TILT_DETECTOR;
     case INV_SENSOR_TYPE_GRAVITY:
       return INV_ICM20948_SENSOR_GRAVITY;
     case INV_SENSOR_TYPE_LINEAR_ACCELERATION:
       return INV_ICM20948_SENSOR_LINEAR_ACCELERATION;
     case INV_SENSOR_TYPE_ORIENTATION:
       return INV_ICM20948_SENSOR_ORIENTATION;
     case INV_SENSOR_TYPE_B2S:
       return INV_ICM20948_SENSOR_B2S;
     default:
       return INV_ICM20948_SENSOR_MAX;
  }//switch
}//enum sensortyp_conversion


// helper
void configureICM() {
  struct inv_icm20948_serif icm20948_serif;
  icm20948_serif.context   = 0; /* no need */
  icm20948_serif.read_reg  = idd_io_hal_read_reg;
  icm20948_serif.write_reg = idd_io_hal_write_reg;
  icm20948_serif.max_read  = 1024 * 16; /* maximum number of bytes allowed per serial read */
  icm20948_serif.max_write = 1024 * 16; /* maximum number of bytes allowed per serial write */

  icm20948_serif.is_spi = interface_is_SPI();

  icm_device.base_state.serial_interface = SERIAL_INTERFACE_I2C;

  inv_icm20948_reset_states(&icm_device, &icm20948_serif);
  inv_icm20948_register_aux_compass(&icm_device, INV_ICM20948_COMPASS_ID_AK09916, AK0991x_DEFAULT_I2C_ADDR);

  rc = icm20948_sensor_setup();

  if (icm_device.selftest_done && !icm_device.offset_done)
  {
    // If we've run selftes and not already set the offset.
    inv_icm20948_set_offset(&icm_device, unscaled_bias);
    icm_device.offset_done = 1;
  }
  //enable sensors
//  rc = inv_icm20948_enable_sensor(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_GYROSCOPE), 1);
//  Serial.print("got "); Serial.print(rc); Serial.println(" after enabling gyro.");
//  rc = inv_icm20948_enable_sensor(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_ACCELEROMETER), 1);
//  Serial.print("got "); Serial.print(rc); Serial.println(" after enabling acccel.");
//  //rc = inv_icm20948_enable_sensor(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_GAME_ROTATION_VECTOR), 1);
//  rc = inv_icm20948_enable_sensor(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_RAW_MAGNETOMETER), 1);
//  Serial.print("got "); Serial.print(rc); Serial.println(" after enabling mag.");
  rc = inv_icm20948_enable_sensor(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_ORIENTATION), 1);
  Serial.print("got "); Serial.print(rc); Serial.println(" after enabling orientation.");

//  rc = inv_icm20948_set_sensor_period(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_GYROSCOPE), ODR_PERIOD); // 10 milliseconds (i hope)
//  Serial.print("got "); Serial.print(rc); Serial.println(" after setting odr on gyro.");
//  rc = inv_icm20948_set_sensor_period(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_ACCELEROMETER), ODR_PERIOD);
//  Serial.print("got "); Serial.print(rc); Serial.println(" after setting odr on accel.");
//  rc = inv_icm20948_set_sensor_period(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_RAW_MAGNETOMETER), ODR_PERIOD);
//  Serial.print("got "); Serial.print(rc); Serial.println(" after setting odr on mag.");
  rc = inv_icm20948_set_sensor_period(&icm_device, idd_sensortype_conversion(INV_SENSOR_TYPE_ORIENTATION), ODR_PERIOD);
  Serial.print("got "); Serial.print(rc); Serial.println(" after setting odr on orientation.");
}



static uint8_t icm20948_get_grv_accuracy(void)
{
  uint8_t accel_accuracy;
  uint8_t gyro_accuracy;

  accel_accuracy = (uint8_t)inv_icm20948_get_accel_accuracy();
  gyro_accuracy = (uint8_t)inv_icm20948_get_gyro_accuracy();
  return (min(accel_accuracy, gyro_accuracy));
}
void build_sensor_event_data(void *context, enum inv_icm20948_sensor sensortype, uint64_t timestamp, const void *data, const void *arg)
{

  float raw_bias_data[6];
  inv_sensor_event_t event;
  (void)context;
  uint8_t sensor_id = convert_to_generic_ids[sensortype];

  memset((void *)&event, 0, sizeof(event));
  event.sensor = sensor_id;
  event.timestamp = timestamp;
  switch (sensor_id)
  {
//  case INV_SENSOR_TYPE_UNCAL_GYROSCOPE:
//    memcpy(raw_bias_data, data, sizeof(raw_bias_data));
//    memcpy(event.data.gyr.vect, &raw_bias_data[0], sizeof(event.data.gyr.vect));
//    memcpy(event.data.gyr.bias, &raw_bias_data[3], sizeof(event.data.gyr.bias));
//    memcpy(&(event.data.gyr.accuracy_flag), arg, sizeof(event.data.gyr.accuracy_flag));
//    Serial.println("UNCAL gyro data");
//    break;
//  case INV_SENSOR_TYPE_UNCAL_MAGNETOMETER:
//    memcpy(raw_bias_data, data, sizeof(raw_bias_data));
//    memcpy(event.data.mag.vect, &raw_bias_data[0], sizeof(event.data.mag.vect));
//    memcpy(event.data.mag.bias, &raw_bias_data[3], sizeof(event.data.mag.bias));
//    memcpy(&(event.data.gyr.accuracy_flag), arg, sizeof(event.data.gyr.accuracy_flag));
//    Serial.println("Uncalibrated Magnetometer data");
//    break;




//  case INV_SENSOR_TYPE_GYROSCOPE:
//    memcpy(event.data.gyr.vect, data, sizeof(event.data.gyr.vect));
//    memcpy(&(event.data.gyr.accuracy_flag), arg, sizeof(event.data.gyr.accuracy_flag));
//
//    if( !gxBuf.available() ) gxBuf.pop(); 
//    if( !gyBuf.available() ) gyBuf.pop(); 
//    if( !gzBuf.available() ) gzBuf.pop(); 
//    
//    gxBuf.append(event.data.gyr.vect[0]);
//    gyBuf.append(event.data.gyr.vect[1]);
//    gzBuf.append(event.data.gyr.vect[2]);
//
//    //Serial.println("appended gyro data to buffer");
//    
//    sprintf(eamessage, "%i,%f,%f,%f,", timestamp, event.data.gyr.vect[0], event.data.gyr.vect[1], event.data.gyr.vect[2]);
//    //Serial.print(eamessage);
//
//    break;


    
//  case INV_SENSOR_TYPE_GRAVITY:
//    memcpy(event.data.acc.vect, data, sizeof(event.data.acc.vect));
//    event.data.acc.accuracy_flag = inv_icm20948_get_accel_accuracy();
//    Serial.println("Gravity data");
//    break;

  
  
  
  
//  case INV_SENSOR_TYPE_LINEAR_ACCELERATION:
//  case INV_SENSOR_TYPE_ACCELEROMETER:
//    memcpy(event.data.acc.vect, data, sizeof(event.data.acc.vect));
//    memcpy(&(event.data.acc.accuracy_flag), arg, sizeof(event.data.acc.accuracy_flag));
//    sprintf(eamessage, "%f,%f,%f", event.data.acc.vect[0], event.data.acc.vect[1], event.data.acc.vect[2]);
//    //Serial.println(eamessage);
//
//    if( !axBuf.available() ) axBuf.pop(); 
//    if( !ayBuf.available() ) ayBuf.pop(); 
//    if( !azBuf.available() ) azBuf.pop(); 
//    
//    axBuf.append(event.data.acc.vect[0]);
//    ayBuf.append(event.data.acc.vect[1]);
//    azBuf.append(event.data.acc.vect[2]);
//
//
//    //Serial.println("appended accel data to buffer");
//
//    break;





    
//  case INV_SENSOR_TYPE_MAGNETOMETER:
//    memcpy(event.data.mag.vect, data, sizeof(event.data.mag.vect));
//    memcpy(&(event.data.mag.accuracy_flag), arg, sizeof(event.data.mag.accuracy_flag));
//    sprintf(eamessage, "Mag: [%f,%f,%f]", event.data.mag.vect[0], event.data.mag.vect[1], event.data.mag.vect[2]);
//    //Serial.println(eamessage);
//
//    break;
//  case INV_SENSOR_TYPE_GEOMAG_ROTATION_VECTOR:
//  case INV_SENSOR_TYPE_ROTATION_VECTOR:
//    memcpy(&(event.data.quaternion.accuracy), arg, sizeof(event.data.quaternion.accuracy));
//    memcpy(event.data.quaternion.quat, data, sizeof(event.data.quaternion.quat));
//    Serial.println("rotation vector");
//    break;
//  case INV_SENSOR_TYPE_GAME_ROTATION_VECTOR:
//    memcpy(event.data.quaternion.quat, data, sizeof(event.data.quaternion.quat));
//    event.data.quaternion.accuracy_flag = icm20948_get_grv_accuracy();
//    sprintf(eamessage, "Quaternion: (%f,%f,%f,%f)", event.data.quaternion.quat[0], event.data.quaternion.quat[1], event.data.quaternion.quat[2], event.data.quaternion.quat[3]);
//    //Serial.println(eamessage);
//    break;
//  case INV_SENSOR_TYPE_BAC:
//    memcpy(&(event.data.bac.event), data, sizeof(event.data.bac.event));
//    Serial.println("BAC data...");
//    break;
//  case INV_SENSOR_TYPE_PICK_UP_GESTURE:
//  case INV_SENSOR_TYPE_TILT_DETECTOR:
//  case INV_SENSOR_TYPE_STEP_DETECTOR:
//  case INV_SENSOR_TYPE_SMD:
//    event.data.event = true;
//    //Serial.println("tilt/step/smd data");
//    break;
//  case INV_SENSOR_TYPE_B2S:
//    event.data.event = true;
//    memcpy(&(event.data.b2s.direction), data, sizeof(event.data.b2s.direction));
//    //Serial.println("B2s data");
//    break;
//  case INV_SENSOR_TYPE_STEP_COUNTER:
//    memcpy(&(event.data.step.count), data, sizeof(event.data.step.count));
//    Serial.println("Step counter data");
//    break;


  case INV_SENSOR_TYPE_ORIENTATION:
    //we just want to copy x,y,z from orientation data
    memcpy(&(event.data.orientation), data, 3 * sizeof(float));
    //sprintf(eamessage, "Orientation: [%f,%f,%f]", event.data.orientation.x, event.data.orientation.y, event.data.orientation.z);
    //Serial.println(eamessage); Serial.println(millis());
    heading = event.data.orientation.x;
    pitch = event.data.orientation.y;
    roll = event.data.orientation.z;
    Serial.println((unsigned long) timestamp);
    break;
//  case INV_SENSOR_TYPE_RAW_ACCELEROMETER:
//  case INV_SENSOR_TYPE_RAW_GYROSCOPE:
//    memcpy(event.data.raw3d.vect, data, sizeof(event.data.raw3d.vect));
//    Serial.println("RAW accel/gyro data");
//    break;
  default:
    return;
  }

  //sensor_event(&event, NULL);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



void strum_up(const float *chord, float velocity)
{
  if (chord[0] > 20.0) string1.noteOn(chord[0], velocity);
  //delay(finger_delay);
  if (chord[1] > 20.0) string2.noteOn(chord[1], velocity);
  //delay(finger_delay);
  if (chord[2] > 20.0) string3.noteOn(chord[2], velocity);
  //delay(finger_delay);
  if (chord[3] > 20.0) string4.noteOn(chord[3], velocity);
  //delay(finger_delay);
  if (chord[4] > 20.0) string5.noteOn(chord[4], velocity);
  //delay(finger_delay);
  if (chord[5] > 20.0) string6.noteOn(chord[5], velocity);
  //delay(finger_delay);
}

void strum_dn(const float *chord, float velocity)
{
  if (chord[5] > 20.0) string1.noteOn(chord[5], velocity);
  //delay(finger_delay);
  if (chord[4] > 20.0) string2.noteOn(chord[4], velocity);
  //delay(finger_delay);
  if (chord[3] > 20.0) string3.noteOn(chord[3], velocity);
  //delay(finger_delay);
  if (chord[2] > 20.0) string4.noteOn(chord[2], velocity);
  //delay(finger_delay);
  if (chord[1] > 20.0) string5.noteOn(chord[1], velocity);
  //delay(finger_delay);
  if (chord[0] > 20.0) string6.noteOn(chord[0], velocity);
  //delay(finger_delay);
}












void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(500);
  AudioMemory(20);

  initGravitonePins();
  ampPower(false);

  buttons.begin();
  //Serial.println("Started buttons");

  //filter.begin(102);
  //Serial.println("Started filter!");
  
  Wire.begin();
  Wire.setClock(400000);

  //attachInterrupt(ICM20948_INT, imuIntISR, RISING);

  //sine1.frequency(220);
  sine1.amplitude(0.95);

  mixer1.gain(0, 0.15);
  mixer1.gain(1, 0.15);
  mixer1.gain(2, 0.15);
  mixer1.gain(3, 0.15);
  mixer2.gain(1, 0.15);
  mixer2.gain(2, 0.15);
  mixer2.gain(3, 1.0); // mix in orig sine waves

  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[scaleIndex], 2);

  
  Serial.print("initializing display...");
  initScreen();

  
  Serial.println("configureing imu");
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE, BLACK);
  //display.drawLine(0,7,112,7,1);
  display.setCursor(0, 0);
  display.print("Gravitone Starting :)");
  display.display();
  configureICM();

  display.clearDisplay();
  display.display();
  Serial.println("Setting volume to 1");
  setVolume(1);
  fade1.fadeOut(500);
  delay(100);
  ampPower(true);
  Serial.print("Started amplifier");
}

unsigned long printedLast = 0;

void loop() {
  if ( millis() - lastButtonUpdate > 15 ) {
    // This takes care of dispatching button presses to the handler function 
    // we passed in the GravitoneButtonInterface contructor
    buttons.update();
    
    lastButtonUpdate = millis();   
  }
  unsigned long now = millis();
  if( now - lastIMUUpdate > 10 ){
    int rv = inv_icm20948_poll_sensor(&icm_device, (void *)0, build_sensor_event_data);
    lastIMUUpdate = now;
  }
  
  if( millis() - lastNoteUpdate > 10 ){
    if( pitch > 0 ){
      pitch = max(pitch, 90);
    } else {
      pitch = 360.0 + pitch;
      pitch = min(pitch, 270);
    }
    note = map(pitch, 270, 90, 0, scale->getNoteCount());
    freq = scale->getNote(note)->freq;

    // continuous tone
    //freq = 220*pow(2,(float)(map(roll, 90, -90, 0,24))/12.0);

    sine1.frequency(freq);

    lastNoteUpdate = millis();
  
//    // try to detect a strum from gyro data
//    float mse = 0.0;
//    for( int idx = 0; idx<gxBuf.size(); idx++ ){
//      mse += pow(abs(strum_gyro[idx])-abs(gxBuf[idx]), 2);
//    }
//
////    Serial.print(" got mse of ");
////    Serial.println(mse);
//
//    if( mse < 1000000 ){
//      Serial.println("got a strum!");
//      //playing = !playing;
//
//
//      
//    }
    
  }


  // NOTE DISPLAY UPDATE
  if ( millis() - lastDispUpdate > 300 ) {
    //display.clearDisplay();
    display.setTextSize(0);
    display.setTextColor(WHITE, BLACK);
    //display.drawLine(0,7,112,7,1);
    display.setCursor(0, 0);
    display.print("Gravitone");
    display.setCursor(0, 10);
    display.print("            ");
    display.setCursor(0, 10);
    //
    //display.print(gs_NoteNames[scale->getPositionFromNoteNumber(note)]); display.print("  "); display.print(freq); display.println(" Hz ");
    display.print(scale->getName()); display.print("           ");
    display.display();
    lastDispUpdate = millis();
  }

  if ( millis() - lastBatUpdate > 1000 ) {
    uint16_t rawBattery = analogRead(BATTERY_SENSE);
    batCounter = (batCounter + 1) % 2;
    if( batCounter == 0 ){
      batVoltage = ((batVoltage + rawBattery) / 2046.0) * 3.3 * 2.0;
  
      int lvl = getBatteryLevelFromVoltage(batVoltage);

      // if not charging but still on USB, show full icon instead of charging
      if ( digitalRead(BATTERY_STAT) && lvl == 5) {
        Serial.println("full battery");
        display.drawBitmap(112, 0,  drawable_16x8_battery_100, 16, 8, 1, 0);
      } else {
        display.drawBitmap(112, 0,  getDrawableForBatteryLevel(lvl), 16, 8, 1, 0);
      }
      
      display.display();
    } else {
      batVoltage += rawBattery;
    }
    lastBatUpdate = millis();

  }

}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//
// BUTTONS
//
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void doButtonAction(int id, bool val) {
  static bool playPressed = false, playLock = false;
  static bool s3pressed = false, s2pressed = false, s4pressed = false, s5pressed = false, s6pressed = false, s7pressed = false, s8pressed = false, s9pressed = false, s10pressed = false;
  static bool s11pressed = false, s12pressed = false;
  static int holdCount = 0;
  const float *chord;
  switch ( id ) {
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 2:
      if ( val && !playing  ) {
        playPressed = true;
        playing = true;
        fade1.fadeIn(20);
      } else if (!val && playing) {
        if( !playLock ){
          playPressed = false;
          playing = false;
          fade1.fadeOut(40);
        }
      }
      break;
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 1:
      if ( val && !s2pressed ) {
        cycleVolume();
        s2pressed = true;
      } else if ( !val && s2pressed ) {
        s2pressed = false;
      }
      break;

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 0:
      if ( val && !s3pressed ) {
        s3pressed = true;
      } else if ( !val && s3pressed ) {
        s3pressed = false;
        if ( holdCount < 12 ) {
          scaleIndex = (scaleIndex + 1) % 12;
          regenerateScale(scaleTypeIndex, scaleIndex);
        }
        holdCount = 0;
      } else if ( val && s3pressed ) {
        holdCount++;
        if ( holdCount == 12 ) {
          hapticTrigger = true;
          scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
          regenerateScale(scaleTypeIndex, scaleIndex);
        }
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 3:
      chord = Gmajor;
      if( val && !s4pressed ){
        s4pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s4pressed ){
        s4pressed = false;
        //strum_up(chord, 0.8);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 4:
      chord = Eminor;
      if( val && !s5pressed ){
        s5pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s5pressed ){
        s5pressed = false;
        //strum_up(chord, 0.8);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 5:
//      chord = Fmajor;
//      if( val && !s6pressed ){
//        s6pressed = true;
//        strum_dn(chord, 0.8);
//      } else if( !val && s6pressed ){
//        s6pressed = false;
//        //strum_up(chord, 0.8);
//      }

      if( playPressed && val ) {
        playLock = !playLock;
      }
      

      break;  
//////////////////////////////////////
//////////////////////////////////////
    case 6:
      chord = Amajor;
      if( val && !s7pressed ){
        s7pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s7pressed ){
        s7pressed = false;
        //strum_up(chord, 0.8);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 7:
      chord = Bminor;
      if( val && !s8pressed ){
        s8pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s8pressed ){
        s8pressed = false;
        //strum_up(chord, 0.8);
      }
      break;   
//////////////////////////////////////
//////////////////////////////////////
    case 8:
      chord = Cmajor;
      if( val && !s9pressed ){
        s9pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s9pressed ){
        s9pressed = false;
        //strum_up(chord, 0.8);
      }
      break;         
//////////////////////////////////////
//////////////////////////////////////
    case 9:
      chord = Dminor;
      if( val && !s10pressed ){
        s10pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s10pressed ){
        s10pressed = false;
        //strum_up(chord, 0.8);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 10:
      chord = Aminor;
      if( val && !s11pressed ){
        s11pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s11pressed ){
        s11pressed = false;
        //strum_up(chord, 0.8);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 11:
      chord = Dmajor;
      if( val && !s12pressed ){
        s12pressed = true;
        strum_dn(chord, 0.8);
      } else if( !val && s12pressed ){
        s12pressed = false;
        //strum_up(chord, 0.8);
      }
      break;            
  }
}

void regenerateScale(int  scaleType, int startNote) {
  if ( scale ) {
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleType], gs_Notes[startNote], 2);
  }
}

void cycleVolume() {
  cycleVolume(false);
}

void cycleVolume(bool mute) {
  static uint8_t vol = 1;
  if ( mute ) {
    setVolume(0);
  } else {
    vol = (vol + 1) % 5;
    setVolume(vol);
  }
}

void setVolume(uint8_t vol) {
  if ( vol > 4 ) {
    vol = 0;
  } else if ( vol > 0 ) {
    ampPower(true);
    switch ( vol ) {
      case 1: 
        amp1.gain(0.25);
        mixer1.gain(0, 0.15);
        mixer1.gain(1, 0.15);
        mixer1.gain(2, 0.15);
        mixer1.gain(3, 0.15);
        mixer2.gain(1, 0.15);
        mixer2.gain(2, 0.15);
        break;
      case 2: 
        amp1.gain(0.50);
        mixer1.gain(0, 0.25);
        mixer1.gain(1, 0.25);
        mixer1.gain(2, 0.25);
        mixer1.gain(3, 0.25);
        mixer2.gain(1, 0.25);
        mixer2.gain(2, 0.25);
        break;
      case 3: 
        amp1.gain(0.75);
        mixer1.gain(0, 0.35);
        mixer1.gain(1, 0.35);
        mixer1.gain(2, 0.35);
        mixer1.gain(3, 0.35);
        mixer2.gain(1, 0.35);
        mixer2.gain(2, 0.35);
        break;
      case 4: 
        amp1.gain(1.00);
        mixer1.gain(0, 0.4);
        mixer1.gain(1, 0.4);
        mixer1.gain(2, 0.4);
        mixer1.gain(3, 0.4);
        mixer2.gain(1, 0.4);
        mixer2.gain(2, 0.4); 
        break;
    }
  } else {
    ampPower(false);
  }
  display.drawBitmap(102, 0,  getDrawableForVolumeLevel(vol), 8, 8, 1, 0);
  display.display();
}

void ampPower(bool state) {
  if ( state ) {
    pinMode(MAX98357_MODE, INPUT);
  } else {
    pinMode(MAX98357_MODE, OUTPUT);
    digitalWrite(MAX98357_MODE, LOW);
  }
}


void initScreen() {
  digitalWrite(OLED_CONTROL, HIGH);
  delay(20);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

void initGravitonePins() {
  pinMode(OLED_CONTROL, OUTPUT);
  digitalWrite(OLED_CONTROL, LOW); // turn screen off
  pinMode(BATTERY_STAT, INPUT_PULLUP);
  pinMode(BATTERY_SENSE, INPUT);
}
