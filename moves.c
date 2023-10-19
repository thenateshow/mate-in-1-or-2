#include "moves.h"
#include "chess.h"
#include <stdio.h>
#include <stdlib.h>

Move *make_new_move(){
	Move *temp_ptr = malloc(sizeof(Move));
	temp_ptr->piece = UNKNOWN;
	return temp_ptr;
}

void add_move_to_list(Move **h, Move *m){
	Move **t = h;
	while(*t){
		t = &(*t)->next_move;
	}
	/*m->next_move = *t;*/
	*t = m;
}

void reset_changed_boards(Board orig_board_c, Board orig_board_nonC, Piece p_c, Piece p_nonC, int piece_captured, PlayerColor c, PlayerColor nonC, Pos pawn_spot){
	if(p_c == KING){
		player[c].k = orig_board_c;
	}
	else if(p_c == QUEEN){
		player[c].q = orig_board_c;
	}
	else if(p_c == ROOK){
		player[c].r = orig_board_c;
	}
	else if(p_c == BISHOP){
		player[c].b = orig_board_c;
	}
	else if(p_c == NIGHT){
		player[c].n = orig_board_c;
	}
	else if(p_c == PAWN){
		player[c].p = orig_board_c;
	}
	else{
		/*printf("ERROR in reset_changed_boards\n");*/
	}

	if(piece_captured == 1){
		if(p_nonC == KING){
			player[nonC].k = orig_board_nonC;
		}
		else if(p_nonC == QUEEN){
			player[nonC].q = orig_board_nonC;
		}
		else if(p_nonC == ROOK){
			player[nonC].r = orig_board_nonC;
		}
		else if(p_nonC == BISHOP){
			player[nonC].b = orig_board_nonC;
		}
		else if(p_nonC == NIGHT){
			player[nonC].n = orig_board_nonC;
		}
		else if(p_nonC == PAWN){
			player[nonC].p = orig_board_nonC;
		}
		else{
			/*printf("ERROR 2 in reset_changed_boards\n");*/
		}
	}

	if(pawn_spot != UNKNOWN_POS){
		SET_BIT(player[c].p, pawn_spot);
	}
}

/*Returns TRUE if the non-CurrentPlayer can reach the CurrentPlayer king location after a given move is made by CurrentPlayer
 *Checking if the move puts CurrentPlayer in check
 *Move m is the potential move
 *PlayerColor c is the color making the move
 *PlayerColor nonC is the opposite color
 */
Bool can_reach_king(Move *m, PlayerColor c, PlayerColor nonC){
	Piece p_c = m->piece;								/*piece being moved for c*/
	Piece p_nonC;										/*piece captured by c from nonC -- IF APPLICABLE*/
	Board orig_board_c;									/*original board for piece move->piece for color c*/
	Board orig_board_nonC;								/*original board for captured piece for color nonC -- IF APPLICABLE*/
	int piece_captured = 0;								/*0 if no piece captured, 1 if piece captured*/
	Pos pawn_spot = UNKNOWN_POS;
	if(p_c == KING){
		orig_board_c = player[c].k;
		RESET_BIT(player[c].k, m->from);
		SET_BIT(player[c].k, m->to);
	}
	else if(p_c == QUEEN){
		orig_board_c = player[c].q;
		RESET_BIT(player[c].q, m->from);
		SET_BIT(player[c].q, m->to);
	}
	else if(p_c == NIGHT){
		orig_board_c = player[c].n;
		RESET_BIT(player[c].n, m->from);
		SET_BIT(player[c].n, m->to);
	}
	else if(p_c == BISHOP){
		orig_board_c = player[c].b;
		RESET_BIT(player[c].b, m->from);
		SET_BIT(player[c].b, m->to);
	}
	else if(p_c == ROOK){
		orig_board_c = player[c].r;
		RESET_BIT(player[c].r, m->from);
		SET_BIT(player[c].r, m->to);
	}
	else if(p_c == PAWN){
		orig_board_c = player[c].p;
		RESET_BIT(player[c].p, m->from);
		SET_BIT(player[c].p, m->to);
	}
	else{
		printf("ERROR in can_reach_king\n");
	}

	if(IS_SET(BOARD(player[nonC]), m->to)){
		if(IS_SET(player[nonC].k, m->to)){
			
		}
		else if(IS_SET(player[nonC].q, m->to)){
			p_nonC = QUEEN;
			orig_board_nonC = player[nonC].q;
			RESET_BIT(player[nonC].q, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].r, m->to)){
			p_nonC = ROOK;
			orig_board_nonC = player[nonC].r;
			RESET_BIT(player[nonC].r, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].b, m->to)){
			p_nonC = BISHOP;
			orig_board_nonC = player[nonC].b;
			RESET_BIT(player[nonC].b, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].n, m->to)){
			p_nonC = NIGHT;
			orig_board_nonC = player[nonC].n;
			RESET_BIT(player[nonC].n, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].p, m->to)){
			p_nonC = PAWN;
			orig_board_nonC = player[nonC].p;
			RESET_BIT(player[nonC].p, m->to);
			piece_captured = 1;
		}
		else{
			printf("ERROR 2 in can_reach_king\n");
		}		/*there is a nonC piece captured*/
	}

	int i;												/*for loop iterator*/
	int king_pos;										/*position of king of color MAKING the move (c)*/
	Board curr = player[c].k;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				king_pos = i;
				i = 64;
			}
		}
	}
	/*check all physically valid moves for nonC, if i = king_pos return TRUE*/
	/*KING*/
	curr = player[nonC].k;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NW*/
				if(	NW_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NW_OF(i))			/*not occupied by same color piece*/
					&& NW_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}	
				/*N*/
				if(	NORTH_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NORTH_OF(i))		/*not occupied by same color piece*/
					&& NORTH_OF(i) == king_pos){					/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*NE*/
				if(	NE_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NE_OF(i))			/*not occupied by same color piece*/
					&& NE_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*W*/
				if(	WEST_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), WEST_OF(i))		/*not occupied by same color piece*/
					&& WEST_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*E*/
				if(	EAST_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), EAST_OF(i))		/*not occupied by same color piece*/
					&& EAST_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*SW*/
				if(	SW_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SW_OF(i))			/*not occupied by same color piece*/
					&& SW_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*S*/
				if(	SOUTH_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SOUTH_OF(i))		/*not occupied by same color piece*/
					&& SOUTH_OF(i) == king_pos){					/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*SE*/
				if(	SE_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SE_OF(i))			/*not occupied by same color piece*/
					&& SE_OF(i) == king_pos){						/*move puts c in check*/
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				i = 64; /*since there is only 1 king, if a king is found the loop can end*/
			}
		}
	}

	/*QUEEN*/
	curr = player[nonC].q;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NW_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NORTH_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NORTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NE_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(WEST_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = WEST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(EAST_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = EAST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SW_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SOUTH_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SOUTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SE_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*ROOK*/
	curr = player[nonC].r;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NORTH_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NORTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(WEST_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = WEST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(EAST_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = EAST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SOUTH_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SOUTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*BISHOP*/
	curr = player[nonC].b;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NW_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NE_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = NE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SW_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[nonC]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SE_OF(temp_pos) == king_pos){					/*can reach king*/
							reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
							return TRUE;
						}
						temp_pos = SE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*NIGHT*/
	curr = player[nonC].n;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NNW*/
				if(	NORTH_OF(NW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& 	!IS_SET(BOARD(player[nonC]), NORTH_OF(NW_OF(i)))		/*not occupied by same color piece*/
				&& 	NORTH_OF(NW_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*NNE*/
				if(	NORTH_OF(NE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), NORTH_OF(NE_OF(i)))		/*not occupied by same color piece*/
				&& 	NORTH_OF(NE_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*WNW*/
				if(	WEST_OF(NW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), WEST_OF(NW_OF(i)))			/*not occupied by same color piece*/
				&& 	WEST_OF(NW_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*ENE*/
				if(	EAST_OF(NE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), EAST_OF(NE_OF(i)))			/*not occupied by same color piece*/
				&& 	EAST_OF(NE_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*WSW*/
				if(	WEST_OF(SW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), WEST_OF(SW_OF(i)))		/*not occupied by same color piece*/
				&& 	WEST_OF(SW_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*ESE*/
				if(	EAST_OF(SE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), EAST_OF(SE_OF(i)))			/*not occupied by same color piece*/
				&& 	EAST_OF(SE_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*SSW*/
				if(	SOUTH_OF(SW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), SOUTH_OF(SW_OF(i)))		/*not occupied by same color piece*/
				&& 	SOUTH_OF(SW_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*SSE*/
				if(	SOUTH_OF(SE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[nonC]), SOUTH_OF(SE_OF(i)))		/*not occupied by same color piece*/
				&& 	SOUTH_OF(SE_OF(i)) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
			}
		}
	}

	/*PAWN*/
	curr = player[nonC].p;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*Capture diagonally*/
				/*WHITE NW*/
				if(	nonC == WHITE									/*checking white pawns*/
					&& NW_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[BLACK]), NW_OF(i))		/*black has a piece on intended square*/
					&& NW_OF(i) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*WHITE NE*/
				if(	nonC == WHITE									/*checking white pawns*/
					&& NE_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[BLACK]), NE_OF(i))		/*black has a piece on intended square*/
					&& NE_OF(i) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*BLACK SW*/
				if(	nonC == BLACK									/*checking white pawns*/
					&& SW_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[WHITE]), SW_OF(i))		/*white has a piece on intended square*/
					&& SW_OF(i) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
				/*BLACK SE*/
				if(	nonC == BLACK									/*checking white pawns*/
					&& SE_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[WHITE]), SE_OF(i))		/*white has a piece on intended square*/
					&& SE_OF(i) == king_pos){
					reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
					return TRUE;
				}
			}
		}
	}

	reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
	return FALSE;
}

Bool can_reach_king_2(Move *m, PlayerColor c, PlayerColor nonC){
	Move **l = &m;
	unsigned int counter = 0;
	unsigned int *count = &counter;

	Piece p_c = m->piece;								/*piece being moved for c*/
	Piece p_nonC;										/*piece captured by c from nonC -- IF APPLICABLE*/
	Board orig_board_c;									/*original board for piece move->piece for color c*/
	Board orig_board_nonC;								/*original board for captured piece for color nonC -- IF APPLICABLE*/
	int piece_captured = 0;								/*0 if no piece captured, 1 if piece captured*/
	Pos pawn_spot = UNKNOWN_POS;

	if(IS_SET(player[c].p, m->from) && p_c != PAWN){
		pawn_spot = m->from;
		RESET_BIT(player[c].p, m->from);
	}

	if(p_c == KING){
		orig_board_c = player[c].k;
		RESET_BIT(player[c].k, m->from);
		SET_BIT(player[c].k, m->to);
	}
	else if(p_c == QUEEN){
		orig_board_c = player[c].q;
		RESET_BIT(player[c].q, m->from);
		SET_BIT(player[c].q, m->to);
	}
	else if(p_c == NIGHT){
		orig_board_c = player[c].n;
		RESET_BIT(player[c].n, m->from);
		SET_BIT(player[c].n, m->to);
	}
	else if(p_c == BISHOP){
		orig_board_c = player[c].b;
		RESET_BIT(player[c].b, m->from);
		SET_BIT(player[c].b, m->to);
	}
	else if(p_c == ROOK){
		orig_board_c = player[c].r;
		RESET_BIT(player[c].r, m->from);
		SET_BIT(player[c].r, m->to);
	}
	else if(p_c == PAWN){
		orig_board_c = player[c].p;
		RESET_BIT(player[c].p, m->from);
		SET_BIT(player[c].p, m->to);
	}
	else{
		printf("ERROR in can_reach_king2\n");
	}

	if(IS_SET(BOARD(player[nonC]), m->to)){
		if(IS_SET(player[nonC].k, m->to)){
			p_nonC = KING;
			orig_board_nonC = player[nonC].k;
			RESET_BIT(player[nonC].k, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].q, m->to)){
			p_nonC = QUEEN;
			orig_board_nonC = player[nonC].q;
			RESET_BIT(player[nonC].q, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].r, m->to)){
			p_nonC = ROOK;
			orig_board_nonC = player[nonC].r;
			RESET_BIT(player[nonC].r, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].b, m->to)){
			p_nonC = BISHOP;
			orig_board_nonC = player[nonC].b;
			RESET_BIT(player[nonC].b, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].n, m->to)){
			p_nonC = NIGHT;
			orig_board_nonC = player[nonC].n;
			RESET_BIT(player[nonC].n, m->to);
			piece_captured = 1;
		}
		else if(IS_SET(player[nonC].p, m->to)){
			p_nonC = PAWN;
			orig_board_nonC = player[nonC].p;
			RESET_BIT(player[nonC].p, m->to);
			piece_captured = 1;
		}
		else{
			printf("ERROR 2 in can_reach_king\n");
		}		/*there is a nonC piece captured*/
	}

	int i;												/*for loop iterator*/
	int king_pos;										/*position of king of color NOT MAKING the move (nonC)*/
	Board curr = player[nonC].k;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				king_pos = i;
				i = 64;
			}
		}
	}
	/*check all physically valid moves for c, if i = king_pos return TRUE*/
	/*KING*/
	curr = player[c].k;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NW*/
				if(	NW_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(i))			/*not occupied by same color piece*/
					&& NW_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}	
				/*N*/
				if(	NORTH_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(i))		/*not occupied by same color piece*/
					&& NORTH_OF(i) == king_pos){					/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*NE*/
				if(	NE_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(i))			/*not occupied by same color piece*/
					&& NE_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*W*/
				if(	WEST_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(i))		/*not occupied by same color piece*/
					&& WEST_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*E*/
				if(	EAST_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(i))		/*not occupied by same color piece*/
					&& EAST_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*SW*/
				if(	SW_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(i))			/*not occupied by same color piece*/
					&& SW_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*S*/
				if(	SOUTH_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(i))		/*not occupied by same color piece*/
					&& SOUTH_OF(i) == king_pos){					/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*SE*/
				if(	SE_OF(i) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(i))			/*not occupied by same color piece*/
					&& SE_OF(i) == king_pos){						/*move puts c in check*/
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				i = 64; /*since there is only 1 king, if a king is found the loop can end*/
			}
		}
	}

	/*QUEEN*/
	curr = player[c].q;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NW_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NORTH_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NORTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NE_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(WEST_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = WEST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(EAST_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = EAST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SW_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SOUTH_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SOUTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SE_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*ROOK*/
	curr = player[c].r;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NORTH_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NORTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(WEST_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = WEST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(EAST_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = EAST_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){					/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SOUTH_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SOUTH_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*BISHOP*/
	curr = player[c].b;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NW_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(NE_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = NE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SW_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SW_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 						/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){						/*square occipied by opposite color piece*/
							cont = FALSE;									/*then don't continue after this*/
						}
						if(SE_OF(temp_pos) == king_pos){					/*can reach king*/
							if(legal_moves(l, nonC, count) == FALSE){
								reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
								return TRUE;
							}
						}
						temp_pos = SE_OF(temp_pos);
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*NIGHT*/
	curr = player[c].n;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NNW*/
				if(	NORTH_OF(NW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& 	!IS_SET(BOARD(player[c]), NORTH_OF(NW_OF(i)))		/*not occupied by same color piece*/
				&& 	NORTH_OF(NW_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*NNE*/
				if(	NORTH_OF(NE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), NORTH_OF(NE_OF(i)))		/*not occupied by same color piece*/
				&& 	NORTH_OF(NE_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*WNW*/
				if(	WEST_OF(NW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), WEST_OF(NW_OF(i)))			/*not occupied by same color piece*/
				&& 	WEST_OF(NW_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*ENE*/
				if(	EAST_OF(NE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), EAST_OF(NE_OF(i)))			/*not occupied by same color piece*/
				&& 	EAST_OF(NE_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*WSW*/
				if(	WEST_OF(SW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), WEST_OF(SW_OF(i)))		/*not occupied by same color piece*/
				&& 	WEST_OF(SW_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*ESE*/
				if(	EAST_OF(SE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), EAST_OF(SE_OF(i)))			/*not occupied by same color piece*/
				&& 	EAST_OF(SE_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*SSW*/
				if(	SOUTH_OF(SW_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), SOUTH_OF(SW_OF(i)))		/*not occupied by same color piece*/
				&& 	SOUTH_OF(SW_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*SSE*/
				if(	SOUTH_OF(SE_OF(i)) != UNKNOWN_POS 					/*on board*/
				&& !IS_SET(BOARD(player[c]), SOUTH_OF(SE_OF(i)))		/*not occupied by same color piece*/
				&& 	SOUTH_OF(SE_OF(i)) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
			}
		}
	}

	/*PAWN*/
	curr = player[c].p;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*Capture diagonally*/
				/*WHITE NW*/
				if(	c == WHITE										/*checking white pawns*/
					&& NW_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[BLACK]), NW_OF(i))		/*black has a piece on intended square*/
					&& NW_OF(i) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*WHITE NE*/
				if(	c == WHITE										/*checking white pawns*/
					&& NE_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[BLACK]), NE_OF(i))		/*black has a piece on intended square*/
					&& NE_OF(i) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*BLACK SW*/
				if(	c == BLACK										/*checking white pawns*/
					&& SW_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[WHITE]), SW_OF(i))		/*white has a piece on intended square*/
					&& SW_OF(i) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
				/*BLACK SE*/
				if(	c == BLACK										/*checking white pawns*/
					&& SE_OF(i) != UNKNOWN_POS						/*on board*/
					&& IS_SET(BOARD(player[WHITE]), SE_OF(i))		/*white has a piece on intended square*/
					&& SE_OF(i) == king_pos){
					if(legal_moves(l, nonC, count) == FALSE){
						reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
						return TRUE;
					}
				}
			}
		}
	}

	reset_changed_boards(orig_board_c, orig_board_nonC, p_c, p_nonC, piece_captured, c, nonC, pawn_spot);
	return FALSE;
}
/* Given a color, this function returns a singly linked list of all legal Moves with the head at *m. 
 * The function returns TRUE if at least 1 legal move is available. 
 * The total number of moves found is stored in the address pointed to by pcount. */
Bool legal_moves(Move **m, PlayerColor c, unsigned int *pcount) {
	PlayerColor nonC;
	if(c == WHITE){
		nonC = BLACK;
	}
	else{
		nonC = WHITE;
	}
	Move *move_first = malloc(sizeof(Move));

	/*Move *next_m = malloc(sizeof(Move));*/

	Move *move = move_first;
	/*move->next_move = next_m;*/
	
	*m = move_first;

	/*first, second, first->next = second*/

	Board curr;
	unsigned int i;
	/*KING*/
	curr = player[c].k;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NW*/
				if(	NW_OF(i) != UNKNOWN_POS 					/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = NW_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}	
				/*N*/
				if(	NORTH_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = NORTH_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*NE*/
				if(	NE_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = NE_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*W*/
				if(	WEST_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = WEST_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*E*/
				if(	EAST_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = EAST_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*SW*/
				if(	SW_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = SW_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*S*/
				if(	SOUTH_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = SOUTH_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*SE*/
				if(	SE_OF(i) != UNKNOWN_POS 				/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(i))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = SE_OF(i);
					move->piece = KING;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				i = 64; /*since there is only 1 king, if a king is found the loop can end*/
			}
		}
	}

	/*QUEEN*/
	curr = player[c].q;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NW_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NORTH_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NE_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = WEST_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = EAST_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SW_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SOUTH_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SE_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*ROOK*/
	curr = player[c].r;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NORTH*/
				while(cont){
					if(	NORTH_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), NORTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(NORTH_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NORTH_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*WEST*/
				while(cont){
					if(	WEST_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), WEST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(WEST_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = WEST_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*EAST*/
				while(cont){
					if(	EAST_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), EAST_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(EAST_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = EAST_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SOUTH*/
				while(cont){
					if(	SOUTH_OF(temp_pos) != UNKNOWN_POS 		/*on board*/
					&& !IS_SET(BOARD(player[c]), SOUTH_OF(temp_pos))){	/*not occupied by same color piece*/
						if(OCCUPIED(SOUTH_OF(temp_pos))){		/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SOUTH_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*BISHOP*/
	curr = player[c].b;
	if(curr != 0){
		Bool cont = TRUE;
		unsigned int temp_pos;
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				cont = TRUE;
				temp_pos = i;
				/*NW*/
				while(cont){
					if(	NW_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), NW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NW_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NW_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*NE*/
				while(cont){
					if(	NE_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), NE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(NE_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = NE_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SW*/
				while(cont){
					if(	SW_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), SW_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SW_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SW_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
				/*SE*/
				while(cont){
					if(	SE_OF(temp_pos) != UNKNOWN_POS 			/*on board*/
					&& !IS_SET(BOARD(player[c]), SE_OF(temp_pos))){		/*not occupied by same color piece*/
						if(OCCUPIED(SE_OF(temp_pos))){			/*square occipied by opposite color piece*/
							cont = FALSE;						/*then don't continue after this*/
						}
						temp_pos = SE_OF(temp_pos);
						move->from = i;
						move->to = temp_pos;
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
					else{
						cont = FALSE;
					}
				}
				cont = TRUE;
				temp_pos = i;
			}
		}
	}

	/*NIGHT*/
	curr = player[c].n;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*NNW*/
				if(	NORTH_OF(NW_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), NORTH_OF(NW_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = NORTH_OF(NW_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*NNE*/
				if(	NORTH_OF(NE_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), NORTH_OF(NE_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = NORTH_OF(NE_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*WNW*/
				if(	WEST_OF(NW_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), WEST_OF(NW_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = WEST_OF(NW_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*ENE*/
				if(	EAST_OF(NE_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), EAST_OF(NE_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = EAST_OF(NE_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*WSW*/
				if(	WEST_OF(SW_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), WEST_OF(SW_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = WEST_OF(SW_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*ESE*/
				if(	EAST_OF(SE_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), EAST_OF(SE_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = EAST_OF(SE_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*SSW*/
				if(	SOUTH_OF(SW_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), SOUTH_OF(SW_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = SOUTH_OF(SW_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*SSE*/
				if(	SOUTH_OF(SE_OF(i)) != UNKNOWN_POS 			/*on board*/
				&& !IS_SET(BOARD(player[c]), SOUTH_OF(SE_OF(i)))){		/*not occupied by same color piece*/
					move->from = i;
					move->to = SOUTH_OF(SE_OF(i));
					move->piece = NIGHT;
					if(can_reach_king(move, c, nonC) == FALSE){						/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
			}
		}
	}

	/*PAWN*/
	curr = player[c].p;
	if(curr != 0){
		for(i = 0; i < 64; i++){
			if(IS_SET(curr, i)){
				/*1 space WHITE*/
				if(	c == WHITE
					&& NORTH_OF(i) != UNKNOWN_POS 											/*on board*/
					&& UNOCCUPIED(NORTH_OF(i))){											/*not occupied by any piece*/
					move->from = i;
					move->to = NORTH_OF(i);
					if((char)RANK_OF(i) == '7'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NORTH_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NORTH_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NORTH_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NORTH_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
				/*1 space BLACK*/
				if(	c == BLACK
					&& SOUTH_OF(i) != UNKNOWN_POS 											/*on board*/
					&& UNOCCUPIED(SOUTH_OF(i))){											/*not occupied by any piece*/
					move->from = i;
					move->to = SOUTH_OF(i);
					if((char)RANK_OF(i) == '2'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SOUTH_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SOUTH_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SOUTH_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SOUTH_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
				/*Move two spaces*/
				if(	c == WHITE																/*checking white pawns*/
					&& (char)RANK_OF(i) == '2'												/*hasn't moved from start - ***MIGHT BE 2****/
					&& UNOCCUPIED(NORTH_OF(i))												/*no piece directly in front of it*/
					&& UNOCCUPIED(NORTH_OF(NORTH_OF(i)))){									/*no piece 2 spots in front of it*/
					move->from = i;
					move->to = NORTH_OF(NORTH_OF(i));
					move->piece = PAWN;
					if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				if(	c == BLACK																/*checking white pawns*/
					&& (char)RANK_OF(i) == '7'												/*hasn't moved from start - ***MIGHT BE 7****/
					&& UNOCCUPIED(SOUTH_OF(i))												/*no piece directly in front of it*/
					&& UNOCCUPIED(SOUTH_OF(SOUTH_OF(i)))){									/*no piece 2 spots in front of it*/
					move->from = i;
					move->to = SOUTH_OF(SOUTH_OF(i));
					move->piece = PAWN;
					if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
						move->next_move = make_new_move();
						move = move->next_move;
						*pcount += 1;
					}
				}
				/*Capture diagonally*/
				/*WHITE NW*/
				if(	c == WHITE																/*checking white pawns*/
					&& NW_OF(i) != UNKNOWN_POS												/*on board*/
					&& (IS_SET(BOARD(player[BLACK]), NW_OF(i)) || NW_OF(i) == ep_square)){	/*black has a piece on intended square*/
					move->from = i;
					move->to = NW_OF(i);
					if((char)RANK_OF(i) == '7'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NW_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NW_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NW_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NW_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
				/*WHITE NE*/
				if(	c == WHITE																/*checking white pawns*/
					&& NE_OF(i) != UNKNOWN_POS												/*on board*/
					&& (IS_SET(BOARD(player[BLACK]), NE_OF(i)) || NE_OF(i) == ep_square)){	/*black has a piece on intended square*/
					move->from = i;
					move->to = NE_OF(i);
					if((char)RANK_OF(i) == '7'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NE_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NE_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NE_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = NE_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
				/*BLACK SW*/
				if(	c == BLACK																/*checking white pawns*/
					&& SW_OF(i) != UNKNOWN_POS												/*on board*/
					&& (IS_SET(BOARD(player[WHITE]), SW_OF(i)) || SW_OF(i) == ep_square)){	/*white has a piece on intended square*/
					move->from = i;
					move->to = SW_OF(i);
					if((char)RANK_OF(i) == '2'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SW_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SW_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SW_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SW_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
				/*BLACK SE*/
				if(	c == BLACK																/*checking white pawns*/
					&& SE_OF(i) != UNKNOWN_POS												/*on board*/
					&& (IS_SET(BOARD(player[WHITE]), SE_OF(i))|| SE_OF(i) == ep_square)){	/*white has a piece on intended square*/
					move->from = i;
					move->to = SE_OF(i);
					if((char)RANK_OF(i) == '2'){
						RESET_BIT(player[c].p, i);
						move->piece = QUEEN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SE_OF(i);
						}
						move->piece = NIGHT;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SE_OF(i);
						}
						move->piece = BISHOP;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SE_OF(i);
						}
						move->piece = ROOK;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
							move->from = i;
							move->to = SE_OF(i);
						}
						SET_BIT(player[c].p, i);
					}
					else{
						move->piece = PAWN;
						if(can_reach_king(move, c, nonC) == FALSE){								/*if move does not put color c in check, it is valid*/
							move->next_move = make_new_move();
							move = move->next_move;
							*pcount += 1;
						}
					}
				}
			}
		}
	}
	move->piece = 0;
	if(*pcount == 0){
		return FALSE;
	}
	else{
		return TRUE;
	}
}

/* Returns TRUE if the CurrentPlayer is under checkmate, FALSE otherwise. */
/*Bool is_checkmate() {
	
}*/

/* Validate a move and make it. Returns TRUE if successful, FALSE if not. 
 *  Error message if any, are stored in msg. 
 * ep_square (if any) is stored in *ep_sq
 */
/*Bool validate_and_move(Move move, char *msg, PlayerColor c, Pos *ep_sq) {
    
}*/

/* Function to decide whether the current position is a draw */
/* Draw can be due to insufficient material, 3 move repetition or stalemate */
	/* 3 move repetition also covers perpetual check */
/*Bool is_draw() {
    
}*/

/* Returns the piece on a square belonging to player color c. 
 * If there is no piece with color c, UNKNOWN is returned. */
/*Piece get_piece_at(Board pos, PlayerColor c) {
    if((player[c].r & pos) == 1){
    	return ROOK;
	}
	if((player[c].n & pos) == 1){
    	return NIGHT;
	}
	if((player[c].b & pos) == 1){
    	return BISHOP;
	}
	if((player[c].k & pos) == 1){
    	return KING;
	}
	if((player[c].q & pos) == 1){
    	return QUEEN;
	}
	if((player[c].p & pos) == 1){
    	return PAWN;
	}
	return UNKNOWN;
}*/

/* Check if this move is trying to castle */
/*unsigned int detect_castle_move(Move move, PlayerColor c) {
    
}*/

/* Perform castling. Moves king and rook and resets castle flags */
/*void perform_castle(unsigned int castle, PlayerColor c) {
    
}*/