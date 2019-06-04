# STM32F446RE-Weather-Station
## Weather Station created with STM32F446RE, Expansion Board X-NUCLEO-IKS01A2, and LCD 2x16.

The repository includes codes for popular NUCLEO board based on STM32 F446RE written with the use of only Hardware Abstraction Layer (HAL)
library from ST and STM32CubeMX - STM32Cube initialization code generator.

Peripheral modules used in this project include:  
1.X-NUCLEO-IKS01A2 expansion board connected directly to STM32F446RE, via the I2C serial bus.  
https://www.st.com/en/ecosystems/x-nucleo-iks01a2.html  
    Modules used:  
    -LPS22HB MEMS pressure sensor, 260-1260 hPa absolute digital output barometer  
    https://www.st.com/resource/en/datasheet/dm00140895.pdf  
    -HTS221: capacitive digital relative humidity and temperature  
    https://www.st.com/resource/en/datasheet/hts221.pdf  
2. LCD numeric display module WC 1602A compatible with HD44780 driver.  
    http://osworld.pl/wp-content/uploads/WC1602A-STBLWHTC-06.pdf

    
    
    

