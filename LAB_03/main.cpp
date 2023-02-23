//
// Created by Alexandr on 2/23/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

unsigned int freqs_to_play[] = { 196,261,329,196,261,329,196,261,329 };
unsigned int note_delay = 400;

void play_freqs();
void print_state();
void char_to_bin(unsigned char state, char* str);
void set_speaker(int is_active);
void set_freq(int freq);
void Menu();

int main() {
	Menu();
	return 0;
}

void Menu() {
	int choice = 0;
	while (1) {
		printf("1 Buzzer check\n");
		printf("2 Chanel states\n");
		printf("0 Quit\n\n");

		printf("Enter option: ");
		scanf("%d", &choice);
		switch (choice) {
		case 0:
			return;
		case 1:
			play_freqs();
			break;
		case 2:
			print_state();
			break;
		default:
			return;
		}
	}
}

//функция считывающая слова состояния каналов
void print_state()
{
	char* bin_state;
	int iChannel;
	char state;
	bin_state = (char*)calloc(9, sizeof(char));
	if (bin_state == NULL)
	{
		printf("Memory allocation error");
		exit(EXIT_FAILURE);
	}
	// https://www.frolov-lib.ru/books/bsp.old/v33/ch5.htm
	outp(0x43, 0xE2);       //заносим управляющее слово, соответствующее команде RBC (Чтение состояния канала) и номеру канала 0
	state = inp(0x40);      //чтение слова состояния канала 0
	char_to_bin(state, bin_state);
	printf("Channel 0x40 state: %s\n", bin_state);

	bin_state[0] = '\0';
	outp(0x43, 0xE4);
	state = inp(0x41);
	char_to_bin(state, bin_state);
	printf("Channel 0x41 state: %s\n", bin_state);

	bin_state[0] = '\0';
	outp(0x43, 0xE8);
	state = inp(0x42);
	char_to_bin(state, bin_state);
	printf("Channel 0x42 state: %s\n", bin_state);

	free(bin_state);
	return;
}

//функция перевода в двоичный код
void char_to_bin(unsigned char state, char* str)
{
	int i;
	char temp;
	for (i = 7; i >= 0; i--)
	{
		temp = state % 2;
		state /= 2;
		str[i] = temp + '0';
	}
	str[8] = '\0';
}

//функция установки значения счетчика
void set_freq(int freq) {
	long base_freq = 1193180;  // внутренняя частота таймера
	long calculated_freq;
	outp(0x43, 0xB6);     //запись в регистр канала канал 2, операция 4, режим работы 3, формат 0
	// 0b10110110

	// формат:
	// bit0   State
	//  0     Двочный
	//  1     Двоично-десятичный

	// режим работы:
	// bit3 bit2 bit1   State
	//  0    0    0     Генерация прерывания IRQ0 при установке счетчика в 0
	//  0    0    1     Установка в режим ждущего мультивибратора
	//  0    1    0     Установка в режим генератора импульсов
	//  0    1    1     Установка в режим генератора прямоугольных импульсов
	//  1    0    0     Установка в режим программно зависимого одновибратора
	//  1    0    1     Установка в режим аппаратно-зависимого одновибратора

	// тип операции:
	// bit5 bit4   State
	//  0    0     Команда блокировки счетчика
	//  0    1     Чтение/запись только младшего байта
	//  1    0     Чтение/запись только старшего байта
	//  1    1     Чтение/запись младшего, а за ним старшего байта

	// выбор канала:
	// bit7 bit6   State
	//  0    0     Выбор первого канала (0)
	//  0    1     Выбор второго канала (1)
	//  1    0     Выбор третьего канала (2)
	//  1    1     Команда считывания значений из регистров каналов
	calculated_freq = base_freq / freq;
	outp(0x42, calculated_freq % 256); // младший байт делителя
	calculated_freq /= 256;
	outp(0x42, calculated_freq);       //старший байт делителя
	return;
}

//функция работы с громкоговорителем
void set_speaker(int is_active) {

//	7	RAM parity error occurred
//	6	I/O channel parity error occurred
//	5	mirrors timer 2 output condition
//	4	toggles with each refresh request
//	3	NMI I/O channel check status
//	2	NMI parity check status
//	1	speaker data status
//	0	timer 2 clock gate to speaker status

	if (is_active) {
		outp(0x61, inp(0x61) | 0x3);    //устанавливаем 2 младших бита в порте динаминка 11
		// 0b00000011
		return;
	} else {
		outp(0x61, inp(0x61) & 0xFC);   //устанавливаем 2 младших бита в порте динаминка 00
		// 0x11111100
		return;
	}
}

//функция воспроизведения песни
void play_freqs() {
	for (int i = 0; i < sizeof(freqs_to_play) / sizeof (unsigned int); i++) {
		set_freq(freqs_to_play[i]);
		set_speaker(1);    //включаем громкоговоритель
		delay(note_delay);         //устанавливаем длительность мс
		set_speaker(0);    //выключаем громкоговоритель
	}
}
