/*
 * InstructionSetDefinition.h
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */

#pragma once
#include "MicroCodes.h"
#include "OpCodes.h"
#include <array>
#include <string>
#include <vector>
namespace cpu {
enum opType : unsigned char {
	nullary,
	unary,
	binary,
	ternary
};
struct Instruction {
	opType operands = nullary;
	std::string opCode = "STOP";
	std::vector< microCodes > operations { };
	Instruction(){
	}

	Instruction( opType oT, std::string opc, std::vector< microCodes > ops );
};

std::vector< Instruction > buildInstructionSet();
}
