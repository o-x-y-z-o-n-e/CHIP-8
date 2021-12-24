#include "chip_8.h"

u_int16_t NNN;     //(_NNN) 12-bit address
u_int8_t NN;       //(__NN) 8-bit constant
u_int8_t N;        //(___N) 4-bit constant
u_int8_t X;        //(_X__) 4-bit register
u_int8_t Y;        //(__Y_) 4-bit register

void decode(u_int16_t op) {
	NNN = op & 0x0FFF;
	NN = op & 0x00FF;
	N = op & 0x000F;
	X = (op & 0x0F00) >> 12;
	Y = (op & 0x00F0) >> 8;
	
	switch(op & 0xF000) {
		case 0x0000: {
			//(1___)
			decode_0xxx(op);
			break;
		}
		
		case 0x1000: {
			//(1NNN) Jumps to address NNN.
			op_1NNN(NNN);
			break;
		}
		
		case 0x2000: {
			//(2NNN) Calls subroutine at NNN.
			op_2NNN(NNN);
			break;
		}
		
		case 0x3000: {
			//(3XNN) Skips the next instruction if VX equals NN.
			op_3XNN(X, NN);
			break;
		}
		
		case 0x4000: {
			//(4XNN) Skips the next instruction if VX does not equal NN.
			op_4XNN(X, NN);
			break;
		}
		
		case 0x5000: {
			//(5XY0) Skips the next instruction if VX equals VY.
			op_5XY0(X, Y);
			break;
		}
		
		case 0x6000: {
			//(6XNN) Sets VX to NN.
			op_6XNN(X, NN);
			break;
		}
		
		case 0x7000: {
			//(7XNN) Adds NN to VX. (Carry flag is not changed)
			op_7XNN(X, NN);
			break;
		}
		
		case 0x8000: {
			//(8___)
			decode_8xxx(op);
			break;
		}
		
		case 0x9000: {
			//(9XY0) Skips the next instruction if VX does not equal VY.
			op_9XY0(X, Y);
			break;
		}
		
		case 0xA000: {
			//(ANNN) Sets I to the address NNN.
			op_ANNN(NNN);
			break;
		}
		
		case 0xB000: {
			//(BNNN) Jumps to the address NNN plus V0.
			op_BNNN(NNN);
			break;
		}
		
		case 0xC000: {
			//(CXNN) Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			op_CXNN(X, NN);
			break;
		}
		
		case 0xD000: {
			//(DXYN) Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
			op_DXYN(X, Y, N);
			break;
		}
		
		case 0xE000: {
			//(E___)
			decode_Exxx(op);
			break;
		}
		
		case 0xF000: {
			//(F___)
			decode_Fxxx(op);
			break;
		}
	}
}


//


void decode_0xxx(u_int16_t op) {
	switch(op & 0x0FFF) {
		case 0x00E0: {
			//(00E0) Clears the screen.
			op_00E0();
			break;
		}
		case 0x00EE: {
			//(00EE) Returns from a subroutine.
			op_00EE();
			break;
		}
		default: {
			//(0NNN) Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN.
			break;
		}
	}
}


//


void decode_8xxx(u_int16_t op) {
	switch(op & 0x000F) {
		case 0x0000: {
			//(8XY0) Sets VX to the value of VY.
			op_8XY0(X, Y);
			break;
		}
		
		case 0x0001: {
			//(8XY1) Sets VX to VX [OR] VY.
			op_8XY1(X, Y);
			break;
		}
		
		case 0x0002: {
			//(8XY2) Sets VX to VX [AND] VY.
			op_8XY2(X, Y);
			break;
		}
		
		case 0x0003: {
			//(8XY3) Sets VX to VX [XOR] VY.
			op_8XY3(X, Y);
			break;
		}
		
		case 0x0004: {
			//(8XY4) Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
			op_8XY4(X, Y);
			break;
		}
		
		case 0x0005: {
			//(8XY5) VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
			op_8XY5(X, Y);
			break;
		}
		
		case 0x0006: {
			//(8XY6) Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
			op_8XY6(X, Y);
			break;
		}
		
		case 0x0007: {
			//(8XY7) Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
			op_8XY7(X, Y);
			break;
		}
		
		case 0x000E: {
			//(8XYE) Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
			op_8XYE(X, Y);
			break;
		}
	}
}


//


void decode_Exxx(u_int16_t op) {
	switch(op & 0x00FF) {
		case 0x009E: {
			//(EX9E) Skips the next instruction if the key stored in VX is pressed.
			op_EX9E(X);
			break;
		}
		
		case 0x00A1: {
			//(EXA1) Skips the next instruction if the key stored in VX is not pressed.
			op_EXA1(X);
			break;
		}
	}
}


//


void decode_Fxxx(u_int16_t op) {
	switch(op & 0x00FF) {
		case 0x0007: {
			//(FX07) Sets VX to the value of the delay timer.
			op_FX07(X);
			break;
		}
		
		case 0x000A: {
			//(FX0A) A key press is awaited, and then stored in VX.
			op_FX0A(X);
			break;
		}
		
		case 0x0015: {
			//(FX15) Sets the delay timer to VX.
			op_FX15(X);
			break;
		}
		
		case 0x0018: {
			//(FX18) Sets the sound timer to VX.
			op_FX18(X);
			break;
		}
		
		case 0x001E: {
			//(FX1E) Adds VX to I. VF is not affected.
			op_FX1E(X);
			break;
		}
		
		case 0x0029: {
			//(FX29) Sets I to the location of the sprite for the character in VX.
			op_FX29(X);
			break;
		}
		
		case 0x0033: {
			//(FX33) Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
			op_FX33(X);
			break;
		}
		
		case 0x0055: {
			//(FX55) Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			op_FX55(X);
			break;
		}
		
		case 0x0065: {
			//(FX65) Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			op_FX65(X);
			break;
		}
	}
}