#include "terminalcontrol.h"

void io_init_terminal(){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  if(has_colors()){
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  }
}

void io_end_terminal(){
  endwin();
}

int read_input(){
  int input = getch();

  if(is_code(input)){
    //sprintf(message,"%s %d\n","input:",input);
    return input;
  }

  //sprintf(message,"Not a command\n");
  //sprintf(message,"%s %d\n","input:",input);
  return 0;
}

int read_numerical_input(){
  return getch() - 48;
}

int read_alphabetical_input(){
  return getch() - 97;
}

int is_code(int num){
  if(num > 48 && num < 58)
    return 1;
  if(num == CODE_Y_LOWER|| num == CODE_K_LOWER || num == CODE_U_LOWER || num == CODE_L_LOWER || num == CODE_N_LOWER|| num == CODE_J_LOWER || num == CODE_B_LOWER || num == CODE_H_LOWER || num == CODE_RIGHT_CHEVRON || num == CODE_LEFT_CHEVRON || num == CODE_SPACE || num == CODE_M_LOWER || num == CODE_UP_ARROW || num == CODE_DOWN_ARROW || num == CODE_ESCAPE || num == CODE_Q_LOWER || num == CODE_Q_UPPER || num == CODE_W_LOWER || num == CODE_T_LOWER || num == CODE_D_LOWER || num == CODE_X_LOWER || num == CODE_I_LOWER || num == CODE_I_UPPER || num == CODE_E_LOWER)
    return 1;

  return 0;
}

void set_message(int num, char c){
  sprintf(message,"color: %d symbol: %c",num,c);
  refresh();
}

void set_message(const char* s, int length){
  int i;
  for(i = 0; i < 79 && i < length; i++){
    message[i] = s[i];
  }

  message[i] = '\0';
  refresh();
}
