#ifndef CHIP_8_H
#define CHIP_8_H

//C standard
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

u_int16_t fetch();
void skip();
void update_screen();

void decode(u_int16_t op);
void decode_0xxx(u_int16_t op);
void decode_8xxx(u_int16_t op);
void decode_Exxx(u_int16_t op);
void decode_Fxxx(u_int16_t op);

void op_00E0();
void op_00EE();
void op_1NNN(int nnn);
void op_2NNN(int nnn);
void op_3XNN(int x, int nn);
void op_4XNN(int x, int nn);
void op_5XY0(int x, int y);
void op_6XNN(int x, int nn);
void op_7XNN(int x, int nn);
void op_8XY0(int x, int y);
void op_8XY1(int x, int y);
void op_8XY2(int x, int y);
void op_8XY3(int x, int y);
void op_8XY4(int x, int y);
void op_8XY5(int x, int y);
void op_8XY6(int x, int y);
void op_8XY7(int x, int y);
void op_8XYE(int x, int y);
void op_9XY0(int x, int y);
void op_ANNN(int nnn);
void op_BNNN(int nnn);
void op_CXNN(int x, int nn);
void op_DXYN(int x, int y, int n);
void op_EX9E(int x);
void op_EXA1(int x);
void op_FX07(int x);
void op_FX0A(int x);
void op_FX15(int x);
void op_FX18(int x);
void op_FX1E(int x);
void op_FX29(int x);
void op_FX33(int x);
void op_FX55(int x);
void op_FX65(int x);

#endif