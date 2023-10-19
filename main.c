#include "moves.h"
#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int num = 0;

PlayerState player[2] = {{ .castle_flags = 0, .r = 0, .n = 0, .b = 0, .k = 0, .q = 0, .p = 0 },
						 { .castle_flags = 0, .r = 0, .n = 0, .b = 0, .k = 0, .q = 0, .p = 0 }};
Pos ep_square = 0;
PlayerColor CurrentPlayer = 0;
PlayerColor nonCurrentPlayer = 0;
Mode mode = 0;

Bool still_good = TRUE;

int main(int argc, char const *argv[]) {
  	if(argc != 4) return 0;

  	if(*argv[3] == '1'){
  		mode = 1;
  	}
  	else if(*argv[3] == '2'){
  		mode = 2;
  	}
  	else{
  		printf("not valid mode\n");
  	}

  	FILE *fptr;
  	if(!(fptr = fopen(argv[1], "r"))){
  		printf("not valid file\n");
  		return 0;
  	}

  	FILE *out;
  	if(!(out = fopen(argv[2], "w"))){
  		printf("not valid output file\n");
  		return 0;
  	}

  	int i;
  	int counter;

    Move * right_move;

  	Board W_K = 0;
  	Board W_Q = 0;
  	Board W_R = 0;
  	Board W_B = 0;
  	Board W_N = 0;
  	Board W_P = 0;
  	Board B_K = 0;
  	Board B_Q = 0;
  	Board B_R = 0;
  	Board B_B = 0;
  	Board B_N = 0;
  	Board B_P = 0;

  	player[WHITE].k = W_K;
  	player[WHITE].q = W_Q;
  	player[WHITE].r = W_R;
  	player[WHITE].b = W_B;
  	player[WHITE].n = W_N;
  	player[WHITE].p = W_P;
  	player[BLACK].k = B_K;
  	player[BLACK].q = B_Q;
  	player[BLACK].r = B_R;
  	player[BLACK].b = B_B;
  	player[BLACK].n = B_N;
  	player[BLACK].p = B_P;

  	char full_line [100];
  	char solution_unchanged [100];

  	while(fgets(full_line, 100, fptr)){
  		fgets(solution_unchanged, 100, fptr);

  		char *layout;
  		char *turn;
  		char *castle;
  		char *ep;
  		/*char *solution;*/

  		layout = strtok(full_line, " \n");
  		turn = strtok(NULL, " \n");
  		castle = strtok(NULL, " \n");
  		ep = strtok(NULL, " \n");
  		/*solution = strtok(solution_unchanged, "\n");*/

  		counter = 0;

  		for(i = 0; i < strlen(layout); i++){
  			if((char)layout[i] != '/'){
  				if(isdigit((char)layout[i])){
  					counter+=(layout[i] - '0');
  				}
  				else{
  					if((char)layout[i] == 'k'){
  						SET_BIT(player[BLACK].k, counter);
  					}
  					if((char)layout[i] == 'q'){
  						SET_BIT(player[BLACK].q, counter);
  					}
  					if((char)layout[i] == 'r'){
  						SET_BIT(player[BLACK].r, counter);
  					}
  					if((char)layout[i] == 'b'){
  						SET_BIT(player[BLACK].b, counter);
  					}
  					if((char)layout[i] == 'n'){
  						SET_BIT(player[BLACK].n, counter);
  					}
  					if((char)layout[i] == 'p'){
  						SET_BIT(player[BLACK].p, counter);
  					}

  					if((char)layout[i] == 'K'){
  						SET_BIT(player[WHITE].k, counter);
  					}
  					if((char)layout[i] == 'Q'){
  						SET_BIT(player[WHITE].q, counter);
  					}
  					if((char)layout[i] == 'R'){
  						SET_BIT(player[WHITE].r, counter);
  					}
  					if((char)layout[i] == 'B'){
  						SET_BIT(player[WHITE].b, counter);
  					}
  					if((char)layout[i] == 'N'){
  						SET_BIT(player[WHITE].n, counter);
  					}
  					if((char)layout[i] == 'P'){
  						SET_BIT(player[WHITE].p, counter);
  					}

  					counter++;
  				}
  			}
  		}

  		if((char)turn[0] == 'w'){
  			CurrentPlayer = WHITE;
  			nonCurrentPlayer = BLACK;
  		}
  		else{
  			CurrentPlayer = BLACK;
  			nonCurrentPlayer = WHITE;
  		}

  		int temp_castle_white = 0;
  		int temp_castle_black = 0;
  		for(i = 0; i < strlen(castle); i++){
  			if((char)castle[i] == 'k'){
  				temp_castle_black += 1;
  			}
  			if((char)castle[i] == 'q'){
  				temp_castle_black += 2;
  			}
  			if((char)castle[i] == 'K'){
 				temp_castle_white += 1;
  			}
  			if((char)castle[i] == 'Q'){
  				temp_castle_white += 2;
  			}
  		}
  		player[WHITE].castle_flags = temp_castle_white;
  		player[BLACK].castle_flags = temp_castle_black;

  		if((char)ep[0] == '-'){
  			ep_square = UNKNOWN_POS;
  		}
  		else{
  			ep_square = TO_POS(ep[0], ep[1]);
  		}

  		if(mode == 1){
  			Move m = { .from = 0, .to = 0, .piece = 0, .promotion_choice = 0, .next_move = NULL };
  			Move *i_m = &m;
  			Move **move_list = &i_m;
  			unsigned int count = 0;
        	unsigned int *pcount = &count;
  			legal_moves(move_list, CurrentPlayer, pcount);
        	Move *index = *move_list;
  			while(index->piece != UNKNOWN){
          		if(can_reach_king_2(index, CurrentPlayer, nonCurrentPlayer) == TRUE){
            		right_move = index;
            		break;
          		}
  				index = index->next_move;
  			}

  			fprintf(out, "\n%c%c-%c%c\n", FILE_OF(right_move->from), RANK_OF(right_move->from), FILE_OF(right_move->to), RANK_OF(right_move->to));
  			Move *temp = *move_list;
  			Move *temp2;
  			while(temp->piece != UNKNOWN){
  				temp2 = temp->next_move;
  				free(temp);
  				temp = temp2;
  			}
  			free(temp2);
  		}
  		else if(mode == 2){
  			Move m = { .from = 0, .to = 0, .piece = 0, .promotion_choice = 0, .next_move = NULL };
  			Move *i_m = &m;
  			Move **move_list = &i_m;
  			unsigned int count = 0;
        	unsigned int *pcount = &count;
        	legal_moves(move_list, CurrentPlayer, pcount);
        	Move *index = *move_list;
        	while(index->piece != UNKNOWN){
        		still_good = TRUE;
        		Move m2 = { .from = 0, .to = 0, .piece = 0, .promotion_choice = 0, .next_move = NULL };
  				Move *i_m2 = &m2;
  				Move **move_list2 = &i_m2;
  				unsigned int count2 = 0;
  				unsigned int *pcount2 = &count2;

  				Piece p_c = index->piece;							/*piece being moved for c*/
				Piece p_nonC;										/*piece captured by c from nonC -- IF APPLICABLE*/
				Board orig_board_c;									/*original board for piece move->piece for color c*/
				Board orig_board_nonC;								/*original board for captured piece for color nonC -- IF APPLICABLE*/
				int piece_captured = 0;								/*0 if no piece captured, 1 if piece captured*/
				Pos pawn_spot = UNKNOWN_POS;
				
				if(IS_SET(player[CurrentPlayer].p, index->from) && p_c != PAWN){
					pawn_spot = index->from;
					RESET_BIT(player[CurrentPlayer].p, index->from);
				}

				if(p_c == KING){
					orig_board_c = player[CurrentPlayer].k;
					RESET_BIT(player[CurrentPlayer].k, index->from);
					SET_BIT(player[CurrentPlayer].k, index->to);
				}
				else if(p_c == QUEEN){
					orig_board_c = player[CurrentPlayer].q;
					RESET_BIT(player[CurrentPlayer].q, index->from);
					SET_BIT(player[CurrentPlayer].q, index->to);
				}
				else if(p_c == NIGHT){
					orig_board_c = player[CurrentPlayer].n;
					RESET_BIT(player[CurrentPlayer].n, index->from);
					SET_BIT(player[CurrentPlayer].n, index->to);
				}
				else if(p_c == BISHOP){
					orig_board_c = player[CurrentPlayer].b;
					RESET_BIT(player[CurrentPlayer].b, index->from);
					SET_BIT(player[CurrentPlayer].b, index->to);
				}
				else if(p_c == ROOK){
					orig_board_c = player[CurrentPlayer].r;
					RESET_BIT(player[CurrentPlayer].r, index->from);
					SET_BIT(player[CurrentPlayer].r, index->to);
				}
				else if(p_c == PAWN){
					orig_board_c = player[CurrentPlayer].p;
					RESET_BIT(player[CurrentPlayer].p, index->from);
					SET_BIT(player[CurrentPlayer].p, index->to);
				}
				else{
					/*printf("\n");*/
				}

				if(IS_SET(BOARD(player[nonCurrentPlayer]), index->to)){
					if(IS_SET(player[nonCurrentPlayer].k, index->to)){
						/*no point, that means king captured*/
					}
					else if(IS_SET(player[nonCurrentPlayer].q, index->to)){
						p_nonC = QUEEN;
						orig_board_nonC = player[nonCurrentPlayer].q;
						RESET_BIT(player[nonCurrentPlayer].q, index->to);
						piece_captured = 1;
					}
					else if(IS_SET(player[nonCurrentPlayer].r, index->to)){
						p_nonC = ROOK;
						orig_board_nonC = player[nonCurrentPlayer].r;
						RESET_BIT(player[nonCurrentPlayer].r, index->to);
						piece_captured = 1;
					}
					else if(IS_SET(player[nonCurrentPlayer].b, index->to)){
						p_nonC = BISHOP;
						orig_board_nonC = player[nonCurrentPlayer].b;
						RESET_BIT(player[nonCurrentPlayer].b, index->to);
						piece_captured = 1;
					}
					else if(IS_SET(player[nonCurrentPlayer].n, index->to)){
						p_nonC = NIGHT;
						orig_board_nonC = player[nonCurrentPlayer].n;
						RESET_BIT(player[nonCurrentPlayer].n, index->to);
						piece_captured = 1;
					}
					else if(IS_SET(player[nonCurrentPlayer].p, index->to)){
						p_nonC = PAWN;
						orig_board_nonC = player[nonCurrentPlayer].p;
						RESET_BIT(player[nonCurrentPlayer].p, index->to);
						piece_captured = 1;
					}
					else{
						printf("\n");
					}
				}
				legal_moves(move_list2, nonCurrentPlayer, pcount2);
				if(*pcount2 != 0){
					Move *index2 = *move_list2;
	        		while(index2->piece != UNKNOWN){
	        			Move m3 = { .from = 0, .to = 0, .piece = 0, .promotion_choice = 0, .next_move = NULL };
	  					Move *i_m3 = &m3;
	  					Move **move_list3 = &i_m3;
	  					unsigned int count3 = 0;
	  					unsigned int *pcount3 = &count3;
		
	  					Piece p_c2 = index2->piece;							/*piece being moved for c*/
						Piece p_nonC2;										/*piece captured by c from nonC -- IF APPLICABLE*/
						Board orig_board_c2;									/*original board for piece move->piece for color c*/
						Board orig_board_nonC2;								/*original board for captured piece for color nonC -- IF APPLICABLE*/
						int piece_captured2 = 0;								/*0 if no piece captured, 1 if piece captured*/
						Pos pawn_spot2 = UNKNOWN_POS;
						
						if(IS_SET(player[nonCurrentPlayer].p, index2->from) && p_c != PAWN){
							pawn_spot2 = index2->from;
							RESET_BIT(player[nonCurrentPlayer].p, index2->from);
						}

						if(p_c2 == KING){
							orig_board_c2 = player[nonCurrentPlayer].k;
							RESET_BIT(player[nonCurrentPlayer].k, index2->from);
							SET_BIT(player[nonCurrentPlayer].k, index2->to);
						}
						else if(p_c2 == QUEEN){
							orig_board_c2 = player[nonCurrentPlayer].q;
							RESET_BIT(player[nonCurrentPlayer].q, index2->from);
							SET_BIT(player[nonCurrentPlayer].q, index2->to);
						}
						else if(p_c2 == NIGHT){
							orig_board_c2 = player[nonCurrentPlayer].n;
							RESET_BIT(player[nonCurrentPlayer].n, index2->from);
							SET_BIT(player[nonCurrentPlayer].n, index2->to);
						}
						else if(p_c2 == BISHOP){
							orig_board_c2 = player[nonCurrentPlayer].b;
							RESET_BIT(player[nonCurrentPlayer].b, index2->from);
							SET_BIT(player[nonCurrentPlayer].b, index2->to);
						}
						else if(p_c2 == ROOK){
							orig_board_c2 = player[nonCurrentPlayer].r;
							RESET_BIT(player[nonCurrentPlayer].r, index2->from);
							SET_BIT(player[nonCurrentPlayer].r, index2->to);
						}
						else if(p_c2 == PAWN){
							orig_board_c2 = player[nonCurrentPlayer].p;
							RESET_BIT(player[nonCurrentPlayer].p, index2->from);
							SET_BIT(player[nonCurrentPlayer].p, index2->to);
						}
						else{
							/*printf("\n");*/
						}
		
						if(IS_SET(BOARD(player[CurrentPlayer]), index2->to)){
							if(IS_SET(player[CurrentPlayer].k, index2->to)){
								/*no point, that means king captured*/
							}
							else if(IS_SET(player[CurrentPlayer].q, index2->to)){
								p_nonC2 = QUEEN;
								orig_board_nonC2 = player[CurrentPlayer].q;
								RESET_BIT(player[CurrentPlayer].q, index2->to);
								piece_captured2 = 1;
							}
							else if(IS_SET(player[CurrentPlayer].r, index2->to)){
								p_nonC2 = ROOK;
								orig_board_nonC2 = player[CurrentPlayer].r;
								RESET_BIT(player[CurrentPlayer].r, index2->to);
								piece_captured2 = 1;
							}
							else if(IS_SET(player[CurrentPlayer].b, index2->to)){
								p_nonC2 = BISHOP;
								orig_board_nonC2 = player[CurrentPlayer].b;
								RESET_BIT(player[CurrentPlayer].b, index2->to);
								piece_captured2 = 1;
							}
							else if(IS_SET(player[CurrentPlayer].n, index2->to)){
								p_nonC2 = NIGHT;
								orig_board_nonC2 = player[CurrentPlayer].n;
								RESET_BIT(player[CurrentPlayer].n, index2->to);
								piece_captured2 = 1;
							}
							else if(IS_SET(player[CurrentPlayer].p, index2->to)){
								p_nonC2 = PAWN;
								orig_board_nonC2 = player[CurrentPlayer].p;
								RESET_BIT(player[CurrentPlayer].p, index2->to);
								piece_captured2 = 1;
							}
							else{
								/*printf("\n");*/
							}
						}
						legal_moves(move_list3, CurrentPlayer, pcount3);
						Move *index3 = *move_list3;
						while(index3->piece != UNKNOWN){
							if(can_reach_king_2(index3, CurrentPlayer, nonCurrentPlayer) == TRUE){
	            				break;
	          				}
	          				index3 = index3->next_move;
						}
						if(index3->piece == UNKNOWN){
							still_good = FALSE;
						}

						reset_changed_boards(orig_board_c2, orig_board_nonC2, p_c2, p_nonC2, piece_captured2, nonCurrentPlayer, CurrentPlayer, pawn_spot2);
	        			index2 = index2->next_move;
	        			index3 = *move_list3;
  						Move *temp;
  						while(index3->piece != UNKNOWN){
  							temp = index3->next_move;
  							free(index3);
  							index3 = temp;
  						}
	        			if(still_good == FALSE){
	        				break;
	        			}
	        		}
	        		reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, CurrentPlayer, nonCurrentPlayer, pawn_spot);
	        		if(still_good == TRUE){
	        			fprintf(out, "%c%c-%c%c\n\n", FILE_OF(index->from), RANK_OF(index->from), FILE_OF(index->to), RANK_OF(index->to));
	        			
	        			index->next_move->piece = UNKNOWN;
	        			num++;
        			}
        			
        		}
        		else{
        			reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, CurrentPlayer, nonCurrentPlayer, pawn_spot);
        		}

        		index = index->next_move;
        		Move *index2 = *move_list2;
  				Move *temp;
  				while(index2->piece != UNKNOWN){
  					temp = index2->next_move;
  					free(index2);
 					index2 = temp;
  				}
        	}
        	index = *move_list;
  			Move *temp;
  			while(index->piece != UNKNOWN){
  				temp = index->next_move;
  				free(index);
  				index = temp;
  			}
  		}

  		else{
  			printf("Invalid mode\n");
  		}

  		player[0].castle_flags = 0;
  		player[0].r = 0;
  		player[0].n = 0;
  		player[0].b = 0;
  		player[0].k = 0;
  		player[0].q = 0;
  		player[0].p = 0;
  		player[1].castle_flags = 0;
  		player[1].r = 0;
  		player[1].n = 0;
  		player[1].b = 0;
  		player[1].k = 0;
  		player[1].q = 0;
  		player[1].p = 0;
		ep_square = 0;
		CurrentPlayer = 0;
  	}

  	fclose(fptr);
  	fclose(out);

  	return 0;
}
