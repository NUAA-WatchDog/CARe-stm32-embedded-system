#ifndef __HX711_H
#define __HX711_H

#include "hw_config.h"

//#define HX711_SCK PDout(5)// PB3
//#define HX711_DOUT PDin(6)// PB4

extern u32 HX711_Read(void);
extern void Eliminate_Error(void);
extern void Get_Weight(void);

extern u32 HX711_Buffer;
extern u32 Weight_Error;
extern u32 Weight_Data;
extern u8 Flag_Error;

#endif

