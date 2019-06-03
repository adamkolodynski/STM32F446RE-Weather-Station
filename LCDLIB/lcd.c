#include "lcd.h"


//deklaracje statyczne

static void LCD_WRITE_DATA(Lcd_HandleTypeDef * lcd, uint8_t data);
static void LCD_WRITE_COMMAND(Lcd_HandleTypeDef * lcd, uint8_t command);
static void LCD_WRITEBYTE(Lcd_HandleTypeDef * lcd, uint8_t data, uint8_t len);


//definicje funkcji

//inicjalizacja struktury danych
Lcd_HandleTypeDef Lcd_create(
		Lcd_PortType port[], Lcd_PinType pin[],
		Lcd_PortType rs_port, Lcd_PinType rs_pin,
		Lcd_PortType en_port, Lcd_PinType en_pin)
{
	Lcd_HandleTypeDef lcd;

	lcd.en_pin = en_pin;
	lcd.en_port = en_port;

	lcd.rs_pin = rs_pin;
	lcd.rs_port = rs_port;

	lcd.data_pin = pin;
	lcd.data_port = port;

		return lcd;
}

//funckcja inicjalizujaca wyswietlacz-wysylanie komendy w celu inicjalizacji
void LCD_INIT(Lcd_HandleTypeDef * lcd)
{
	HAL_Delay(40); // zgodnie z karta katalogowa odczekac min 30ms po wlaczeniu zasilania	

	LCD_WRITE_COMMAND(lcd,FUNCTION_SET );	
	HAL_Delay(1);	
	LCD_WRITE_COMMAND(lcd,DISPLAY_ON );
	HAL_Delay(1);
	LCD_WRITE_COMMAND(lcd,CLEAR_DISPLAY );
	HAL_Delay(2);		
	LCD_WRITE_COMMAND(lcd,ENTRY_MODE );
	HAL_Delay(1);	

	
}

void LCD_STRING(Lcd_HandleTypeDef * lcd, char * string,  uint8_t line)
{
	if(line==1){
	LCD_WRITE_COMMAND(lcd,FIRST_LINE );	
	HAL_Delay(1);
	for(uint8_t i = 0; i < strlen(string); i++)
	{
		LCD_WRITE_DATA(lcd, string[i]);
	}
}
if(line==2){
	LCD_WRITE_COMMAND(lcd,SECOND_LINE );	
	HAL_Delay(1);
	for(uint8_t i = 0; i < strlen(string); i++)
	{
		LCD_WRITE_DATA(lcd, string[i]);
	}
}		

}
void LCD_CLEARDISPLAY(Lcd_HandleTypeDef * lcd){

LCD_WRITE_COMMAND(lcd,CLEAR_DISPLAY );
HAL_Delay(2);	

}

//Funkcje wewnetrzne

//wpisanie komendy do Lcd
void LCD_WRITE_COMMAND(Lcd_HandleTypeDef * lcd, uint8_t command)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_COMMAND_WRITE);	//wybor rejestru = komenda
	
		LCD_WRITEBYTE(lcd, command , LCD_8BIT);
	
	}

//Wyslanie danych do Lcd
void LCD_WRITE_DATA(Lcd_HandleTypeDef * lcd, uint8_t data)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_DATA_WRITE);			 //wybor rejestru = dane

		LCD_WRITEBYTE(lcd, data, LCD_8BIT);
		
}

//wysyla 8 bity przez linie danych do urzadzenia
void LCD_WRITEBYTE(Lcd_HandleTypeDef * lcd, uint8_t data, uint8_t len)
{
		for(uint8_t i = 0; i < len; i++)
	{
		HAL_GPIO_WritePin(lcd->data_port[i], lcd->data_pin[i], (data >> i) & 0x01);
	}

	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, 1);
  HAL_Delay(1);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, 0); 	
}