
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
char count = 0;
bool flag_star = false, flag_start = false; // whether the string is complete
unsigned char data_RX[12],data_char[6], data_buf, mode = 0;
/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
  --------------------------------------------------------------------------------------*/
void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup(void)
{

  //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()
  Serial.begin(9600);
  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)

}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
  --------------------------------------------------------------------------------------*/
void loop(void)
{
  if (flag_star == true)
  {
    if (mode == 0) {
      flag_star = false;
      // 10 x 14 font clock, including demo of OR and NOR modes for pixels so that the flashing colon can be overlayed
      dmd.clearScreen( true );
      dmd.selectFont(Arial_Black_16);
      dmd.drawChar(  1,  1, data_RX[1], GRAPHICS_NORMAL );
      dmd.drawChar(  10,  1, data_RX[2], GRAPHICS_NORMAL );
      dmd.drawChar( 19,  1, data_RX[3], GRAPHICS_NORMAL );
      dmd.drawChar( 28,  1, data_RX[4], GRAPHICS_NORMAL );
      dmd.drawChar(  37,  1, data_RX[5], GRAPHICS_NORMAL );
      dmd.drawChar(  46,  1, data_RX[6], GRAPHICS_NORMAL );
      dmd.drawChar( 55,  1, data_RX[7], GRAPHICS_NORMAL );
      //dmd.drawChar( 56,  1, data_RX[8], GRAPHICS_NORMAL );
      //dmd.drawChar( 15,  1, data_RX[9], GRAPHICS_OR     );   // clock colon overlay on
    }
    if(mode == 1)
    {
      flag_star = false;
      // 10 x 14 font clock, including demo of OR and NOR modes for pixels so that the flashing colon can be overlayed
      dmd.clearScreen( true );
      dmd.selectFont(Arial_Black_16);
      dmd.drawChar(  2,  1, data_RX[1], GRAPHICS_NORMAL );
      dmd.drawChar(  14,  1, data_RX[2], GRAPHICS_NORMAL );
      dmd.drawChar( 26,  1, data_RX[3], GRAPHICS_NORMAL );
      dmd.drawChar( 38,  1, data_RX[4], GRAPHICS_NORMAL );
      dmd.drawChar(  50,  1, data_RX[5], GRAPHICS_NORMAL );
      //dmd.drawChar(  60,  1, data_RX[6], GRAPHICS_NORMAL );
      //dmd.drawChar( 55,  1, data_RX[7], GRAPHICS_NORMAL );
    }
  }
  


}

void serialEvent() {
  while (Serial.available()) {
    char dat_buff = (char)Serial.read();
    switch (dat_buff)
    {
      case '$':
        {
          flag_start = true;
          flag_star = false;
          mode = 0;
          count = 0;
          break;
        }
      case '#':
        {
          flag_start = true;
          flag_star = false;
          mode = 1;
          count = 0;
          break;
        }
      case '*':
        {
          flag_star = true;
          break;
        }
    }
    if (flag_start == true)
    {
      data_RX[count] = dat_buff;
      count++;
      if (count >= 15) count = 0;

    }
    //    // get the new byte:
    //    char inChar = (char)Serial.read();
    //    // add it to the inputString:
    //    inputString += inChar;
    //    // if the incoming character is a newline, set a flag so the main loop can
    //    // do something about it:
    //    if (inChar == '\n') {
    //      stringComplete = true;
  }
}
