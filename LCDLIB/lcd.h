#ifndef LCD_H_
#define LCD_H_

#include "stm32f4xx_hal.h"
#include "string.h"
#include "main.h"


//komendy inicjujace wyswietlacz

#define FUNCTION_SET 0x3F // 2-liniowy wyswietlacz, Display ON
#define DISPLAY_ON 0x0C // Display ON, Cursor OFF, Blink OFF
#define CLEAR_DISPLAY 0x01  //Display ON
#define ENTRY_MODE 0x06 //Increment ON, Shift OFF

//definicje ogolne
#define FIRST_LINE 0x80 //Kursor na pierwszej linii
#define SECOND_LINE 0xC0 //Kursor na drugiej linii
#define LCD_8BIT 8
#define LCD_DATA_WRITE 1
#define LCD_COMMAND_WRITE 0

//struktura do przechowywania danych
#define Lcd_PortType GPIO_TypeDef*
#define Lcd_PinType uint16_t

typedef struct {
	Lcd_PortType * data_port;
	Lcd_PinType * data_pin;

	Lcd_PortType rs_port;
	Lcd_PinType rs_pin;

	Lcd_PortType en_port;
	Lcd_PinType en_pin;

} Lcd_HandleTypeDef;


// Funkcje publiczne

void LCD_INIT(Lcd_HandleTypeDef * lcd);  //Inicjalizacja wyswietlacza
void LCD_STRING(Lcd_HandleTypeDef * lcd, char * string, uint8_t line); //Wyswietlenie ciagu znakow na wyswietlaczu na wybranej linii
void LCD_CLEARDISPLAY(Lcd_HandleTypeDef * lcd); //Wyczysc wyswietlacz


// Stworzenie struktury danych do obslugi wyswietlacza
Lcd_HandleTypeDef Lcd_create(
		Lcd_PortType port[], Lcd_PinType pin[],
		Lcd_PortType rs_port, Lcd_PinType rs_pin,
		Lcd_PortType en_port, Lcd_PinType en_pin);



#endif /* LCD_H_ */