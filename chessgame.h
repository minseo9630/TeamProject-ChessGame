// chessgame.h
#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <stdbool.h>

// 말 타입
#define PAWN   1
#define ROOK   2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN  5
#define KING   6

// ANSI 색상 매크로
#define ANSI_BG_BLACK   "\033[40m"
#define ANSI_BG_WHITE   "\033[47m"
#define ANSI_FG_DARK    "\033[34m"
#define ANSI_FG_LIGHT   "\033[33m"
#define ANSI_RESET      "\033[0m"

// 전역 변수들 외부에서 쓸 수 있게 extern
extern int board[8][8];
extern bool has_moved[8][8];
extern bool WHITE_KING_CHECK;
extern bool BLACK_KING_CHECK;

extern const char *white_pieces[];
extern const char *black_pieces[];

// 함수 선언
void clear_to_move(void);
void clear_screen(void);
void display(void);

int move(int from_r, int from_c, int to_r, int to_c);
int is_check(int color);
int is_king_safe(int state, int turn,
                 int from_r, int from_c,
                 int to_r, int to_c);

bool is_square_attacked(int target_r, int target_c, int by_color);
bool is_checkmate(int color);

#endif // CHESSGAME_H