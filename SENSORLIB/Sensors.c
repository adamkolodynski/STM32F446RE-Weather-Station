#include "Sensors.h"

//zmienne lokalne=bez extern
int32_t temp; //do oblicznia temperatury HTS221
int32_t hum; //do obliczania wilgotnosci
uint32_t press; //do oblicznia cisnienia
int32_t temp2; //  do oblicznia temperatury LPS22HB

//zmienne lokalne do przeksztalcania wczytach int8_t do int16_t dla rejestrow 16 bitoweych TYLKO HTS221
uint8_t buffer[4]; //zmienne do przeksztalcania wczytach uint8_t do uint16_t dla rejestrow 16 bitoweych
uint8_t CalibReg8[3];// 0=H0_rHx2, 1=H1_rHx2 oba podzielic przez 2!  3=T1/T0 msb ;; Wszystie unsigned
uint16_t CalibReg16u[2]; //0=T0_degC, 1=T1_degC_ || T0 i T1 sa 10 bitowe unsigned, i sa wynikiem polaczenia z T1/T0msb, podzielone przez 8
int16_t CalibReg16s[6]; // 0=H_OUT, 1=T_OUT, 2=H0_T0_OUT, 3=H1_T0_OUT, 4=T0_OUT, 5-T1_OUT, 

//zmienne globalne wynikowe
uint8_t Output[5]; //wynik pomiaru 0-temperatury,1- wilgotnosci,2 i 3 temperatury bo wynik 16 bitowy, 4-temperature v2


// dane konfiguracyjne do rejestrów HTS221
uint8_t HTS221AV  = 0x1B ;
uint8_t HTS221REG1 = 0x85;
uint8_t HTS221REG2 = 0x01;
uint8_t HTS221REG3 = 0x00 ;



//dane konfiguracyjne do rejestrow konfigurujacych LPS22HB
uint8_t LSB22HB_INTERRUPT_CFGREG = 0x00; //zero na konfigracji przerwan od czujnika
uint8_t LSP22HBREG1 = 0x12; //1Hz pomiaru, BDU ON
uint8_t LSP22HBREG2 = 0x10; //enable multiple register read
uint8_t LSP22HBREG3 = 0x00; //default
uint8_t LSB22HB_FIFO_CTRLREG = 0x00; //Bypass mode
uint8_t LSB22HB_RES_CONFREG = 0x00; //low-noise mode

void HTS221_init(I2C_HandleTypeDef *wzk){

	//wpisanie konfiguracji
HAL_I2C_Mem_Write_IT(wzk, HTS221_ADDRESS, HTS221_AV_CONF, 1, &HTS221AV,1);
HAL_Delay(2);
HAL_I2C_Mem_Write_IT(wzk, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1, &HTS221REG1,1);
HAL_Delay(2);
HAL_I2C_Mem_Write_IT(wzk, HTS221_ADDRESS, HTS221_CTRL_REG2_ADDR, 1, &HTS221REG2,1);
HAL_Delay(2);
HAL_I2C_Mem_Write_IT(wzk, HTS221_ADDRESS, HTS221_CTRL_REG3_ADDR, 1, &HTS221REG3,1);
HAL_Delay(2);

	//czytanie po kolei kolejnych rejestrów kalibracyjnych
HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_H0_RH_X2_ADDR | 0x80, 1, buffer, 2);
HAL_Delay(2);
CalibReg8[0]=buffer[0]>>1	;
CalibReg8[1]=buffer[1]>>1	;

HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_T1_T0_MSB_X8_ADDR, 1, buffer, 1);
HAL_Delay(2);
CalibReg8[2]=buffer[0];

	
HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_T0_degC_X8_ADDR | 0x80, 1, buffer, 2);	
HAL_Delay(2);
CalibReg16u[0]=(((uint16_t)(CalibReg8[2] & 0x03)<<8)|(uint16_t)buffer[0])>>3;
CalibReg16u[1]=(((uint16_t)(CalibReg8[2] & 0x0C)<<6)|(uint16_t)buffer[1])>>3;

HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_H0_T0_OUT_L_ADDR | 0x80, 1, buffer, 2);	
HAL_Delay(2);
CalibReg16s[2]=(int16_t)buffer[1]<<8 | (int16_t)buffer[0];

HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_H1_T0_OUT_L_ADDR | 0x80, 1, buffer, 2);	
HAL_Delay(2);
CalibReg16s[3]=(int16_t)buffer[1]<<8 | (int16_t)buffer[0];

HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_T0_OUT_L_ADDR | 0x80, 1, buffer, 2);	
HAL_Delay(2);
CalibReg16s[4]=(int16_t)buffer[1]<<8 | (int16_t)buffer[0];

HAL_I2C_Mem_Read_IT(wzk, HTS221_ADDRESS, HTS221_T1_OUT_L_ADDR | 0x80, 1, buffer, 2);	
HAL_Delay(2);
CalibReg16s[5]=(int16_t)buffer[1]<<8 | (int16_t)buffer[0];

};


void HTS221_odczyt(I2C_HandleTypeDef *wzk){
//oczdczy dwóch rejestrów(4 po 8 bitów) wyniku pomiaru temperatury i wilgotnosci
HAL_I2C_Mem_Read(wzk, HTS221_ADDRESS, HTS221_HUMIDITY_OUT_L_ADDR | 0x80, 1, buffer, 4,1000);
//polacznie w 16 bitowe wyniki
CalibReg16s[0]=(int16_t)buffer[1]<<8 | (int16_t)buffer[0];
CalibReg16s[1]=(int16_t)buffer[3]<<8 | (int16_t)buffer[2];	

//liniowa interpolacja 	
temp=(((int32_t)(CalibReg16u[1]-CalibReg16u[0]))*((int32_t)(CalibReg16s[1]-CalibReg16s[4]))/((int32_t)(CalibReg16s[5]-CalibReg16s[4])))+CalibReg16u[0];

hum=(((int32_t)(CalibReg8[1]-CalibReg8[0]))*((int32_t)(CalibReg16s[0]-CalibReg16s[2]))/((int32_t)(CalibReg16s[3]-CalibReg16s[2])))+CalibReg8[0];
	//wpisanie wyniku do zmiennej wynikowej dostepnej w main
Output[0]=(uint8_t)temp;	
	Output[1]=(uint8_t)hum;
	
};

void LPS22HB_init(I2C_HandleTypeDef *wzk){

HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_INTERRUPT_CFG, 1, &LSB22HB_INTERRUPT_CFGREG,1); 
HAL_Delay(2);
	
HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_CTRL_REG1, 1, &LSP22HBREG1,1); 
HAL_Delay(2);
	
	HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_CTRL_REG2, 1, &LSP22HBREG2,1); 
HAL_Delay(2);
	
	HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_CTRL_REG3, 1, &LSP22HBREG3,1); 
HAL_Delay(2);
	
		HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_FIFO_CTRL, 1, &LSB22HB_FIFO_CTRLREG,1); 
HAL_Delay(2);
	
HAL_I2C_Mem_Write_IT(wzk, LSB22HB_ADDRESS, LSB22HB_RES_CONF, 1, &LSB22HB_RES_CONFREG,1); 
HAL_Delay(2);	
	
};


void LPS22HB_odczyt(I2C_HandleTypeDef *wzk){

HAL_I2C_Mem_Read(wzk, LSB22HB_ADDRESS, LSB22HB_PRESS_OUT_XL, 1, buffer, 3,1000);//odczyt trzech rejestrow danych cisnienia
press=((buffer[2]<<16) | (buffer[1]<<8) | buffer[0])/4096 ; //polaczeni 2 rej i skalowanie
	Output[2]=(uint8_t)(press>>8);  //wynik musi byc wyslany w dwóch 8 bitowych porcjach
	Output[3]=(uint8_t)(press);
	
HAL_I2C_Mem_Read(wzk, LSB22HB_ADDRESS, LSB22HB_TEMP_OUT_L, 1, buffer, 2,1000);	//odczyt 2 rejestrow danych temepratury
	temp2=(buffer[1]<<8 | buffer[0])/100; //sKalowanie
	Output[4]=(uint8_t)(temp2);
	
	
};

