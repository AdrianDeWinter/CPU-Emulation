/*
 * CPU.h
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */

#pragma once
#include <bitset>
#include <array>
#include <iostream>
#include <string>

#include "InstructionSetDefinition.h"
using namespace std;

namespace cpu {
const unsigned char ADDRESS_BITS = 16;
const unsigned char MEM_SIZE = 64;

/*
 * @brief Provided one Byte (8 bit) of storage space
 */
struct dataSpace {
	bitset< 8 > byte;
	/*
	 * @brief clears the stored byte (sets it to 0x0)
	 */
	void clear();

	void increment();
	void decrement();

	void add( unsigned char i );
	void add( bitset< 8 > i );
	void sub( unsigned char i );
	void sub( bitset< 8 > i );
};

struct Bus : public dataSpace {
};

struct commonBase {
	Bus* dataBus;
	Bus* addressBus;
	commonBase( Bus* addressBus, Bus* dataBus );
};

struct busConnection_Data {
private:
	Bus* dataBus;
	bitset< 8 >* byteField;
public:
	busConnection_Data( Bus* dataBus, bitset< 8 >* byte );
	void readFromDataBus();
	void writeToDataBus();

	void addFromDataBus();
	void subFromDataBus();
};

struct busConnection_Address {
private:
	Bus* addressBus;
	bitset< 8 >* byteField;
public:
	busConnection_Address( Bus* addressBus, bitset< 8 >* byte );
	void readFromAddressBus();
	void writeToAddressBus();

	void addFromAddressBus();
	void subFromAddressBus();
};

struct Register : public busConnection_Data, dataSpace {
	Register( Bus* dataBus );
};

struct InstructionCounter : public busConnection_Address,
		busConnection_Data,
		dataSpace {
	InstructionCounter( Bus* addressBus, Bus* dataBus );
};

struct StackPointer : busConnection_Address, busConnection_Data, dataSpace {
	StackPointer( Bus* addressBus, Bus* dataBus );
};
struct OpRegister : busConnection_Data, busConnection_Address, dataSpace {
	OpRegister( Bus* addressBus, Bus* dataBus );
};

struct InstructionRegister {
	OpRegister* opCode;
	OpRegister* operand1;
	OpRegister* operand2;
	OpRegister* operand3;
	bool jmp = false;
	InstructionRegister( Bus* addressBus, Bus* dataBus );
	~InstructionRegister();
};

struct Memory : public commonBase {
	array< unsigned char, MEM_SIZE > mem { };

	Memory( Bus* ab, Bus* db );

	void writeToDataBus();
	void writeToAddressBus();
	void readFromDataBus();
};

struct Status : public Register {
	Status( Bus* db );

	auto Zero();
	auto InterruptEnable();
	auto Neg();
	auto Carry();
};

struct CPUSettings {
	bool printOpCodeAddresses = true;
	bool printOpCodes = true;
	bool printOperands = true;
	bool printMicroInstructions = true;
	bool printAddresses_Hex = true;
	bool printOperands_Hex = true;
	bool printData_Hex = true;
	bool printBool_Textual = true;
	void printData( bitset< 8 >* value );
	void printOperand( bitset< 8 >* value );
	void printAddress( bitset< 8 >* value );
	void printBool( bool b );
};

struct CPU {
	Bus* dataBus;
	Bus* addressBus;
	InstructionCounter* IC;
	Register* A;
	Register* B;
	InstructionRegister* insReg;
	Status* flags;
	Memory* mem;
	StackPointer* SP;
	CPUSettings settings;

	vector< Instruction > instructionSet;
	CPU();
	~CPU();
	void fetchInstruction();
	bool decode();

	void run();
};
}
