/*
 * InstructionSet.h
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */

#pragma once

namespace cpu {
enum OpCodes : unsigned char {
	STOP,
	LDA_immediate,
	LDA_absolute,
	STA_immediate,
	STA_absolute,
	LDB_immediate,
	LDB_absolute,
	STB_immediate,
	STB_absolute,
	add_immediate,
	add_absolute,
	add_B,
	sub_immediate,
	sub_absolute,
	sub_B,
	mov_immediate,
	mov_absolute,
	jmp_immediate,
	jmp_absolute,
	jmp_relativeFwd,
	jmp_relativeBkwd,
	print_memLoc,
	print_A,
	print_B,
	BRZ,
	BRNZ,
	BRE,
	BRNE,
	BRN,
	BRNN,
	BRC,
	BRNC,
	SPI,
	Stack_push,
	Stack_push_A,
	Stack_push_B,
	Stack_pop,
	Stack_pop_A,
	Stack_pop_B,
	JSR,
	RET
};
}
