/*
 * MicroCode.h
 *
 *  Created on: 27 Jan 2021
 *      Author: Martin Blecker
 */

#pragma once

namespace cpu {
enum microCodes {
	Mem_readByte,
	Mem_writeByte,
	Mem_readAddress,
	IC_increment,
	A_readFromBus,
	B_readFromBus,
	IC_readFromBus,
	A_writeToBus,
	B_writeToBus,
	IC_writeToBus,
	A_addFromBus,
	IC_addFromBus,
	A_subFromBus,
	IC_subFromBus,
	IC_toDataBus,
	IC_fromDataBus,
	SR_toDataBus,
	SR_fromDataBus,
	operand1_toAddBus,
	operand1_toDataBus,
	operand2_toAddBus,
	operand2_toDataBus,
	operand3_toAddBus,
	operand3_toDataBus,
	print,
	get_ZeroFlag,
	get_CarryFlag,
	get_NegativeFlag,
	invert_TestFlag,
	jmp_Test,
	SP_set,
	stack_push,
	stack_pop
};
}
