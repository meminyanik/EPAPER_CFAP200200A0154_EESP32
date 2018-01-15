//=============================================================================
// "ESP32" example program for Crystalfontz ePaper. 
//
// This project is for the CFAP200200A0-0154 :
//
//   https://www.crystalfontz.com/product/cfap200200a00154
//
// It was written against a ESP32 @3.3v.
//-----------------------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>
//=============================================================================
// The display is SPI, include the library header.
#include <SPI.h>

// Include the ePaper module header.
#include "ePaper_CFAP200200A0154.h"

// Include the images. These images were prepared with "bmp_to_epaper" which
// is available on the Crystalfontz site.
#include "Images_for_CFAP200200A00154.h"
// This module is 200x200 pixels. Make sure that the image files make sense.
#if ((HEIGHT_PIXELS != (200)) || (WIDTH_PIXELS  != (200)))
#error "IMAGE INCLUDE FILE IS WRONG SIZE"
#endif
//=============================================================================
void Partial_Update_Letters_Demo(void)
  {
  Load_LUT_For_Partial_Update();
  ePaper_POWERON();

  //Fill entire display RAM with background image.
  SetDisplayArea(0,                  // X start
                 WIDTH_MONO_BYTES-1, // X end
                 HEIGHT_PIXELS-1,    // Y start
                 0);                 // Y end
  Load_Flash_Image_To_Display_RAM(WIDTH_PIXELS,  //X size
                                  HEIGHT_PIXELS, //Y size
                                  Mono_1BPP);    //Image
  
  //Swap foreground and background, this will
  //show the full-screen image we just put up.
  ePaper_Update_Partial();

  //Now again fill entire display RAM with the same
  //background image. This will make it so the parts
  //of the dispaly that are not changed by the partial
  //update do not flicker.
  //If you instead load Diag_1BPP, you can see the
  //background swap out on every other update.
  SetDisplayArea(0,                  // X start
                 WIDTH_MONO_BYTES-1, // X end
                 HEIGHT_PIXELS-1,    // Y start
                 0);                 // Y end
  Load_Flash_Image_To_Display_RAM(WIDTH_PIXELS,  //X size
                                  HEIGHT_PIXELS, //Y size
                                  Mono_1BPP);    //Image

  // Now we can animate some smaller bitmaps to show
  // the four letters 'C' 'F' 'A' 'P' 
  uint8_t
    i;
  for(i=0;i<12;i++)
    {
    //Partial write to background
    SetDisplayArea(6,                             // X start
                   6+LETTER_WIDTH_MONO_BYTES-1,   // X end
                   126,                           // Y start 
                   126-(LETTER_HEIGHT_PIXELS-1)); // Y end
    switch(i%4)
      {
      case 0:
        Load_Flash_Image_To_Display_RAM(LETTER_WIDTH_PIXELS,
                                        LETTER_HEIGHT_PIXELS,
                                        (uint8_t *)Mono_Letter_C);
        break;
      case 1:
        Load_Flash_Image_To_Display_RAM(LETTER_WIDTH_PIXELS,
                                        LETTER_HEIGHT_PIXELS,
                                        (uint8_t *)Mono_Letter_F);
        break;
      case 2:
        Load_Flash_Image_To_Display_RAM(LETTER_WIDTH_PIXELS,
                                        LETTER_HEIGHT_PIXELS,
                                        (uint8_t *)Mono_Letter_A);
        break;
      case 3:
        Load_Flash_Image_To_Display_RAM(LETTER_WIDTH_PIXELS,
                                        LETTER_HEIGHT_PIXELS,
                                        (uint8_t *)Mono_Letter_P);
        break;
      }
    ePaper_Update_Partial();
    delay(100);
    }
  ePaper_POWEROFF();
  }
//===========================================================================
void setup(void)
  {
  //Debug port / Arduino Serial Monitor
  Serial.begin(115200);
  
  // Configure the pin directions
  pinMode(EPD_BUSY, INPUT);
  pinMode(EPD_RESET , OUTPUT);   
  pinMode(EPD_CS, OUTPUT);     
  pinMode(EPD_DC, OUTPUT);     
  pinMode(EPD_BUSSEL, OUTPUT);
  // Set output pins' default state
  digitalWrite(EPD_RESET, HIGH);   //active low
  digitalWrite(EPD_CS, HIGH);      //active low
  digitalWrite(EPD_DC, HIGH);      //active low 
  digitalWrite(EPD_BUSSEL, LOW);   // 4 wire mode
  
  //Set up SPI interface
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  //Initialize the display controller hardware 
  ePaper_Init();
  }
//=============================================================================
void loop()
  {
  // Show the logo from Images_for_CFAP200200A00154.h
  Show_Full_Screen_Image(Mono_1BPP,      //image data
                         WIDTH_PIXELS,   //X size
                         HEIGHT_PIXELS); //Y size
  delay(3000);

  //Loop through some smaller images to show off the partial update feature.
  Partial_Update_Letters_Demo();
  delay(2500);
  }
//=============================================================================
