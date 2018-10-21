#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

typedef enum{
  SO = 2,
  SCLK = 3,
  CS1 = A2,
  //LCD = 0x3F,   // real
  LCD = 0x20,   //simulacao
  TUBE = 6
} ports;

typedef enum{
    MIN_TEMP = -10,
    MAX_TEMP = 150,
} temperature;

double sensor1 = 0;        
uint8_t degree[8]  = {140, 146, 146, 140, 128, 128, 128, 128};
LiquidCrystal_I2C lcd(LCD,2,1,0,4,5,6,7,3, POSITIVE);
 
byte SPIRead(void);
double readThermocouple(int ChipSelect);
void lcd_display();
void update_tube();
