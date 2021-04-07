// Gravitone Example Configuration: Post-Rotation Theremin with Aeronautical-like Axes
// Matt Ruffner 2019, Rev by Chad Parrish Dec 2020
// MoveTones LLC


#include <Audio.h>
#include <Gravitone.h>
#include <ICM_20948.h>
#include <Adafruit_GFX.h>
#include <MadgwickAHRS.h>
#include <Adafruit_SSD1306.h>

#define AD0_VAL 0

// GRAVITONE BUTTON INTERFACE
GravitoneButtonInterface buttons(doButtonAction);

#define SERIAL_PORT Serial
#define WIRE_PORT Wire

ICM_20948_I2C myICM;
Madgwick filter;

Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// pointer to various scales, keys and types
gs_Scale *scale;
uint8_t scaleIndex = 0;
uint8_t scaleTypeIndex = 5;

unsigned long lastUpdate = 0, lastDispUpdate = 0, lastBatUpdate = 0, lastButtonUpdate = 0, lastIMUUpdate = 0, lastNoteUpdate = 0;
float ax, ay, az, gx, gy, gz, mx, my, mz;
float roll, pitch, heading;
float freq = 0;
float batVoltage = 0;

uint8_t bat_icon_state = 0;
int sus = 0;
int note = 0;
bool playing = false;

bool continuous = false;

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=140,403
AudioAmplifier           amp1;           //xy=323,403
AudioEffectFade          fade1;          //xy=492,402
AudioOutputI2S           i2s1;           //xy=793,403
AudioConnection          patchCord1(waveform1, amp1);
AudioConnection          patchCord2(amp1, fade1);
AudioConnection          patchCord3(fade1, 0, i2s1, 0);
AudioConnection          patchCord4(fade1, 0, i2s1, 1);
// GUItool: end automatically generated code

int batCounter = 0;

int activeWaveform = WAVEFORM_SINE;
int16_t arbitraryWaveform[256];



void configureIMU() {
  bool initialized = false;
  while( !initialized ){
    myICM.begin( WIRE_PORT, AD0_VAL );

    SERIAL_PORT.print( F("Initialization of the sensor returned: ") );
    SERIAL_PORT.println( myICM.statusString() );
    if( myICM.status != ICM_20948_Stat_Ok ){
      SERIAL_PORT.println( "Trying again..." );
      delay(500);
    }else{
      initialized = true;
    }
  }

  // In this advanced example we'll cover how to do a more fine-grained setup of your sensor
  SERIAL_PORT.println("Device connected!");

  // Here we are doing a SW reset to make sure the device starts in a known state
  myICM.swReset( );
  if( myICM.status != ICM_20948_Stat_Ok){
    SERIAL_PORT.print(F("Software Reset returned: "));
    SERIAL_PORT.println(myICM.statusString());
  }
  delay(250);
  
  // Now wake the sensor up
  myICM.sleep( false );
  myICM.lowPower( false );

  // The next few configuration functions accept a bit-mask of sensors for which the settings should be applied.

  // Set Gyro and Accelerometer to a particular sample mode
  // options: ICM_20948_Sample_Mode_Continuous
  //          ICM_20948_Sample_Mode_Cycled
  myICM.setSampleMode( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr | ICM_20948_Internal_Mag), ICM_20948_Sample_Mode_Continuous ); 
  if( myICM.status != ICM_20948_Stat_Ok){
    SERIAL_PORT.print(F("setSampleMode returned: "));
    SERIAL_PORT.println(myICM.statusString());
  }

  // Set full scale ranges for both acc and gyr
  ICM_20948_fss_t myFSS;  // This uses a "Full Scale Settings" structure that can contain values for all configurable sensors
  
  myFSS.a = gpm2;         // (ICM_20948_ACCEL_CONFIG_FS_SEL_e)
                          // gpm2
                          // gpm4
                          // gpm8
                          // gpm16
                          
  myFSS.g = dps1000;       // (ICM_20948_GYRO_CONFIG_1_FS_SEL_e)
                          // dps250
                          // dps500
                          // dps1000
                          // dps2000
                          
  myICM.setFullScale( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS );  
  if( myICM.status != ICM_20948_Stat_Ok){
    SERIAL_PORT.print(F("setFullScale returned: "));
    SERIAL_PORT.println(myICM.statusString());
  }

  ICM_20948_smplrt_t imu_fs;
  imu_fs.g = 24;
  imu_fs.a = 24;

  ICM_20948_Status_e fsSetStat = myICM.setSampleRate(ICM_20948_Internal_Acc, imu_fs);
  SERIAL_PORT.print(F("Set sample rate for accel returned: ")); SERIAL_PORT.println(myICM.statusString(fsSetStat));
  fsSetStat = myICM.setSampleRate(ICM_20948_Internal_Gyr, imu_fs);
  SERIAL_PORT.print(F("Set sample rate for gyr returned: ")); SERIAL_PORT.println(myICM.statusString(fsSetStat));


  SERIAL_PORT.println();
  SERIAL_PORT.println(F("Configuration complete!")); 
}
  




/////////////////////////////////////////////////////////////////////////////////////
// MAIN SETUP FUNCTION
/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(1000);
  AudioMemory(20);

  initGravitonePins();
  ampPower(false);

  buttons.begin();
  Serial.println("Started buttons");

  filter.begin(44);
  Serial.println("Started filter!");
  
  Wire.begin();
  Wire.setClock(400000);
  
// Set Default scale and waveform
  waveform1.begin(0.8, 220, WAVEFORM_SINE);
  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[scaleIndex], 2);

// Set out gravitone display
  Serial.print("initializing display...");
  initScreen();
  
  Serial.println("configuring imu");
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE, BLACK);
  //display.drawLine(0,7,112,7,1);
  display.setCursor(0, 0);
  display.print("Gravitone Starting :)");
  display.display();
  configureIMU();

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
unsigned long microsPerReading = 1000000/44, microsPrevious=0;

float tempax, tempgx;

bool recordMotion = false;
int recordMotionCounter = 0;

void loop() {
  unsigned long now = millis();
  
  if ( now - lastButtonUpdate > 15 ) {
    // This takes care of dispatching button presses to the handler function 
    // we passed in the GravitoneButtonInterface contructor
    buttons.update();
    
    lastButtonUpdate = now;
  }
  
  unsigned long microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

// Orientation parameters extracted from IMU
    myICM.getAGMT();
    ax = myICM.accX();
    ay = myICM.accY();
    az = myICM.accZ();
    gx = myICM.gyrX();
    gy = myICM.gyrY();
    gz = myICM.gyrZ();
// IMU outputs, 6-axis accelerometer and gyroscope
    // {ax,gx} = {ay,gy}; {ay,gy} = {ax,gx}; {az,gz} = -1*{az,gz};


// Rotation commands to reorient the pitch, roll, and yaw to be
// more intuitive for the gravitone shape
    tempax = ax;
    tempgx = gx;
    ax = ay;  // Swap x and y
    gx = gy;
    ay = tempax;
    gy = tempgx;
    az = -1*az; // Flip z directions
    gz = -1*gz;
    

// update the filter to compute orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

// save the heading, pitch and roll
    roll = filter.getRoll();
    
    pitch = filter.getPitch();
    
    heading = filter.getYaw();
 
    if( recordMotion  && recordMotionCounter < 256 ){
      //arbitraryWaveform[recordMotionCounter] = (int16_t)(az*(32000.0/2.0));
      arbitraryWaveform[recordMotionCounter] = (int16_t)(gy*800);
      recordMotionCounter++;
      if(recordMotionCounter == 256){
        recordMotion = false;
      }
    }

 // increment previous time, so we keep a pace for refreshing display and sound
    microsPrevious = microsPrevious + microsPerReading;
  }

// Coerce pitch angle for tone conversion to upper and lower limits
  if( now - lastNoteUpdate > 10 ){
    float mappedPitch, vollevel = .8;

    float maxpitch = 80;
    float minpitch = -80;

    bool ignore = false;

    if( pitch > maxpitch ){
      mappedPitch = maxpitch;
    }
    else if( pitch < minpitch) {
      mappedPitch = minpitch;
    }
    else if( pitch >= minpitch && pitch <= maxpitch){
      mappedPitch = pitch;
    }
    else{
      ignore = true;
      mappedPitch = pitch;
    }

 // If valid angle value, map angle to tone frequency for sound creation 
    if (!ignore) {
    // find index of note in scale, also used in display
      note = map(mappedPitch, -80, 80, 0, scale->getNoteCount());

      // Assign frequency based on mode (continuous or scale) 
      if( continuous )
      // Continuous tone mode, assign angle to frequency proportionally
      freq = scale->unison.freq*pow(2,(float)(map(mappedPitch, -80, 80, 0,24))/12.0);
      else
      // Scale tone mode, round angle to nearest frequency in selected scale
      freq = scale->getNote(note)->freq;
      
      // If arbitrary waveform chosen, drop 1 octave to reduce alising of harmonics 
      if( activeWaveform == WAVEFORM_ARBITRARY) {
        waveform1.frequency(freq/2);   // drop by an octave (lower frequency)
        waveform1.amplitude(vollevel);
      }      else  {       
        waveform1.frequency(freq);
        waveform1.amplitude(vollevel);    
    } 
    }
    // set to base frequency ( if ignore is true is set )
    else {
       waveform1.frequency(0);
    }
    lastNoteUpdate = now;
  }


  // NOTE DISPLAY UPDATE
  if ( millis() - lastDispUpdate > 300 ) {
    //display.clearDisplay();
    display.setTextSize(0);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.fillRect(0,0,90,10,BLACK);
    display.print(scale->getName());

    display.fillRect(0,22,100,10,BLACK);
    display.fillRect(0,22,(100.0/256.0)*(float)recordMotionCounter, 10, WHITE);
    //

    display.fillRect(68,12,80,10,BLACK);
    display.setCursor(68,12);
    if( continuous ){
      display.print("Continuous");
    } else {
      display.print("Scale");
    }

    display.fillRect(0,12, 50, 10, BLACK);
    display.setCursor(0,12);
    switch( activeWaveform ){
      case WAVEFORM_SINE:
        display.print("Sine");
        break;
      case WAVEFORM_SQUARE:
        display.print("Square");
        break;
      case WAVEFORM_TRIANGLE:
        display.print("Triangle");
        break;
      case WAVEFORM_SAWTOOTH:
        display.print("Sawtooth");
        break;
      case WAVEFORM_PULSE:
        display.print("Pulse");
        break;
      case WAVEFORM_ARBITRARY:
        display.print("User");
        break;
    }

    display.setCursor(100,22);
    display.fillRect(100,22,20,10,BLACK);
    display.print(note); //display.print("  "); display.print(freq); display.println(" Hz ");
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
  static bool playing = false, playLock = false;
  static bool s3pressed = false, s2pressed = false, s4pressed = false, s5pressed = false, s6pressed = false, s7pressed = false, s8pressed = false, s9pressed = false, s10pressed = false;
  static bool s11pressed = false, s12pressed = false;
  static int holdCount = 0;
  switch ( id ) {
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 2:
      if ( val && !playing  ) {
        playing = true;
        fade1.fadeIn(20);
      } else if (!val && playing) {
        if( !playLock ){
          playing = false;
          fade1.fadeOut(100);
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
          scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
          regenerateScale(scaleTypeIndex, scaleIndex);
        }
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 3:
      if( val && !s4pressed ){
        s4pressed = true;
        activeWaveform = WAVEFORM_SINE;
        waveform1.begin(activeWaveform);
      } else if( !val && s4pressed ){
        s4pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 4:
      if( val && !s5pressed ){
        s5pressed = true;
        activeWaveform = WAVEFORM_SAWTOOTH;
        waveform1.begin(activeWaveform);
      } else if( !val && s5pressed ){
        s5pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 5:
      if( val && !s6pressed ){
        s6pressed = true;
        activeWaveform = WAVEFORM_SQUARE;
        waveform1.begin(activeWaveform);
      } else if( !val && s6pressed ){
        s6pressed = false;
      }
      break;  
//////////////////////////////////////
//////////////////////////////////////
    case 6:
      if( val && !s7pressed ){
        s7pressed = true;
        activeWaveform = WAVEFORM_TRIANGLE;
        waveform1.begin(activeWaveform);
      } else if( !val && s7pressed ){
        s7pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 7:
      if( val && !s8pressed ){
        s8pressed = true;
        activeWaveform = WAVEFORM_PULSE;
        waveform1.begin(activeWaveform);
      } else if( !val && s8pressed ){
        s8pressed = false;
        //strum_up(chord, 0.8);
      }
      break;   
//////////////////////////////////////
//////////////////////////////////////
    case 8:
      if( val && !s9pressed ){
        s9pressed = true;
        activeWaveform = WAVEFORM_ARBITRARY;
        waveform1.begin(activeWaveform);
        waveform1.arbitraryWaveform(arbitraryWaveform, 172);
      } else if( !val && s9pressed ){
        s9pressed = false;
      }
      break;         
//////////////////////////////////////
//////////////////////////////////////
    case 9:
      if( val && !s10pressed ){
        s10pressed = true;
        continuous = !continuous;
      } else if( !val && s10pressed ){
        s10pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 10:
      if( val && !s11pressed ){
        s11pressed = true;
        recordMotionCounter = 0;
        memset(arbitraryWaveform, 0, sizeof(arbitraryWaveform));
        display.drawRect(0,22,100,10,WHITE);
        display.fillRect(0,23,100, 8, BLACK);
      } else if( !val && s11pressed ){
        s11pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 11:
      if( val && !s12pressed ){
        s12pressed = true;
        recordMotion = true;
      } else if( !val && s12pressed ){
        s12pressed = false;
        recordMotion = false;
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
    vol = (vol + 1) % 5;  // Cycling through volume levels with multiple button pushes
    setVolume(vol);
  }
}

//This function changes the amplifier gain setting
void setVolume(uint8_t vol) {
  if ( vol > 4 ) {
    vol = 0;  // Mute
  } else if ( vol > 0 ) {
    ampPower(true);
    // Set gain of amp based on vol
    switch ( vol ) {
      case 1: 
        amp1.gain(0.2);
        break;
      case 2: 
        amp1.gain(0.5);
        break;
      case 3: 
        amp1.gain(0.7);
        break;
      case 4: 
        amp1.gain(0.9);
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
