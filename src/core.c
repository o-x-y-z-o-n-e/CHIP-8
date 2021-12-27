#include "chip_8.h"

#define MEMORY_SIZE 4096
#define KEY_OFFSET 0
#define FONT_OFFSET 16
#define PROGRAM_OFFSET 512
#define STACK_SIZE 16
#define REGISTERS 16

//font (in hex, aka. 0 to F). 4x5 characters.
const u_int8_t FONT[] = {
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

//display matrix
//bool display[DISPLAY_WIDTH][DISPLAY_HEIGHT];

//RAM
u_int8_t memory[MEMORY_SIZE];
u_int8_t display[2048];

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


int init_core() {
	pc = PROGRAM_OFFSET;

	reset_keys();
	load_font();

	return 0;
}



int load_program(const char* file_path) {
	FILE* file = fopen(file_path, "rb");
	if(file == NULL) {
		printf("ERROR: Cannot access file!\n");
		return 1;
	}

	/*
	struct stat st;
	stat(file_path, &st);
	int size = st.st_size;
	*/

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	if(size > MEMORY_SIZE - PROGRAM_OFFSET) {
		printf("ERROR: File is too large!\n");
		fclose(file);
		return 1;
	}

	fread(&memory[PROGRAM_OFFSET], 1, size, file);

	fclose(file);
	
	return 0;
}


void reset_keys() {
	for(int l = 0; l < 0xF; l++) {
		memory[KEY_OFFSET + l] = 0;
	}
}


void load_font() {
	for(int l = 0; l < 5 * 0xF; l++) {
		memory[FONT_OFFSET + l] = FONT[l];
	}
}


u_int16_t fetch() {
	u_int16_t op = ((uint16_t)memory[pc] << 8) | memory[pc+1];
	pc += 2;
	return op;
}


void skip() {
	pc += 2;
}


//opcode execute functions.
void op_00E0() {
	for (int l = 0; l < 2048; l++) {
		display[l] = 0;
	}

	/*
	for(int x = 0; x < DISPLAY_WIDTH; x++) {
		for(int y = 0; y < DISPLAY_HEIGHT; y++) {
			display[x][y] = false;
		}
	}
	*/
	
	//update_screen();
	draw_screen(display);
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
	pc = nnn;
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
	/*
	u_int8_t o = registers[x];
	registers[x] += registers[y];
	registers[0xF] = registers[x] < o;
	*/

	u_int16_t sum = registers[x] + registers[y];

	if(sum > 0xFF) {
		registers[0xF] = 1;
	} else {
		registers[0xF] = 0;
	}

	registers[x] = sum & 0xFF;
}

void op_8XY5(int x, int y) {
	/*
	u_int8_t o = registers[x];
	registers[x] -= registers[y];
	registers[0xF] = registers[x] < o;
	*/

	if(registers[x] > registers[y]) {
		registers[0xF] = 1;
	} else {
		registers[0xF] = 0;
	}

	registers[x] -= registers[y];
}

void op_8XY6(int x, int y) {
	//registers[x] = registers[y];
	registers[0xF] = (registers[x] & 0x01);
	registers[x] >>= 1;
}

void op_8XY7(int x, int y) {
	/*
	u_int8_t o = registers[y];
	registers[x] = registers[y] - registers[x];
	registers[0xF] = registers[x] < o;
	*/

	if(registers[y] > registers[x]) {
		registers[0xF] = 1;
	} else {
		registers[0xF] = 0;
	}

	registers[x] = registers[y] - registers[x];
}

void op_8XYE(int x, int y) {
	//registers[x] = registers[y];
	registers[0xF] = registers[x] & 0x80;
	registers[x] <<= 1;
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
	int px = registers[x] % DISPLAY_WIDTH;
	int py = registers[y] % DISPLAY_HEIGHT;
	
	registers[0xF] = 0;

	for(int r = 0; r < n && py+r < DISPLAY_HEIGHT; r++) {
		for(int c = 0; c < 8 && px+c < DISPLAY_WIDTH; c++) {
			u_int8_t bit = memory[i+r] & (0x80 >> c);
			u_int8_t* pixel = &display[(px + c) + (DISPLAY_WIDTH * (py + r))];
			
			if (bit != 0) {
				if (*pixel != 0) {
					registers[0xF] = 1;
				}

				*pixel ^= 0xFF;
			}
		}
	}

	draw_screen(display);
}

void op_EX9E(int x) {
	if(memory[KEY_OFFSET + registers[x]]) skip();
}

void op_EXA1(int x) {
	if(!memory[KEY_OFFSET + registers[x]]) skip();
}

void op_FX07(int x) {
	registers[x] = delay;
}

void op_FX0A(int x) {
	/*
	char ch = getch();
	switch(ch) {
		case 0x31: { // 1 -> 1
			registers[x] = 0x1;
			break;
		}
		case 0x32: { // 2 -> 2
			registers[x] = 0x2;
			break;
		}
		case 0x33: { // 3 -> 3
			registers[x] = 0x3;
			break;
		}
		case 0x34: { // 4 -> C
			registers[x] = 0xC;
			break;
		}
		case 0x71: { // q -> 4
			registers[x] = 0x4;
			break;
		}
		case 0x77: { // w -> 5
			registers[x] = 0x5;
			break;
		}
		case 0x65: { // e -> 6
			registers[x] = 0x6;
			break;
		}
		case 0x72: { // r -> D
			registers[x] = 0xD;
			break;
		}
		case 0x61: { // a -> 7
			registers[x] = 0x7;
			break;
		}
		case 0x73: { // s -> 8
			registers[x] = 0x8;
			break;
		}
		case 0x64: { // d -> 9
			registers[x] = 0x9;
			break;
		}
		case 0x66: { // f -> E
			registers[x] = 0xE;
			break;
		}
		case 0x7A: { // z -> A
			registers[x] = 0xA;
			break;
		}
		case 0x78: { // x -> 0
			registers[x] = 0x0;
			break;
		}
		case 0x63: { // c -> B
			registers[x] = 0xB;
			break;
		}
		case 0x76: { // v -> F
			registers[x] = 0xF;
			break;
		}
	}
	*/

	for(int l = 0; l < 0xF; l++) {
		if(memory[KEY_OFFSET + l] == 0x1) {
			registers[x] = l;
			return;
		}
	}

	pc -= 2;
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
	i = FONT_OFFSET + ((registers[x] & 0x0F) * 5);
}

void op_FX33(int x) {
	u_int8_t d1 = registers[x] / 100;
	u_int8_t d2 = (registers[x] / 10) % 10;
	u_int8_t d3 = registers[x] % 10;

	memory[i] = d1;
	memory[i+1] = d2;
	memory[i+2] = d3;
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