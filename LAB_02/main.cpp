#include <dos.h>

typedef struct {
	unsigned char symb;
	unsigned char color;
} VIDEO;

int color = 0x1F;

void init();
void read_registers();
void decode_byte_to_bits(int indent, int byte);


//System int (buffer)

// IRQ0-7
void _interrupt(*source_08) (...); // system timer
void _interrupt(*source_09) (...); // keyboard controller
void _interrupt(*source_0A) (...); // cascade (прерывание от slave контроллера)
void _interrupt(*source_0B) (...); // serial port COM2
void _interrupt(*source_0C) (...); // serial port COM1
void _interrupt(*source_0D) (...); // parallel port 2 and 3 or sound card
void _interrupt(*source_0E) (...); // floppy controller
void _interrupt(*source_0F) (...); // parallel port 1

// IRQ8-15
void _interrupt(*source_70) (...); // CMOS
void _interrupt(*source_71) (...); // ACPI
void _interrupt(*source_72) (...); // open/SCSI/NIC
void _interrupt(*source_73) (...); // open/SCSI/NIC
void _interrupt(*source_74) (...); // mouse controller
void _interrupt(*source_75) (...); // math co-processor
void _interrupt(*source_76) (...); // ATA channel 1
void _interrupt(*source_77) (...); // ATA channel 2


// handled_ interrupt handlers
// IRQ0-7
void _interrupt handled_08(...) { read_registers(); source_08(); }
void _interrupt handled_09(...) { read_registers(); source_09(); }
void _interrupt handled_0A(...) { read_registers(); source_0A(); }
void _interrupt handled_0B(...) { read_registers(); source_0B(); }
void _interrupt handled_0C(...) { read_registers(); source_0C(); }
void _interrupt handled_0D(...) { read_registers(); source_0D(); }
void _interrupt handled_0E(...) { read_registers(); source_0E(); }
void _interrupt handled_0F(...) { read_registers(); source_0F(); }

// IRQ8-15
void _interrupt handled_70(...) { read_registers(); source_70(); }
void _interrupt handled_71(...) { read_registers(); source_71(); }
void _interrupt handled_72(...) { read_registers(); source_72(); }
void _interrupt handled_73(...) { read_registers(); source_73(); }
void _interrupt handled_74(...) { read_registers(); source_74(); }
void _interrupt handled_75(...) { read_registers(); source_75(); }
void _interrupt handled_76(...) { read_registers(); source_76(); }
void _interrupt handled_77(...) { read_registers(); source_77(); }

void init()
{
	// IRQ0-7
	source_08 = getvect(0x08); //Timer
	source_09 = getvect(0x09); //Keyboard
	source_0A = getvect(0x0A); //SLAVE REQUEST
	source_0B = getvect(0x0B); //COM2,4
	source_0C = getvect(0x0C); //COM1,3
	source_0D = getvect(0x0D); //LPT2
	source_0E = getvect(0x0E); //Floppy
	source_0F = getvect(0x0F); //LPT1
	// IRQ8-15
	source_70 = getvect(0x70); //CMOS
	source_71 = getvect(0x71); //Respond of ray
	source_72 = getvect(0x72); //Additional devices
	source_73 = getvect(0x73); //Additional devices
	source_74 = getvect(0x74); //Mouse
	source_75 = getvect(0x75); //FPU
	source_76 = getvect(0x76); //1 IDE-Controller
	source_77 = getvect(0x77); //2 IDE-Controller

	setvect(0xD8, handled_08);
	setvect(0xD9, handled_09);
	setvect(0xDA, handled_0A);
	setvect(0xDB, handled_0B);
	setvect(0xDC, handled_0C);
	setvect(0xDD, handled_0D);
	setvect(0xDE, handled_0E);
	setvect(0xDF, handled_0F);

	setvect(0x08, handled_70);
	setvect(0x09, handled_71);
	setvect(0x0A, handled_72);
	setvect(0x0B, handled_73);
	setvect(0x0C, handled_74);
	setvect(0x0D, handled_75);
	setvect(0x0E, handled_76);
	setvect(0x0F, handled_77);

	_disable(); // cli

	// Master
	outp(0x20, 0x11);
//	outp(0x21, 0xD8); // D8h basic vector
	outp(0x21, 0x80	); // D8h basic vector
	outp(0x21, 0x04);
	outp(0x21, 0x01);
	// Slave
	outp(0xA0, 0x11);
//	outp(0xA1, 0x08); // 08h basic vector
	outp(0xA1, 0x08); // 08h basic vector
	outp(0xA1, 0x02);
	outp(0xA1, 0x01);

	_enable(); // sti
}

int main()
{
	init();
	// Make resident program
	unsigned far* fp;
	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
	return 0;
}

// Read registers
void read_registers()
{
	int gui_offset = 40;
	int reg;

	VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);

	screen += gui_offset;
	screen->symb = 'I';
	screen->color = color;
	screen++;
	screen->symb = 'M';
	screen->color = color;
	screen++;
	screen->symb = 'R';
	screen->color = color;

	screen += 7;
	screen->symb = 'I';
	screen->color = color;
	screen++;
	screen->symb = 'R';
	screen->color = color;
	screen++;
	screen->symb = 'R';
	screen->color = color;

	screen += 7;
	screen->symb = 'I';
	screen->color = color;
	screen++;
	screen->symb = 'S';
	screen->color = color;
	screen++;
	screen->symb = 'R';
	screen->color = color;

	screen -= gui_offset + 20;

	screen += 80 + gui_offset - 10;
	screen->symb = 'M';
	screen->color = color;
	screen++;
	screen->symb = 'A';
	screen->color = color;
	screen++;
	screen->symb = 'S';
	screen->color = color;
	screen++;
	screen->symb = 'T';
	screen->color = color;
	screen++;
	screen->symb = 'E';
	screen->color = color;
	screen++;
	screen->symb = 'R';
	screen->color = color;
	screen++;
	screen->symb = ':';
	screen->color = color;

	screen -= gui_offset - 10 + 80 + 6;


	screen += 80 * 2 + gui_offset - 10;
	screen->symb = 'S';
	screen->color = color;
	screen++;
	screen->symb = 'L';
	screen->color = color;
	screen++;
	screen->symb = 'A';
	screen->color = color;
	screen++;
	screen->symb = 'V';
	screen->color = color;
	screen++;
	screen->symb = 'E';
	screen->color = color;
	screen++;
	screen->symb = ':';
	screen->color = color;




	// Leading controller
	reg = inp(0x21); // IMR - mask
	decode_byte_to_bits(80+0+gui_offset, reg);

	outp(0x20, 0x0B); // IRR - request
	reg = inp(0x20);
	decode_byte_to_bits(80+9+gui_offset, reg);

	outp(0x20, 0x0A); // ISR - service

	reg = inp(0x20);
	decode_byte_to_bits(80+18+gui_offset, reg);

	reg = inp(0xA1); // IMR
	decode_byte_to_bits(80*2+gui_offset, reg);

	outp(0xA0, 0x0B); //IRR
	reg = inp(0xA0);
	decode_byte_to_bits(80*2+9+gui_offset, reg);

	outp(0xA0, 0x0A); // ISR
	reg = inp(0xA0);
	decode_byte_to_bits(80*2+18+gui_offset, reg);
}

void decode_byte_to_bits(int indent, int byte)
{
	char bit;
	int i;
	VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);
	screen += indent;
	for (i = 7; i >= 0; i--)
	{
		//Count bits
		bit = byte % 2;
		byte /= 2;
		//Fill the screen
		screen->symb = bit + '0';
		screen->color = color;
		screen++;
	}
}