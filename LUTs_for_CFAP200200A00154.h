#ifndef __LUTS_FOR_CFAP200200A00154_H__
#define __LUTS_FOR_CFAP200200A00154_H__
//=============================================================================
// "Arduino" example program for Crystalfontz ePaper. 
//
// This project is for the CFAP200200A0-0154 :
//
//   https://www.crystalfontz.com/product/cfap200200a00154
//=============================================================================
const uint8_t LUT_Full_Update[31] PROGMEM =
  {0x32, // command
   0x02, //C221 25C Full update waveform
   0x02,
   0x01,
   0x11,
   0x12,
   0x12,
   0x22,
   0x22,
   0x66,
   0x69,
   0x69,
   0x59,
   0x58,
   0x99,
   0x99,
   0x88,
   0x00,
   0x00,
   0x00,
   0x00,
   0xF8,
   0xB4,
   0x13,
   0x51,
   0x35,
   0x51,
   0x51,
   0x19,
   0x01,
   0x00};

const uint8_t LUT_Partial_Update[31] PROGMEM =
  {0x32, // command
   0x10, //C221 25C partial update waveform
   0x18,
   0x18,
   0x08,
   0x18,
   0x18,
   0x08,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x13,
   0x14,
   0x44,
   0x12,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00};
//=============================================================================
#endif //  __LUTS_FOR_CFAP200200A00154_H__
