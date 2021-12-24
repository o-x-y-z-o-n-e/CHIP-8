#include "chip_8.h"

#define WIDTH 64
#define HEIGHT 32
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define REGISTERS 16

//display matrix
bool display[WIDTH][HEIGHT];

//RAM
u_int8_t memory[MEMORY_SIZE];

//program counter
u_int16_t pc;

//stack & counter
u_int16_t stack[STACK_SIZE];
u_int8_t sc;

//delay halts
u_int8_t delay;
u_int8_t sound;

//registers
u_int16_t i;
u_int8_t registers[REGISTERS];

//font (in hex, aka. 0 to F). 4x5 characters.
u_int8_t font[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

//

int main(int argc, int* argv[]) {
	pc = 0x200;
	
	u_int8_t t1 = 32;
	u_int8_t t2 = 64;
	u_int8_t test = t1 - t2;
	
	printf("TEST: %u\n", 65 > 300);
	return 0;
	
	bool running = true;
	while(running) {
		u_int16_t opcode = fetch();
		
		decode(opcode);
	}
	
	printf("Done!\n");
	
	return 0;
}



u_int16_t fetch() {
	u_int16_t op = ((uint16_t)memory[pc] << 8) | memory[pc+1];
	pc += 2;
	return op;
}


void skip() {
	pc += 2;
}


void update_screen() {
	//NOTE: Find good curses library/method
}


//opcode execute functions.

void op_00E0() {
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < HEIGHT; y++) {
			display[x][y] = false;
		}
	}
	
	update_screen();
}

void op_00EE() {
	sc--;
	pc = stack[sc];
	stack[sc] = 0;
}

void op_1NNN(int nnn) {
	pc = nnn;
}

void op_2NNN(int nnn) {
	stack[sc] = pc;
	sc++;
	pc == nnn;
}

void op_3XNN(int x, int nn) {
	if(registers[x] == nn) skip();
}

void op_4XNN(int x, int nn) {
	if(registers[x] != nn) skip();
}

void op_5XY0(int x, int y) {
	if(registers[x] == registers[y]) skip();
}

void op_6XNN(int x, int nn) {
	registers[x] = nn;
}

void op_7XNN(int x, int nn) {
	registers[x] += nn;
}

void op_8XY0(int x, int y) {
	registers[x] = registers[y];
}

void op_8XY1(int x, int y) {
	registers[x] |= registers[y];
}

void op_8XY2(int x, int y) {
	registers[x] &= registers[y];
}

void op_8XY3(int x, int y) {
	registers[x] ^= registers[y];
}

void op_8XY4(int x, int y) {
	u_int8_t o = registers[x];
	registers[x] += registers[y];
	registers[0xF] = registers[x] < o;
}

void op_8XY5(int x, int y) {
	u_int8_t o = registers[x];
	registers[x] -= registers[y];
	registers[0xF] = registers[x] < o;
}

void op_8XY6(int x, int y) {
	//IMPLEMENT.
}

void op_8XY7(int x, int y) {
	u_int8_t o = registers[y];
	registers[x] = registers[y] - registers[x];
	registers[0xF] = registers[x] < o;
}

void op_8XYE(int x, int y) {
	//IMPLEMENT.
}

void op_9XY0(int x, int y) {
	if(registers[x] != registers[y]) skip();
}

void op_ANNN(int nnn) {
	i = nnn;
}

void op_BNNN(int nnn) {
	pc = registers[0] + nnn;
}

void op_CXNN(int x, int nn) {
	registers[x] = rand() & nn;
}

void op_DXYN(int x, int y, int n) {
	//IMPLEMENT.
}

void op_EX9E(int x) {
	//IMPLEMENT.
}

void op_EXA1(int x) {
	//IMPLEMENT.
}

void op_FX07(int x) {
	registers[x] = delay;
}

void op_FX0A(int x) {
	//IMPLEMENT.
}

void op_FX15(int x) {
	delay = registers[x];
}

void op_FX18(int x) {
	sound = registers[x];
}

void op_FX1E(int x) {
	i += registers[x];
}

void op_FX29(int x) {
	//IMPLEMENT.
}

void op_FX33(int x) {
	//IMPLEMENT.
}

void op_FX55(int x) {
	for(int l = 0; l <= x; l++) {
		memory[i+l] = registers[l];
	}
}

void op_FX65(int x) {
	for(int l = 0; l <= x; l++) {
		registers[l] = memory[i+l];
	}
}