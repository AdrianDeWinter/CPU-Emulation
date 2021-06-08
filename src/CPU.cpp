/*
 * CPU.cpp
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */

#include "CPU.h"
namespace cpu {

void dataSpace::clear(){
	byte = 0;
}

void dataSpace::increment(){
	byte = bitset< 8 >( byte.to_ulong() + 1 );
}
void dataSpace::decrement(){
	byte = bitset< 8 >( byte.to_ulong() - 1 );
}
void dataSpace::add( unsigned char i ){
	byte = bitset< 8 >( byte.to_ulong() + i );
}
void dataSpace::add( bitset< 8 > i ){
	byte = bitset< 8 >( byte.to_ulong() + i.to_ulong() );
}
void dataSpace::sub( unsigned char i ){
	byte = bitset< 8 >( byte.to_ulong() - i );
}
void dataSpace::sub( bitset< 8 > i ){
	byte = bitset< 8 >( byte.to_ulong() - i.to_ulong() );
}

commonBase::commonBase( Bus* addressBus, Bus* dataBus ){
	this->dataBus = dataBus;
	this->addressBus = addressBus;
}

busConnection_Data::busConnection_Data( Bus* db, bitset< 8 >* byte ){
	dataBus = db;
	byteField = byte;
}

void busConnection_Data::readFromDataBus(){
	*byteField = dataBus->byte;
}
void busConnection_Data::writeToDataBus(){
	dataBus->byte = *byteField;
}

void busConnection_Data::addFromDataBus(){
	*byteField = bitset< 8 >(
			byteField->to_ulong() + dataBus->byte.to_ulong() );
}
void busConnection_Data::subFromDataBus(){
	*byteField = bitset< 8 >(
			byteField->to_ulong() - dataBus->byte.to_ulong() );
}

busConnection_Address::busConnection_Address( Bus* ab, bitset< 8 >* byte ){
	addressBus = ab;
	byteField = byte;
}

void busConnection_Address::readFromAddressBus(){
	*byteField = addressBus->byte;
}
void busConnection_Address::writeToAddressBus(){
	addressBus->byte = *byteField;
}

void busConnection_Address::addFromAddressBus(){
	*byteField = bitset< 8 >(
			byteField->to_ulong() + addressBus->byte.to_ulong() );
}
void busConnection_Address::subFromAddressBus(){
	*byteField = bitset< 8 >(
			byteField->to_ulong() - addressBus->byte.to_ulong() );
}

Register::Register( Bus* dataBus ) :
		busConnection_Data( dataBus, &this->byte ){
}

InstructionCounter::InstructionCounter( Bus* addressBus, Bus* dataBus ) :
		busConnection_Address( addressBus, &this->byte ), busConnection_Data(
				dataBus, &this->byte ){
}

Status::Status( Bus* db ) :
		Register( db ){
}

auto Status::Zero(){
	return byte[1];
}

auto Status::InterruptEnable(){
	return byte[7];
}

auto Status::Neg(){
	return byte[2];
}

auto Status::Carry(){
	return byte[0];
}

StackPointer::StackPointer( Bus* addressBus, Bus* dataBus ) :
		busConnection_Address( addressBus, &byte ), busConnection_Data( dataBus,
				&byte ){
}

OpRegister::OpRegister( Bus* ab, Bus* db ) :
		busConnection_Data( db, &byte ), busConnection_Address( ab, &byte ){

}
InstructionRegister::InstructionRegister( Bus* ab, Bus* db ){
	opCode = new OpRegister( ab, db );
	operand1 = new OpRegister( ab, db );
	operand2 = new OpRegister( ab, db );
	operand3 = new OpRegister( ab, db );
}

InstructionRegister::~InstructionRegister(){
	delete opCode;
	delete operand1;
	delete operand2;
	delete operand3;
}

Memory::Memory( Bus* ab, Bus* db ) :
		commonBase( ab, db ){
}

void Memory::writeToDataBus(){
	dataBus->byte = mem[addressBus->byte.to_ulong()];
}
void Memory::writeToAddressBus(){
	addressBus->byte = mem[addressBus->byte.to_ulong()];
}
void Memory::readFromDataBus(){
	mem[addressBus->byte.to_ulong()] = (unsigned char)dataBus->byte.to_ulong();
}

void CPUSettings::printData( bitset< 8 >* value ){
	if ( printData_Hex )
		cout << hex << "0x" << value->to_ulong();
	else
		cout << "0b" << *value;
}
void CPUSettings::printOperand( bitset< 8 >* value ){
	if ( printOperands_Hex )
		cout << hex << "0x" << value->to_ulong();
	else
		cout << "0b" << *value;
}
void CPUSettings::printAddress( bitset< 8 >* value ){
	if ( printAddresses_Hex )
		cout << hex << "0x" << value->to_ulong();
	else
		cout << "0b" << *value;
}

void CPUSettings::printBool( bool b ){
	if ( printBool_Textual )
		cout << boolalpha << b;
	else
		cout << b;
}

CPU::CPU(){
	this->dataBus = new Bus();
	this->addressBus = new Bus();
	this->IC = new InstructionCounter( addressBus, dataBus );
	this->A = new Register( dataBus );
	this->B = new Register( dataBus );
	this->flags = new Status( dataBus );
	this->mem = new Memory( addressBus, dataBus );
	this->instructionSet = buildInstructionSet();
	this->insReg = new InstructionRegister( addressBus, dataBus );
	this->SP = new StackPointer( addressBus, dataBus );
}

CPU::~CPU(){
	delete SP;
	delete insReg;
	delete mem;
	delete A;
	delete B;
	delete flags;
	delete IC;
	delete addressBus;
	delete dataBus;
}
void CPU::fetchInstruction(){
	IC->writeToAddressBus();
	mem->writeToDataBus();
	insReg->opCode->readFromDataBus();
}

bool CPU::decode(){
	if ( settings.printOpCodeAddresses ) {
		cout << "( ";
		settings.printAddress( &IC->byte );
		cout << " ) ";
	}

	OpCodes opcode = (OpCodes)insReg->opCode->byte.to_ulong();

	if ( opcode == STOP ) {
		cout << "Stop Code" << endl;
		dataBus->clear();
		addressBus->clear();
		return false;
	}

	Instruction I = instructionSet[opcode];

	//fetch opcodes
	if ( settings.printOpCodes )
		cout << I.opCode;

	if ( I.operands >= unary ) {
		IC->increment();
		IC->writeToAddressBus();
		mem->writeToDataBus();
		insReg->operand1->readFromDataBus();
		if ( settings.printOperands ) {
			cout << " ";
			settings.printOperand( &insReg->operand1->byte );
		}
	}
	if ( I.operands >= binary ) {
		IC->increment();
		IC->writeToAddressBus();
		mem->writeToDataBus();
		insReg->operand2->readFromDataBus();
		if ( settings.printOperands ) {
			cout << " ";
			settings.printOperand( &insReg->operand2->byte );
		}
	}
	if ( I.operands == ternary ) {
		IC->increment();
		IC->writeToAddressBus();
		mem->writeToDataBus();
		insReg->operand3->readFromDataBus();
		if ( settings.printOperands ) {
			cout << " ";
			settings.printOperand( &insReg->operand3->byte );
		}
	}
	cout << endl;

	for ( unsigned char microIns : I.operations )
		switch ( microIns ) {
			case Mem_readByte:
				mem->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printData( &dataBus->byte );
					cout << " from memory address ";
					settings.printAddress( &addressBus->byte );
					cout << endl;
				}
				break;

			case Mem_writeByte:
				mem->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " to memory address ";
					settings.printAddress( &addressBus->byte );
					cout << endl;
				}
				break;

			case Mem_readAddress:
				mem->writeToAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read Address ";
					settings.printAddress( &addressBus->byte );
					cout << " from memory" << endl;
				}
				break;

			case IC_increment:
				IC->increment();
				break;

			case A_readFromBus:
				A->readFromDataBus();
				flags->Zero() = A->byte.to_ulong() == 0;
				flags->Neg() = A->byte[7];
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus into Register A" << endl;
				}
				break;

			case B_readFromBus:
				B->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus into Register B" << endl;
				}
				break;

			case IC_readFromBus:
				IC->readFromAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printAddress( &addressBus->byte );
					cout << " from dataBus into Instruction Counter" << endl;
				}
				break;

			case A_writeToBus:
				A->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from Register A to dataBus" << endl;
				}
				break;

			case B_writeToBus:
				B->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from Register B to dataBus" << endl;
				}
				break;

			case IC_writeToBus:
				IC->writeToAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printAddress( &addressBus->byte );
					cout << " from Instruction Counter to dataBus" << endl;
				}
				break;

			case A_addFromBus:
				flags->Carry() = A->byte.to_ulong() + dataBus->byte.to_ulong()
						>= 255;
				A->addFromDataBus();
				flags->Zero() = A->byte.to_ulong() == 0;
				flags->Neg() = A->byte[7];
				if ( settings.printMicroInstructions ) {
					cout << "	added ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus to Register A, result: ";
					settings.printData( &A->byte );
					cout << endl;
				}
				break;

			case IC_addFromBus:
				IC->addFromAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	added ";
					settings.printAddress( &addressBus->byte );
					cout << " from addressBus to Instruction Counter, result: ";
					settings.printAddress( &IC->byte );
					cout << endl;
				}
				break;

			case A_subFromBus:
				flags->Carry() = (int)A->byte.to_ulong()
						- (int)dataBus->byte.to_ulong() <= 0;
				A->subFromDataBus();
				flags->Zero() = A->byte.to_ulong() == 0;
				flags->Neg() = A->byte[7];
				if ( settings.printMicroInstructions ) {
					cout << "	subtracted ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus to Register A, result: ";
					settings.printData( &A->byte );
					cout << endl;
				}
				break;

			case IC_subFromBus:
				IC->subFromAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	subtracted ";
					settings.printAddress( &addressBus->byte );
					cout << " from addressBus to Instruction Counter, result: ";
					settings.printAddress( &IC->byte );
					cout << endl;
				}
				break;

			case IC_toDataBus:
				IC->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printAddress( &dataBus->byte );
					cout << " from InstructionCounter Register to dataBus"
							<< endl;
				}
				break;

			case IC_fromDataBus:
				IC->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printAddress( &dataBus->byte );
					cout << " from dataBus into the InstructionCounter Register"
							<< endl;
				}
				break;

			case SR_toDataBus:
				flags->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from the Status Register to dataBus" << endl;
				}
				break;

			case SR_fromDataBus:
				flags->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus into the Status Register" << endl;
				}
				break;

			case operand1_toAddBus:
				insReg->operand1->writeToAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printAddress( &addressBus->byte );
					cout << " from Operand Register 1 to addressBus" << endl;
				}
				break;

			case operand1_toDataBus:
				insReg->operand1->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from Operand Register 1 to dataBus" << endl;
				}
				break;

			case operand2_toAddBus:
				insReg->operand2->writeToAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printAddress( &addressBus->byte );
					cout << " from Operand Register 2 to addressBus" << endl;
				}
				break;

			case operand2_toDataBus:
				insReg->operand2->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from Operand Register 2 to dataBus" << endl;
				}
				break;

			case operand3_toAddBus:
				insReg->operand3->writeToAddressBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printAddress( &addressBus->byte );
					cout << " from Operand Register 3 to addressBus" << endl;
				}
				break;

			case operand3_toDataBus:
				insReg->operand3->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	wrote ";
					settings.printData( &dataBus->byte );
					cout << " from Operand Register 3 to dataBus" << endl;
				}
				break;

			case print:
				cout << "	";
				settings.printData( &dataBus->byte );
				cout << endl;
				break;

			case get_ZeroFlag:
				insReg->jmp = flags->Zero();
				if ( settings.printMicroInstructions ) {
					cout << "	read Zero flag as ";
					settings.printBool( this->insReg->jmp );
					cout << endl;
				}
				break;

			case get_CarryFlag:
				insReg->jmp = flags->Carry();
				if ( settings.printMicroInstructions ) {
					cout << "	read Carry flag as ";
					settings.printBool( this->insReg->jmp );
					cout << endl;
				}
				break;

			case get_NegativeFlag:
				insReg->jmp = flags->Neg();
				if ( settings.printMicroInstructions ) {
					cout << "	read Negative flag as ";
					settings.printBool( this->insReg->jmp );
					cout << endl;
				}
				break;

			case invert_TestFlag:
				insReg->jmp = !insReg->jmp;
				if ( settings.printMicroInstructions ) {
					cout << "	inverted jump flag to ";
					settings.printBool( this->insReg->jmp );
					cout << endl;
				}
				break;

			case jmp_Test:
				if ( insReg->jmp ) {
					insReg->operand1->writeToAddressBus();
					IC->readFromAddressBus();
					if ( settings.printMicroInstructions )
						cout << "	jumped to ";
				}
				else {
					IC->increment();
					if ( settings.printMicroInstructions )
						cout << "	jump condition not met, proceeding at ";
				}
				if ( settings.printMicroInstructions ) {
					settings.printAddress( &IC->byte );
					cout << endl;
				}
				break;

			case SP_set:
				SP->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	read ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus into the StackPointer Register"
							<< endl;
				}
				break;

			case stack_push:
				SP->decrement();
				SP->writeToAddressBus();
				mem->readFromDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	pushed ";
					settings.printData( &dataBus->byte );
					cout << " from dataBus onto the Stack at Address ";
					settings.printAddress( &addressBus->byte );
					cout << endl;
				}
				break;

			case stack_pop:
				SP->writeToAddressBus();
				mem->writeToDataBus();
				if ( settings.printMicroInstructions ) {
					cout << "	popped ";
					settings.printData( &dataBus->byte );
					cout << " from the Stack at Address ";
					settings.printAddress( &addressBus->byte );
					cout << " onto the dataBus" << endl;
				}
				SP->increment();
				break;

			default:
				cout << hex << "invalid microcode 0x"
						<< (unsigned short int)microIns << endl;
				return false;
				break;
		}
	dataBus->clear();
	addressBus->clear();
	return true;
}

void CPU::run(){
	do {
		fetchInstruction();
	} while ( decode() );
}
}
