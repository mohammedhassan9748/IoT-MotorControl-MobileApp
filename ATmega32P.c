#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ALCD.h"

#ifndef F_CPU
#define F_CPU 16000000
#endif

// Pin Definitions
char IN1 = 4;          // PD4
char IN2 = 5;          // PD5
char String[3] = "000";
uint8_t percentage = 0;
uint8_t Start = 7;

// Function Prototypes
void PORTD_Init(void);
void Timer0_Init(void);
void ADC_Init(void);
void Interrupt_Init(void);
void LCD_Init(void);
void Uart_init(int ubrr);
void UART_putc(unsigned char data);
char UART_getc(void);
void Send_Motor_Speed_Blynk(void);
void convert_Number_String(uint8_t Num);
void Send_Motor_Speed_Dir_LCD(void);
void Send_Motor_Speed_Dir(void);
void Start_Stop_Motor(void);

// Main Function
int main(void)
{
	LCD_Init();
	Uart_init(103);
	PORTD_Init();
	ADC_Init();
	Interrupt_Init();
	Timer0_Init();
	sei();   // Enable global interrupts

	while (1)
	{
		ADCSRA |= (1 << ADSC);     // Start ADC conversion
		Send_Motor_Speed_Dir();
		Start_Stop_Motor();
	}
}

// Initialize Port D
void PORTD_Init(void)
{
	DDRD |= (1 << IN1) | (1 << IN2) | (1 << Start);    // Set IN1, IN2, and Start as outputs
	PORTD |= (1 << IN1);    // Set IN1 high
	PORTD &= ~(1 << IN2);   // Set IN2 low
	PORTD &= ~(1 << Start); // Set Start low
}

// Initialize Timer0
void Timer0_Init(void)
{
	DDRD |= (1 << 6);     // PD6 as output
	OCR0A = 0;
	TCCR0A = (1 << COM0A1) | (1 << WGM00);
	TCCR0B = 0x01;
}

// Initialize ADC
void ADC_Init(void)
{
	ADMUX |= (1 << REFS0);    // Set reference voltage to AVCC
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // Enable ADC, enable ADC interrupt, set ADC prescaler to 128
}

// Initialize External Interrupt
void Interrupt_Init(void)
{
	DDRD &= ~(1 << PIND2);      // Set PIND2 (INT0) as input
	PORTD |= (1 << PIND2);      // Enable pull-up resistor for PIND2
	EICRA |= (1 << ISC01);      // Set INT0 to trigger on falling edge
	EICRA &= ~(1 << ISC00);     // Set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);       // Enable INT0 interrupt
}

// Initialize LCD
void LCD_Init(void)
{
	lcd_init(16);               // Initialize LCD with 16 columns
	lcd_clear();                // Clear LCD screen
	lcd_gotoxy(0, 0);           // Set cursor to first line, first column
	lcd_puts("S P E E D = ");   // Display text on LCD
	lcd_gotoxy(0, 1);           // Set cursor to second line, first column
	lcd_puts("D I R = ");        // Display text on LCD
}

// Initialize UART
void Uart_init(int ubrr)
{
	UBRR0L = (uint8_t)(ubrr & 0xFF);     // Set lower byte of baud rate
	UBRR0H = (uint8_t)(ubrr >> 8);        // Set higher byte of baud rate
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);    // Enable UART receiver and transmitter
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);    // Set frame format: 8 data bits, no parity, 1 stop bit
}

// Send a character over UART
void UART_putc(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));    // Wait for transmit buffer to be empty
	UDR0 = data;                         // Load data into transmit register
}

// Receive a character over UART
char UART_getc(void)
{
	while (!(UCSR0A & (1 << RXC0)));     // Wait for data to be received
	return UDR0;                         // Return received data
}

// Send Motor Speed to Blynk
void Send_Motor_Speed_Blynk(void)
{
	UART_putc(percentage);    // Send motor speed over UART
}

// Convert a number to a string
void convert_Number_String(uint8_t Num)
{
	uint8_t i = 3;
	while (i != 0)
	{
		String[i - 1] = (Num % 10) + '0';   // Convert each digit to ASCII and store in String
		Num /= 10;
		i--;
	}
}

// Send Motor Speed and Direction to LCD
void Send_Motor_Speed_Dir_LCD(void)
{
	convert_Number_String(percentage);     // Convert motor speed to string
	lcd_gotoxy(13, 0);
	lcd_puts(String);                      // Display motor speed on LCD

	if ((PORTD & (1 << IN1)) && (PORTD & (1 << PIND7)))
	{
		lcd_gotoxy(12, 1);
		lcd_puts("    ");
		lcd_gotoxy(13, 1);
		lcd_puts("CW ");                  // Display "CW" for clockwise direction
	}
	else if ((PORTD & (1 << IN2)) && (PORTD & (1 << PIND7)))
	{
		lcd_gotoxy(12, 1);
		lcd_puts("    ");
		lcd_gotoxy(13, 1);
		lcd_puts("CCW");                  // Display "CCW" for counterclockwise direction
	}
	else
	{
		lcd_gotoxy(12, 1);
		lcd_puts("    ");
		lcd_gotoxy(12, 1);
		lcd_puts("STOP");                 // Display "STOP" for stopped motor
	}
}

// Send Motor Speed and Direction
void Send_Motor_Speed_Dir(void)
{
	Send_Motor_Speed_Blynk();               // Send motor speed to Blynk
	Send_Motor_Speed_Dir_LCD();             // Send motor speed and direction to LCD
}

// Start or Stop the Motor based on UART input
void Start_Stop_Motor(void)
{
	uint8_t Status = UART_getc();           // Get status from UART
	if (Status == 1)
	{
		PORTD |= (1 << Start);               // Set Start pin high to start the motor
	}
	else
	{
		PORTD &= ~(1 << Start);              // Set Start pin low to stop the motor
	}
}

// ADC Conversion Complete Interrupt Service Routine
ISR(ADC_vect)
{
	OCR0A = (uint8_t)round((ADC * 255.0) / 1023.0);   // Update OCR0A with ADC result

	if (OCR0A < 102)
	{
		percentage = 0;
	}
	else
	{
		percentage = (uint8_t)round((OCR0A - 102.0) * (100.0 / 153.0));   // Calculate motor speed percentage
	}
}

// External Interrupt 0 Service Routine
ISR(INT0_vect)
{
	PORTD ^= (1 << IN1) | (1 << IN2);      // Toggle IN1 and IN2 pins
	_delay_ms(250);                        // Delay for debouncing
}
