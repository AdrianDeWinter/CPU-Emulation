/*
 * InstructionSetDefinition.cpp
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */
#include "InstructionSetDefinition.h"
namespace cpu {
/*
 Instruction::Instruction( Instruction& i ){
 operands = i.operands;
 operations = i.operations;
 }*/
Instruction::Instruction( opType oT, std::string opc,
		std::vector< microCodes > ops ){
	operands = oT;
	opCode = opc;
	operations = ops;
}
std::vector< Instruction > buildInstructionSet(){
	std::vector< Instruction > instructionSet;

	instructionSet.emplace_back( nullary, "STOP",
			std::vector< microCodes > { } );

	instructionSet.emplace_back( unary, "LDA_immediate",
			std::vector< microCodes > {
					operand1_toDataBus,
					A_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "LDA_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readByte,
					A_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "STA_immediate",
			std::vector< microCodes > {
					operand1_toAddBus,
					A_writeToBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( unary, "STA_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readAddress,
					A_writeToBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( unary, "LDB_immediate",
			std::vector< microCodes > {
					operand1_toDataBus,
					B_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "LDB_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readByte,
					B_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "STB_immediate",
			std::vector< microCodes > {
					operand1_toAddBus,
					B_writeToBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( unary, "STB_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readAddress,
					B_writeToBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( unary, "add_immediate",
			std::vector< microCodes > {
					operand1_toDataBus,
					A_addFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "add_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readByte,
					A_addFromBus,
					IC_increment } );

	instructionSet.emplace_back( nullary, "add_B", std::vector< microCodes > {
			B_writeToBus,
			A_addFromBus,
			IC_increment } );

	instructionSet.emplace_back( unary, "sub_immediate",
			std::vector< microCodes > {
					operand1_toDataBus,
					A_subFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "sub_absolute",
			std::vector< microCodes > {
					operand1_toDataBus,
					Mem_readByte,
					A_subFromBus,
					IC_increment } );

	instructionSet.emplace_back( nullary, "sub_B", std::vector< microCodes > {
			B_writeToBus,
			A_subFromBus,
			IC_increment } );

	instructionSet.emplace_back( binary, "mov_immediate",
			std::vector< microCodes > {
					operand1_toDataBus,
					operand2_toAddBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( binary, "mov_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readByte,
					operand2_toAddBus,
					Mem_writeByte,
					IC_increment } );

	instructionSet.emplace_back( unary, "jmp_immediate",
			std::vector< microCodes > {
					operand1_toAddBus,
					IC_readFromBus } );

	instructionSet.emplace_back( unary, "jmp_absolute",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readAddress,
					IC_readFromBus } );

	instructionSet.emplace_back( unary, "jmp_relativeFwd",
			std::vector< microCodes > {
					operand1_toAddBus,
					IC_addFromBus } );

	instructionSet.emplace_back( unary, "jmp_relativeBkwd",
			std::vector< microCodes > {
					operand1_toAddBus,
					IC_subFromBus } );

	instructionSet.emplace_back( unary, "print_memLoc",
			std::vector< microCodes > {
					operand1_toAddBus,
					Mem_readByte,
					print,
					IC_increment } );

	instructionSet.emplace_back( nullary, "print_A", std::vector< microCodes > {
			A_writeToBus,
			print,
			IC_increment } );

	instructionSet.emplace_back( nullary, "print_B", std::vector< microCodes > {
			B_writeToBus,
			print,
			IC_increment } );

	instructionSet.emplace_back( unary, "BRZ", std::vector< microCodes > {
			get_ZeroFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRNZ", std::vector< microCodes > {
			get_ZeroFlag,
			invert_TestFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRE", std::vector< microCodes > {
			B_writeToBus,
			A_subFromBus,
			get_ZeroFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRNE", std::vector< microCodes > {
			B_writeToBus,
			A_subFromBus,
			get_ZeroFlag,
			invert_TestFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRN", std::vector< microCodes > {
			get_NegativeFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRNN", std::vector< microCodes > {
			get_NegativeFlag,
			invert_TestFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRC", std::vector< microCodes > {
			get_CarryFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "BRNC", std::vector< microCodes > {
			get_CarryFlag,
			invert_TestFlag,
			jmp_Test } );

	instructionSet.emplace_back( unary, "SPI", std::vector< microCodes > {
			operand1_toDataBus,
			SP_set,
			IC_increment } );

	instructionSet.emplace_back( unary, "Stack_push",
			std::vector< microCodes > {
					operand1_toDataBus,
					stack_push,
					IC_increment } );

	instructionSet.emplace_back( nullary, "Stack_push_A",
			std::vector< microCodes > {
					A_writeToBus,
					stack_push,
					IC_increment } );

	instructionSet.emplace_back( nullary, "Stack_push_B",
			std::vector< microCodes > {
					B_writeToBus,
					stack_push,
					IC_increment } );

	instructionSet.emplace_back( unary, "Stack_pop", std::vector< microCodes > {
			stack_pop,
			operand1_toAddBus,
			Mem_writeByte,
			IC_increment } );

	instructionSet.emplace_back( nullary, "Stack_pop_A",
			std::vector< microCodes > {
					stack_pop,
					A_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( nullary, "Stack_pop_B",
			std::vector< microCodes > {
					stack_pop,
					B_readFromBus,
					IC_increment } );

	instructionSet.emplace_back( unary, "JSR", std::vector< microCodes > {
			IC_increment,
			IC_toDataBus,
			stack_push,
			A_writeToBus,
			stack_push,
			B_writeToBus,
			stack_push,
			SR_toDataBus,
			operand1_toAddBus,
			IC_readFromBus } );

	instructionSet.emplace_back( nullary, "RET", std::vector< microCodes > {
			stack_pop,
			IC_fromDataBus,
			stack_pop,
			A_readFromBus,
			stack_pop,
			B_readFromBus,
			stack_pop,
			SR_fromDataBus } );

	return instructionSet;
}
}
