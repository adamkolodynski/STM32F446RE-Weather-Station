# STM32F446RE-Weather-Station
## Weather Station created with STM32F446RE, Expansion Board X-NUCLEO-IKS01A2, and LCD 2x16.

The repository includes codes for popular NUCLEO board based on STM32 F446RE written with the use of only Hardware Abstraction Layer (HAL)
library from ST and STM32CubeMX - STM32Cube initialization code generator.

## Peripheral modules used in this project include:  
1.X-NUCLEO-IKS01A2 expansion board connected directly to STM32F446RE, via the I2C serial bus.  
https://www.st.com/en/ecosystems/x-nucleo-iks01a2.html  
    Modules used:  
    -LPS22HB MEMS pressure sensor, 260-1260 hPa absolute digital output barometer  
    https://www.st.com/resource/en/datasheet/dm00140895.pdf  
    -HTS221: capacitive digital relative humidity and temperature  
    https://www.st.com/resource/en/datasheet/hts221.pdf  
2. LCD numeric display module WC 1602A compatible with HD44780 driver.  
    http://osworld.pl/wp-content/uploads/WC1602A-STBLWHTC-06.pdf

# Instructions  
## Board connections:

| STM32F446  | LCD 2x16 |                                    
| ------------- | ------------- |           
| GND  | Vss  |                             
| +5V  | Vcc  |                             
| 0-5V  | Vee  |                            
| PB6  | RS  |                              
| GND(write only)  | RW  |
| PC7  | E  |
| PC6  | D0 |
| PA12 | Dl  |
| PC2  | D2  |
| PC3  | D3  |
| PA9  | D4  |
| PA8  | D5  |
| PC9  | D6  |
| PC8  | D7  |
| +5V  | LED+  |
| GND  | LED-  |
    
| STM32F446  | X-NUCLEO-IKS01A2 |
| ------------- | ------------- |           
| GND  | GND  |                             
| +3.3V  | +3.3V  |                             
| PB8(I2C1_SDA)  | CN5-9  |   
| PB9(I2C1_SCL)  | CN5-10  |   
## 2. Upload the code inside the board  
The code contains all libraries used by peripherical modules but make sure you all have a working environment.  
Build the code and run it.  
If it fails then make sure default HAL libraries, SENSORLIB and LCDLIB are added inside project properties.  
