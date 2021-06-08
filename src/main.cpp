/*
 * main.cpp
 *
 *  Created on: 26 Jan 2021
 *      Author: Martin Blecker
 */
#include "CPU.h"

using namespace cpu;
int main(){
	CPU* cpu = new CPU();
	//
	array< unsigned char, MEM_SIZE > program {
			//0x0
			LDA_immediate,
			0b1010,
			LDB_immediate,
			0b0101,
			//0x4
			SPI,
			0x30,
			Stack_push_A,
			Stack_push_B,
			//0x8
			Stack_push,
			0xFF,
			LDA_immediate,
			0x0,
			//0xc
			BRZ,
			0x16,
			Stack_pop,
			0x20,
			//0x10
			print_memLoc,
			0x20,
			sub_immediate,
			0x1,
			//0x14
			jmp_immediate,
			0xc,
			0x0 };

	program[0x20] = 0x0;
	cpu->mem->mem = program;
	cpu->settings.printMicroInstructions = false;
	cpu->settings.printOpCodeAddresses = true;
	cpu->settings.printOperands = true;
	cpu->settings.printOpCodes = true;
	cpu->settings.printData_Hex = false;
	cpu->run();
	delete cpu;
}
