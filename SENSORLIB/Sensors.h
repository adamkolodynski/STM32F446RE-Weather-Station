

#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"

//adres HTS221
#define HTS221_ADDRESS  (0x5F << 1)
//adresy rejestrów HTS221
#define HTS221_WHO_AM_I_ADDR   0x0F

#define HTS221_AV_CONF   0x10
#define HTS221_CTRL_REG1_ADDR 0x20
#define HTS221_CTRL_REG2_ADDR 0x21
#define HTS221_CTRL_REG3_ADDR 0x22

#define HTS221_STATUS_REG_ADDR 0x27

#define HTS221_HUMIDITY_OUT_L_ADDR 0x28
#define HTS221_HUMIDITY_OUT_H_ADDR 0x29

#define HTS221_TEMP_OUT_L_ADDR  0x2A
#define HTS221_TEMP_OUT_H_ADDR  0x2B

#define HTS221_H0_RH_X2_ADDR    0x30
#define HTS221_H1_RH_X2_ADDR   0x31

#define HTS221_T0_degC_X8_ADDR  0x32
#define HTS221_T1_degC_X8_ADDR   0x33

#define HTS221_T1_T0_MSB_X8_ADDR  0x35

#define HTS221_H0_T0_OUT_L_ADDR   0x36
#define HTS221_H0_T0_OUT_H_ADDR  0x37

#define HTS221_H1_T0_OUT_L_ADDR  0x3A
#define HTS221_H1_T0_OUT_H_ADDR   0x3B

#define HTS221_T0_OUT_L_ADDR  0x3C
#define HTS221_T0_OUT_H_ADDR   0x3D

#define HTS221_T1_OUT_L_ADDR  0x3E
#define HTS221_T1_OUT_H_ADDR  0x3F




//adress LPS22HB
#define LSB22HB_ADDRESS 0xBA
//adresy rejestrow
#define LSB22HB_INTERRUPT_CFG  0x0B //ustawiam zero
#define LSB22HB_THS_P_L 0x0D //podwojny //brak zmian

#define LSB22HB_WHO_AM_I  0x0F  //read only

#define LSB22HB_CTRL_REG1 0x10
#define LSB22HB_CTRL_REG2 0x11
#define LSB22HB_CTRL_REG3 0x12
#define LSB22HB_FIFO_CTRL 0x14
#define LSB22HB_REF_P_XL 0x15 //potrojny, nie zmieniam/odczytuje

#define LSB22HB_RPDS_L 0x18 //podwojny, , nie zmieniam/odczytuje

#define LSB22HB_RES_CONF 0x1A

//read only registers
#define LSB22HB_INT_SOURCE 0x25 //dont care
#define LSB22HB_FIFO_STATUS 0x26 //dont care
#define LSB22HB_STATUS 0x27 //dont care
#define LSB22HB_PRESS_OUT_XL 0x28 //potrojny

#define LSB22HB_TEMP_OUT_L 0x2B //podwojny
#define LSB22HB_LPFP_RES 0x33 //dont care


extern uint8_t Output[5]; //wynik pomiaru 0-temperatury,1- wilgotnosci,2 i 3 temperatury bo wynik 16 bitowy, 4-temperature v2


/*Inicjalizuje czujnik
Wpisuje do czterech rejestrów konfiguracyjnych odpowiednie dane.
Odczytuje niezmienne rejestry kalibracyjne i wpisuje do lokalnej pamieci
*/

void HTS221_init(I2C_HandleTypeDef *wzk); //wczytanie do pamieci ustawien poczatkowych i rejestrów kalibracyjnych
void HTS221_odczyt(I2C_HandleTypeDef *wzk); //pomiar z dwóch rejestrów i kalkulacje temperatury i wilgotnosci


void LPS22HB_init(I2C_HandleTypeDef *wzk);//wpisuje do rejestrów konfiguracyjnych odpowiednie dane
void LPS22HB_odczyt(I2C_HandleTypeDef *wzk); //odczyt temperatury i cisnienia, wpisanie wyniu do OUTPUT