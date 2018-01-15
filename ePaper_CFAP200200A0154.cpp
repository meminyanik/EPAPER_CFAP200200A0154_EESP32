//=============================================================================
// "Arduino" example program for Crystalfontz ePaper. 
//
// This project is for the CFAP200200A0-0154 :
//
//   https://www.crystalfontz.com/product/cfap200200a00154
//=============================================================================
#include <SPI.h>
#include <Arduino.h>

#include "ePaper_CFAP200200A0154.h"
#include "LUTs_for_CFAP200200A00154.h"

//SPI access macros
#define ePaper_RST_0  (digitalWrite(EPD_RESET, LOW))
#define ePaper_RST_1  (digitalWrite(EPD_RESET, HIGH))
#define ePaper_CS_0   (digitalWrite(EPD_CS, LOW))
#define ePaper_CS_1   (digitalWrite(EPD_CS, HIGH))
#define ePaper_DC_0   (digitalWrite(EPD_DC, LOW))
#define ePaper_DC_1   (digitalWrite(EPD_DC, HIGH))
#define ePaper_BS_0   (digitalWrite(EPD_BUSSEL, LOW))
#define ePaper_BS_1   (digitalWrite(EPD_BUSSEL, HIGH))

//=============================================================================
void ePaper_WriteCMD(uint8_t command)
  {
  ePaper_CS_0;                   
  ePaper_DC_0;   // command write
  SPI.transfer(command);
  ePaper_CS_1;
  }
//=============================================================================
void ePaper_WriteDATA(uint8_t data)
  {
  ePaper_CS_0;                   
  ePaper_DC_1;   // data write
  SPI.transfer(data);
  ePaper_CS_1;
  }
//=============================================================================
void WriteCMD_One_Parameter(uint8_t command,uint8_t para)
  {
  while(0 != digitalRead(EPD_BUSY)); // wait 
  ePaper_CS_0;                   
  ePaper_DC_0;   // command write
  SPI.transfer(command);
  ePaper_DC_1;   // data write
  SPI.transfer(para);
  ePaper_CS_1;
  }
//=============================================================================
void WriteCMD_String(uint8_t *data, uint8_t datalen)
  {
  uint8_t
    i;

  //Select the controller   
  ePaper_CS_0;                     

  //The first byte in the string is the command
  ePaper_DC_0;
  SPI.transfer(*data++);
  //Remember that we wrote the command
  datalen--;
    
  //Remaining bytes are data
  ePaper_DC_1;
  for(i= 0;i<datalen;i++)
    {
    SPI.transfer(*data++);
    }
  //Deslect the controller   
  ePaper_CS_1;
  }
//=============================================================================
void WriteCMD_StringFlash(const uint8_t *data, uint8_t datalen)
  {
  uint8_t
    i;
  uint8_t
    index;

  index=0;

  //Select the controller   
  ePaper_CS_0;                     

  //The first byte in the string is the command
  ePaper_DC_0;
  SPI.transfer(pgm_read_byte(&data[index++]));
  //Remember that we wrote the command
  datalen--;
    
  //Remaming bytes are data
  ePaper_DC_1;
  for(i= 0;i<datalen;i++)
    {
    SPI.transfer(pgm_read_byte(&data[index++]));
    }

  //Deslect the controller   
  ePaper_CS_1;
  }
//=============================================================================
void Load_LUT_For_Full_Update(void)
  {
  WriteCMD_StringFlash(LUT_Full_Update,31);
  }
//-----------------------------------------------------------------------------
void Load_LUT_For_Partial_Update(void)
  {
  WriteCMD_StringFlash(LUT_Partial_Update,31);
  }
//=============================================================================
void Load_Flash_Image_To_Display_RAM(uint8_t  XSize,
                                     uint16_t YSize,
                                     const uint8_t  *image)
  {
  uint8_t
    x;
  uint16_t
    y;
  
  uint16_t
    index;

    index=0;

  //Convert Xsize from pixels to bytes, rounding up
  XSize=(XSize+7)>>3;

  while(0 != digitalRead(EPD_BUSY)); // wait 
  
  //Select the controller   
  ePaper_CS_0;                     

  //Write the command, 0x24
  ePaper_DC_0;
  SPI.transfer(0x24);

  //Pump the image out as data    
  ePaper_DC_1;

  for(y=0;y<YSize;y++)
    {
    for(x=0;x<XSize;x++)
      {
      SPI.transfer(pgm_read_byte(&image[index++]));
      }
    }
  //Deslect the controller   
  ePaper_CS_1;
  }
//=============================================================================
void SetDisplayArea(uint8_t  RAM_XST,uint8_t  RAM_XEND,
                    uint16_t RAM_YST,uint16_t RAM_YEND)
  {
  uint8_t
    buffer[5];
  //Set region X
  buffer[0] = 0x44; // command
  buffer[1] = RAM_XST;
  buffer[2] = RAM_XEND;
  WriteCMD_String(buffer,3);
  //Set region Y
  buffer[0] = 0x45; // command
  buffer[1] = (uint8_t)(RAM_YST&0x00FF);
  buffer[2] = (uint8_t)(RAM_YST>>8);
  buffer[3] = (uint8_t)(RAM_YEND&0x00FF);
  buffer[4] = (uint8_t)(RAM_YEND>>8);
  WriteCMD_String(buffer,5);

  //Set origin X
  buffer[0] = 0x4e; //command
  buffer[1] = RAM_XST;
  WriteCMD_String(buffer, 2);
  //Set origin Y
  buffer[0] = 0x4f;
  buffer[1] = (uint8_t)(RAM_YST&0x00FF);
  buffer[2] = (uint8_t)(RAM_YST>>8);
  WriteCMD_String(buffer, 3);
  }
//=============================================================================
void ePaper_POWERON(void)
  {
  WriteCMD_One_Parameter(0x22,0xc0);
  ePaper_WriteCMD(0x20);
  //ePaper_WriteCMD(0xff);

  while(0 != digitalRead(EPD_BUSY)); // wait 
  }
//=============================================================================
void ePaper_POWEROFF(void)
  {
  WriteCMD_One_Parameter(0x22,0xc3);
  ePaper_WriteCMD(0x20);
  }
//=============================================================================
void ePaper_Update_Full(void)
  {
  //0x22 = Display Update Control 2
  //Display Update Sequence Option
  //Executes set bits like a mini program, from MSB to LSB
  //    C    7
  // 1100 0111
  // |||| ||||-- CLK/OSC DISABLE  (0x01)
  // |||| |||--- CP DISABLE       (0x02)
  // |||| ||---- DISPLAY_PATTERN  (0x04) <<
  // |||| |----- INITIAL DISPLAY  (0x08)
  // ||||------- LOAD LUT         (0x10)
  // |||-------- LOAD TEMPERATURE (0x20)
  // ||--------- CP ENABLE        (0x40)
  // |---------- CLK/OSC ENABLE   (0x80)
  WriteCMD_One_Parameter(0x22,0xc7);
  // 0x20 = Master Activation
  // Activate Display Update Sequence
  // The Display Update Sequence Option is
  // located at R22h    
  ePaper_WriteCMD(0x20);
  //0xFF = NOP
  //This command is an empty command; it
  //does not have any effect on the display module.
  //However it can be used to terminate Frame Memory
  //Write or Read Commands.   
  ePaper_WriteCMD(0xff);
  }
//-----------------------------------------------------------------------------
 void ePaper_Update_Partial(void)
  {
  //0x22 = Display Update Control 2
  //Display Update Sequence Option
  //Executes set bits like a mini program, from MSB to LSB
  //    0    4
  // 0000 0100
  // |||| ||||-- CLK/OSC DISABLE  (0x01)
  // |||| |||--- CP DISABLE       (0x02)
  // |||| ||---- DISPLAY_PATTERN  (0x04)  <<
  // |||| |----- INITIAL DISPLAY  (0x08)
  // ||||------- LOAD LUT         (0x10)
  // |||-------- LOAD TEMPERATURE (0x20)
  // ||--------- CP ENABLE        (0x40)
  // |---------- CLK/OSC ENABLE   (0x80)
  WriteCMD_One_Parameter(0x22,0x04);
  // 0x20 = Master Activation
  // Activate Display Update Sequence
  // The Display Update Sequence Option is
  // located at R22h    
  ePaper_WriteCMD(0x20);
  //0xFF = NOP
  //This command is an empty command; it
  //does not have any effect on the display module.
  //However it can be used to terminate Frame Memory
  //Write or Read Commands.   
  ePaper_WriteCMD(0xff);
  }
//-----------------------------------------------------------------------------
const uint8_t GDOControl[]       PROGMEM = {0x01,0xC7,0x00,0x00}; //for 1.54inch
const uint8_t softstart[]        PROGMEM = {0x0c,0xd7,0xd6,0x9d};
const uint8_t VCOMVol[]          PROGMEM = {0x2c,0xa8}; // VCOM 7c
const uint8_t DummyLine[]        PROGMEM = {0x3a,0x1a}; // 4 dummy line per gate
const uint8_t Gatetime[]         PROGMEM = {0x3b,0x08}; // 2us per line
const uint8_t RamDataEntryMode[] PROGMEM = {0x11,0x01}; // Ram data entry mode
void ePaper_Init(void)
  {
  ePaper_BS_0;    // 4 wire spi mode selected

  //Reset the controller
  ePaper_RST_0;
  delay(100);
  ePaper_RST_1;
  delay(1000);
  
  // Panel configuration, Gate selection
  WriteCMD_StringFlash(GDOControl, sizeof(GDOControl));
  // X decrease, Y decrease
  WriteCMD_StringFlash(softstart, sizeof(softstart));
  // VCOM setting
  WriteCMD_StringFlash(VCOMVol, sizeof(VCOMVol));
  //dummy line per gate
  WriteCMD_StringFlash(DummyLine, sizeof(DummyLine));
  // Gate time setting
  WriteCMD_StringFlash(Gatetime, sizeof(Gatetime));
  // X decrease, Y decrease
  WriteCMD_StringFlash(RamDataEntryMode, sizeof(RamDataEntryMode));
  }
//=============================================================================
void Show_Full_Screen_Image(const uint8_t *image,
                            uint16_t xsize,
                            uint16_t ysize)
  {
  //Get xbytes from xsize, rounding up
  uint8_t
    xbytes;
  xbytes=(xsize+7)>>3;

  Load_LUT_For_Full_Update();
  ePaper_POWERON();

  SetDisplayArea(0,        // X start
                 xbytes-1, // X end
                 ysize-1,  // Y start
                 0);       // Y end

  Load_Flash_Image_To_Display_RAM(xsize, ysize, image);
  ePaper_Update_Full();  

  ePaper_POWEROFF();
  }
//=============================================================================

