#ifndef TERMINALCONTROL_H
#define TERMINALCONTROL_H

#include "core.h"

/* a list of ncurses codes */
#define CODE_7 55
#define CODE_Y_LOWER 121
#define CODE_8 56
#define CODE_K_LOWER 107
#define CODE_9 57
#define CODE_U_LOWER 117
#define CODE_6 54
#define CODE_L_LOWER 108
#define CODE_3 51
#define CODE_N_LOWER 110
#define CODE_2 50
#define CODE_J_LOWER 106
#define CODE_1 49
#define CODE_B_LOWER 98
#define CODE_4 52
#define CODE_H_LOWER 104
#define CODE_RIGHT_CHEVRON 62
#define CODE_LEFT_CHEVRON 60
#define CODE_5 53
#define CODE_SPACE 32
#define CODE_M_LOWER 109
#define CODE_UP_ARROW 259
#define CODE_DOWN_ARROW 258
#define CODE_ESCAPE 27
#define CODE_Q_LOWER 113
#define CODE_Q_UPPER 81

/* codes for equipment management */
#define CODE_W_LOWER 119
#define CODE_T_LOWER 116
#define CODE_D_LOWER 100
#define CODE_X_LOWER 120
#define CODE_I_LOWER 105
#define CODE_I_UPPER 73
#define CODE_E_LOWER 101

#define TERMINAL_HEIGHT 21
#define TERMINAL_WIDTH 81

void io_init_terminal();
void io_end_terminal();

int read_input();
/* only defined for inputs 0-9 */
int read_numerical_input();
/* only defined for inputs a-z */
int read_alphabetical_input();

/* returns whether "num" is a code */
int is_code(int num);

void set_message(int color, char c);
void set_message(const char* s, int length);

#endif
