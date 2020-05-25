// Gravitone Example Configuration: With Chords
// Matt Ruffner 2019
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


// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=162,625
AudioSynthKarplusStrong  string3;        //xy=170,390
AudioSynthKarplusStrong  string4;        //xy=170,451
AudioSynthKarplusStrong  string2;        //xy=177,326
AudioSynthKarplusStrong  string1;        //xy=179,261
AudioAmplifier           amp1;           //xy=339,625
AudioSynthKarplusStrong  string6;        //xy=466,514
AudioMixer4              mixer1;         //xy=480,356
AudioSynthKarplusStrong  string5;        //xy=480,449
AudioEffectFade          fade1;          //xy=504,624
AudioMixer4              mixer2;         //xy=738,378
AudioOutputI2S           i2s1;           //xy=1006,379
AudioConnection          patchCord1(sine1, amp1);
AudioConnection          patchCord2(string3, 0, mixer1, 2);
AudioConnection          patchCord3(string4, 0, mixer1, 3);
AudioConnection          patchCord4(string2, 0, mixer1, 1);
AudioConnection          patchCord5(string1, 0, mixer1, 0);
AudioConnection          patchCord6(amp1, fade1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);
AudioConnection          patchCord8(mixer1, 0, mixer2, 0);
AudioConnection          patchCord9(string5, 0, mixer2, 1);
AudioConnection          patchCord10(fade1, 0, mixer2, 3);
AudioConnection          patchCord11(mixer2, 0, i2s1, 0);
AudioConnection          patchCord12(mixer2, 0, i2s1, 1);
// GUItool: end automatically generated code


const int finger_delay = 5;
const int hand_delay = 220;

int chordnum=0;

int batCounter = 0;

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
                          
  myFSS.g = dps250;       // (ICM_20948_GYRO_CONFIG_1_FS_SEL_e)
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

    //if( myICM.dataReady() ){
      myICM.getAGMT();
      ax = myICM.accX();
      ay = myICM.accY();
      az = myICM.accZ();
      gx = myICM.gyrX();
      gy = myICM.gyrY();
      gz = myICM.gyrZ();
//      mx = myICM.magX();
//      my = myICM.magY();
//      mz = myICM.magZ();
//      
      //lastIMUUpdate = now;
    //}
    
    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // save the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
  
  if( now - lastNoteUpdate > 10 ){
    float mappedRoll;
    bool ignore = false;
    if( roll >= -180  && roll <= -90 ){
      mappedRoll = roll + 180;
    } else if( roll <= 180 && roll >= 90) {
      mappedRoll = map(roll, 180, 90, 0, -90);
    } else {
      ignore = true;
      mappedRoll = 0;
    }
    if (!ignore) {
      Serial.println(roll);
      note = map(mappedRoll, -90, 90, 0, scale->getNoteCount());
      freq = scale->getNote(note)->freq;
  
      // continuous tone
      //freq = 220*pow(2,(float)(map(roll, 90, -90, 0,24))/12.0);
  
      sine1.frequency(freq);
    } else {
      sine1.frequency(0);
    }
    lastNoteUpdate = now;
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
