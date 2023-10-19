#include "io.h"
#include <stdio.h>
#include <string.h>

/*
 * Implement each function in this file.
 */
 
void display_board() {
	int i;
    for(i = 0; i < 64; i++){
  		if(IS_SET(player[BLACK].p, i)){
  			printf(" p ");
  		}
  		else if(IS_SET(player[BLACK].k, i)){
  			printf(" k ");
  		}
  		else if(IS_SET(player[BLACK].q, i)){
  			printf(" q ");
  		}
  		else if(IS_SET(player[BLACK].r, i)){
  			printf(" r ");
  		}
  		else if(IS_SET(player[BLACK].b, i)){
  			printf(" b ");
  		}
  		else if(IS_SET(player[BLACK].n, i)){
  			printf(" n ");
  		}

  		else if(IS_SET(player[WHITE].p, i)){
  			printf(" P ");
  		}
  		else if(IS_SET(player[WHITE].k, i)){
  			printf(" K ");
  		}
  		else if(IS_SET(player[WHITE].q, i)){
  			printf(" Q ");
  		}
  		else if(IS_SET(player[WHITE].r, i)){
  			printf(" R ");
  		}
  		else if(IS_SET(player[WHITE].b, i)){
  			printf(" B ");
  		}
  		else if(IS_SET(player[WHITE].n, i)){
  			printf(" N ");
  		}

  		else{
  			printf(" - ");
  		}
  		if(i%8 == 7){
  			printf("\n");
  		}
  	}
}

/*Bool parse_board(char *board) {
    
}*/

/* Parses the input move. 
 * This function repeatedly prompts for a move until a valid move is typed in. 
 * This function only validates that:
 *	 1) from and to are valid squares on the board.
 *	 2) from and to are separated by a '-' 
 * This function does not check the legality of the move.
 */ 
/*Move parse_move() {
	
}*/

/*Bool check_input(char *m){
	if(strlen(m) != 5 || strcmp(m[2] + "", "-") != 0){
		return FALSE;
	}
	if(	m[0] < 97  ||
		m[0] > 104 ||
		m[3] < 97  ||
		m[3] > 104 ||
		m[1] < 49  ||
		m[1] > 56  ||
		m[4] < 49  ||
		m[4] > 56){
		return FALSE;
	}
	return TRUE;
}*/

/* This function accepts a move from the user. The move is not validated */
/*void prompt_move(Move *m, unsigned int move_number) {
    
}*/

/* This function prints a move on to the console */
void print_move(Move *m) {
    printf("%c%c-%c%c\n", FILE_OF(m->from), RANK_OF(m->from), FILE_OF(m->to), RANK_OF(m->to));
}

