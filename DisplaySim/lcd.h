/*
//  lcd.h 
//
//		Routines to write to Hitachi HD44780 compatible LCD driver 
//
//      (c) 2006, Harry Broeders <J.Z.M.Broeders@hhs.nl>
//      Version 1.0
//
//		This software uses delay routines written by:
//
//      (c) 2006, Jesse op den Brouw <J.E.J.opdenBrouw@hhs.nl>
//      Version 1.0
*/

#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

/* All programs that want to use the LCD should
   start by calling lcd_init() */

void lcd_init(void); /* initialize LCD */

/* low level API (Application Programmers Interface*/

uint8_t lcd_read_data(void); 
void lcd_write_data(uint8_t data);
uint8_t lcd_read_command(void);
void lcd_write_command(uint8_t command);

/* somewhat higher level API */

void lcd_cls(void); /* clear screen */
void lcd_home(void); /* place cursor in home position (upper left corner) */
void lcd_cursor(bool cursorOn, bool cursorBlinks); /* configure cursor */
void lcd_goto(uint8_t row, uint8_t column); /* place cursor on row (0..1) and column (0..39) */
uint8_t lcd_get_row(void); /* get current cursor row position (0..1) */
uint8_t lcd_get_column(void); /* get current cursor column position (0..39) */

void lcd_putc(char c); /* write character c */
void lcd_puts(char* s); /* write string s */
void lcd_puts_P(const char* PROGMEM s); /* write string s from program space */

/* The rest of this header file is needed to make the 
   delays needed in the LCD implementation dependent  
   from theAVR clock freqency which should be defined
   as F_CPU (in Hz).
   This is NOT RELEVANT for application programmers.
*/

#ifndef F_CPU
#define F_CPU 3686000
#warning
#warning F_CPU set to 3.686 MHz !!!!
#warning
#endif

#if F_CPU < 500000
#error F_CPU is too low!!!
#endif
#if F_CPU > 16000000
#error F_CPU is too high!!!
#error
#endif

void _lcd_delay_8(uint8_t);
void _lcd_delay_16(uint16_t);

#define _lcd_delay_us_small(us) \
		_lcd_delay_8((((F_CPU)/1000000.0)*(us))/3.0)
#define _lcd_delay_us(us) \
		_lcd_delay_16((((F_CPU)/1000000.0)*(us))/4.0)

#endif
