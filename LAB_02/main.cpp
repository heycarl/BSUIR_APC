#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define color 0x1D

struct VIDEO
{
	unsigned char symbol;
	unsigned char attribute;
};

void print_byte(VIDEO far* screen, char byte)
{
	char temp;
	for (char i = 0; i < 8; i++)
	{
		temp = byte % 2;
		byte = byte >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
}

void print()
{
	char temp;
	int i, val;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);

	val = inp(0x21);                // get mask Master registor
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
	screen++;

	val = inp(0xA1);              // get mask Slave registor
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
	screen += 63;
	outp(0x20,0x0A);

	val = inp(0x20);                    // get Master's request register
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
	screen++;

	outp(0xA0,0x0A);
	val = inp(0xA0);                     // get Slave's request register
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
	screen+=63;

	outp(0x20,0x0B);
	val = inp(0x20);                     // get Master's service register
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
	screen++;

	outp(0xA0,0x0B);
	val = inp(0xA0);                       // get Slave's service register
	for (i = 0; i < 8; i++)
	{
		temp = val % 2;
		val = val >> 1;
		screen->symbol = temp + '0';
		screen->attribute = color;
		screen++;
	}
//	print_byte(screen, val);
}

// IRQ 0-7

void interrupt(*base_interrupt_8) (...); 		// IRQ 0  - interrupt of timer (18,2 times per second)
void interrupt(*base_interrupt_9) (...); 		// IRQ 1  - interrupt of keypad (press and release key)
void interrupt(*base_interrupt_10) (...);		// IRQ 2  - interrupt for cascade interruptions in AT machines
void interrupt(*base_interrupt_11) (...);		// IRQ 3  - interrupt of async port COM 2
void interrupt(*base_interrupt_12) (...);		// IRQ 4  - interrupt of async port COM 1
void interrupt(*base_interrupt_13) (...);		// IRQ 5  - interrupt of hard disk controller (for XT)
void interrupt(*base_interrupt_14) (...);		// IRQ 6  - interrupt of floppy disk controller (when finish operation with floppy disk)
void interrupt(*base_interrupt_15) (...);		// IRQ 7  - interrupt of printer (when printer is ready to work)

// IRQ 8-15

void interrupt(*base_interrupt_70) (...);		// IRQ 8  - interrupt of real time clock
void interrupt(*base_interrupt_71) (...);		// IRQ 9  - interrupt of EGA controller
void interrupt(*base_interrupt_72) (...);		// IRQ 10 - reserved interrupt
void interrupt(*base_interrupt_73) (...);		// IRQ 11 - reserved interrupt
void interrupt(*base_interrupt_74) (...);		// IRQ 12 - reserved interrupt
void interrupt(*base_interrupt_75) (...);		// IRQ 13 - interrupt of mathematic soprocessor
void interrupt(*base_interrupt_76) (...);		// IRQ 14 - interrupt of hard disk
void interrupt(*base_interrupt_77) (...);		// IRQ 15 - reserved interrupt


void interrupt  hooked_interrupt08(...) { print(); base_interrupt_8(); }
void interrupt  hooked_interrupt09(...) { print(); base_interrupt_9(); }
void interrupt  hooked_interrupt0A(...) { print(); base_interrupt_10(); }
void interrupt  hooked_interrupt0B(...) { print(); base_interrupt_11(); }
void interrupt  hooked_interrupt0C(...) { print(); base_interrupt_12(); }
void interrupt  hooked_interrupt0D(...) { print(); base_interrupt_13(); }
void interrupt  hooked_interrupt0E(...) { print(); base_interrupt_14(); }
void interrupt  hooked_interrupt0F(...) { print(); base_interrupt_15(); }

void interrupt  hooked_interruptC0(...) { print(); base_interrupt_70(); }
void interrupt  hooked_interruptC1(...) { print(); base_interrupt_71(); }
void interrupt  hooked_interruptC2(...) { print(); base_interrupt_72(); }
void interrupt  hooked_interruptC3(...) { print(); base_interrupt_73(); }
void interrupt  hooked_interruptC4(...) { print(); base_interrupt_74(); }
void interrupt  hooked_interruptC5(...) { print(); base_interrupt_75(); }
void interrupt  hooked_interruptC6(...) { print(); base_interrupt_76(); }
void interrupt  hooked_interruptC7(...) { print(); base_interrupt_77(); }

void initialize()
{
	base_interrupt_8 = getvect(0x08);
	base_interrupt_9 = getvect(0x09);
	base_interrupt_10 = getvect(0x0A);
	base_interrupt_11 = getvect(0x0B);
	base_interrupt_12 = getvect(0x0C);
	base_interrupt_13 = getvect(0x0D);
	base_interrupt_14 = getvect(0x0E);
	base_interrupt_15 = getvect(0x0F);


	base_interrupt_70 = getvect(0x70);
	base_interrupt_71 = getvect(0x71);
	base_interrupt_72 = getvect(0x72);
	base_interrupt_73 = getvect(0x73);
	base_interrupt_74 = getvect(0x74);
	base_interrupt_75 = getvect(0x75);
	base_interrupt_76 = getvect(0x76);
	base_interrupt_77 = getvect(0x77);

	//set new handlers for IRQ 0-7
	setvect(0x80, hooked_interrupt08);
	setvect(0x81, hooked_interrupt09);
	setvect(0x82, hooked_interrupt0A);
	setvect(0x83, hooked_interrupt0B);
	setvect(0x84, hooked_interrupt0C);
	setvect(0x85, hooked_interrupt0D);
	setvect(0x86, hooked_interrupt0E);
	setvect(0x87, hooked_interrupt0F);

	//set new handlers for IRQ8-15
	setvect(0x08, hooked_interruptC0);
	setvect(0x09, hooked_interruptC1);
	setvect(0x0A, hooked_interruptC2);
	setvect(0x0B, hooked_interruptC3);
	setvect(0x0C, hooked_interruptC4);
	setvect(0x0D, hooked_interruptC5);
	setvect(0x0E, hooked_interruptC6);
	setvect(0x0F, hooked_interruptC7);

	_disable(); // CLI

	// interrupt init for Master
	//   0    0      0      1      0      0      0      1
	//                          tr Md  int Sze  use SL  use ICW4
	outp(0x20, 0x11);	//ICW1 - initialize master
	outp(0x21, 0x80);   //ICW2 - base vector for master
	outp(0x21, 0x04);	//ICW3 - the port bit of Slave (in binary format)
	outp(0x21, 0x01);	//ICW4 - default

	// interrupt init for Slave
	outp(0xA0, 0x11);  //ICW1 - initialize Slave
	outp(0xA1, 0x08);  //ICW2 - base vector for slave
	outp(0xA1, 0x02);  //ICW3 - the port number of connected port on Master
	outp(0xA1, 0x01);  //ICW4 - default

	_enable(); // STI
}

int main()
{
	unsigned far *fp;
	initialize();
	system("cls");

	printf("                   - mask    |\n");
	printf("                   - prepare |\n");
	printf("                   - service |\n");
	printf("Master  | Slave              |\n");
	printf("_____________________________|\n");

	FP_SEG(fp) = _psp; //получаем сегмент
	FP_OFF(fp) = 0x2c; //получаем смещение сегмента данных с переменными среды
	_dos_freemem(*fp); //освобождение памяти под них

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); //оставляем резидентной, первый параметр – код
												//завершения, второй – объём памяти, который должен быть зарезервирован
												//для программы после её завершения

	return 0;
}