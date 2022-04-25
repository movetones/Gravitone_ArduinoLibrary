#include "ModeXylotar.h"

ModeXylotar::ModeXylotar()
{
  timeAccelerating = 0;
  lastAz = 0;

  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 3);

  mixer1.gain(0,0.27);
  mixer1.gain(1,0.27);
  mixer1.gain(2,0.27);
  mixer1.gain(3,0.27);
  mixer2.gain(0,0.27);
  mixer2.gain(1,0.27);
  mixer2.gain(2,0.27);

  addPatch( new AudioConnection(string1, 0, mixer1, 0));
  addPatch( new AudioConnection(string2, 0, mixer1, 1));
  addPatch( new AudioConnection(string3, 0, mixer1, 2));
  addPatch( new AudioConnection(string4, 0, mixer1, 3));
  addPatch( new AudioConnection(string5, 0, mixer2, 1));
  addPatch( new AudioConnection(string6, 0, mixer2, 2));
  addPatch( new AudioConnection(mixer1, 0, mixer2, 0));
  addPatch( new AudioConnection(mixer2, 0, amp1, 0));
}

void ModeXylotar::start()
{
  GravitoneOutputMode::start();

}

void ModeXylotar::stop()
{
  GravitoneOutputMode::stop();

}

const char * ModeXylotar::getName()
{
  return "ModeXylotar";
}

void ModeXylotar::onUpdateDisplay()
{
  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("Xylotar");

}

void ModeXylotar::onUpdateOrientation()
{
  auto acc = hardware->getAcc();
  auto R = hardware->getRot();

  auto accr = R * acc;



  Serial.print(accr(0));
  Serial.print(",");
  Serial.print(accr(1));
  Serial.print(",");
  Serial.println(accr(2));
}
