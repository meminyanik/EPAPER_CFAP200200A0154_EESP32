#ifndef __ePaper_CFAP200200A0154_H__
#define __ePaper_CFAP200200A0154_H__
//=============================================================================
// "Arduino" example program for Crystalfontz ePaper. 
//
// This project is for the CFAP200200A0-0154 :
//
//   https://www.crystalfontz.com/product/cfap200200a00154
//=============================================================================
// Arduino digital pins used for the e-paper display 
#define EPD_BUSSEL  12 //2
#define EPD_BUSY    15 //3
#define EPD_RESET   13 //4
#define EPD_DC      5 //5
#define EPD_CS      2 //10

void ePaper_Init(void);
void ePaper_POWERON(void);
void ePaper_POWEROFF(void);
void ePaper_Update_Full(void);
void ePaper_Update_Partial(void);
void Load_LUT_For_Full_Update(void);
void Load_LUT_For_Partial_Update(void);
void SetDisplayArea(uint8_t  RAM_XST,uint8_t  RAM_XEND,
                    uint16_t RAM_YST,uint16_t RAM_YEND);
void Load_Flash_Image_To_Display_RAM(uint8_t  XSize,
                                     uint16_t YSize,
                                     const uint8_t  *image);
void Show_Full_Screen_Image(const uint8_t *image,
                            uint16_t xsize,
                            uint16_t ysize);
//=============================================================================
#endif //  __ePaper_CFAP200200A0154_H__
