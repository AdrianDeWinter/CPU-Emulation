# CPU-Emulation

A C++ processor emulator that will be able to take in any instruction set defined via series of micro-instructions and thus emulate any microprocessor architecture

Current State:
 * Can handle any set of opcodes defined via the existing microcodes, but currently only from InstructionSetDefinition.cpp
 * can use these opcodes to run any program given in hex format
Todo:
 * import programs and opcode definitions from files
 * expand and generalize microcodes to allow broader range of microarchitectures to be emulated