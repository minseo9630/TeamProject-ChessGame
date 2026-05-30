// chessgame.c
#include "chessgame.h"
#include<stdio.h>
#include<stdlib.h>

// 전역 변수 실제 정의

int board[8][8] ={ // 8행 (흑색: 2, 3, 4, 5, 6)
    {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
    {PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},// 2행 (백색: -1, -2, -3, -4, -5, -6)
    {-PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN},
    {-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK}};

bool has_moved[8][8];
bool WHITE_KING_CHECK = false;
bool BLACK_KING_CHECK = false;

const char *white_pieces[] = {"♙","♖","♘","♗","♕","♔"};
const char *black_pieces[] = {"♟","♜","♞","♝","♛","♚"};

void clear_to_move()
{ // -> false: 한번도 안움직임. /true: 한번은 움직임
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            has_moved[r][c] = false;
        }
    }
}

void clear_screen()
{
    // system("cls");
    printf("\033[H\033[2J");
    fflush(stdout);
}

void display()
{
    int i, j;

    printf("  A B C D E F G H\n");

    for (i = 0; i < 8; i++)
    {

        printf("%d ", 8 - i);

        for (j = 0; j < 8; j++)
        {
            int piece_value = board[i][j];
            const char *piece_icon = " "; // 기본값: 빈 칸
            int piece_type;

            // 배경색
            if ((i + j) % 2 == 0)
            {
                printf("%s", ANSI_BG_BLACK);
            }
            else
            {
                printf("%s", ANSI_BG_WHITE);
            }

            if (piece_value != 0)
            { // 칸에 말이 있다면

                if (piece_value < 0)
                {                                // 백색 말 (음수)
                    printf("%s", ANSI_FG_LIGHT); // 노랑색
                    piece_type = abs(piece_value);
                    piece_icon = white_pieces[piece_type - 1];
                }
                else
                {                               // 흑색 말 (양수)
                    printf("%s", ANSI_FG_DARK); // 파랑색
                    piece_type = piece_value;
                    piece_icon = black_pieces[piece_type - 1];
                }
            }

            // 아이콘 출력
            printf("%s ", piece_icon);

            printf("%s", ANSI_RESET);
        }
        printf("\n");
    }
    printf(ANSI_RESET); // 혹시 모를 경우를 대비해 최종 초기화
}

int move(int from_r, int from_c, int to_r, int to_c)
{ // 기물 움직임

    int selected_piece = board[from_r][from_c];

    // PAWN 일때.
    if (abs(selected_piece) == PAWN)
    {
        if (board[from_r][from_c] == 1)
        { // 흑색 폰일경우
            if (to_r < from_r)
            {
                return 0; // 뒤로 이동시 return 0
            }
        }
        else
        { // 백색 폰일경우
            if (to_r > from_r)
            {
                return 0; // 뒤로 이동시 return 0
            }
        }
        if (abs(from_c - to_c) == 1 && abs(from_r - to_r) == 1)
        { // 대각선 한칸 이동
            // int step_r = (to_r > from_r) ? 1 : -1;    //위로 1이동 or 아래로 1 이동
            // int step_r = (to_c > from_c) ? 1 : -1;    //옆으로 한칸 이동
            if (board[to_r][to_c] != 0)
            { // 대각선에 기물이 있을때.
                if ((board[from_r][from_c] < 0 && board[to_r][to_c] < 0) || (board[from_r][from_c] > 0 && board[to_r][to_c] > 0))
                { // 대각선에 있는 값이 같은색인지 판별
                    return 0;
                }
                else
                {
                    // 승진
                    if (to_r == 0 || to_r == 7)
                    {
                        return 10;
                    }
                    return 1;
                }
            }
            else
            { // 대각선에 기물이 없을때
                return 0;
            }
        }
        else if (from_c != to_c || abs(from_r - to_r) > 2)
        { // 옳지 않은 이동일때
            return 0;
        }
        else
        {
            // if((selected_piece == -1 && from_r == 6) || (selected_piece == 1 && from_r == 1)){ //폰 위치가 초기 위치일때.
            if (has_moved[from_r][from_c] == false)
            {                                        // 한번도 안움직였을때
                int step = (to_r > from_r) ? 1 : -1; // 위로 -1이동 or 아래로 1 이동

                for (int r = from_r + step; r != to_r; r += step)
                { // 이동 경로가 막혀있는지.
                    if (board[r][from_c] != 0)
                    {
                        return 0; // 앞이 막혀 있는 상태
                    }
                }
                return 1;
            }
            else
            {
                if (abs(from_r - to_r) == 1)
                {
                    if (board[to_r][to_c] != 0)
                    { // 도착칸에 기물이 있는경우
                        return 0;
                    }
                    // 승진
                    if (to_r == 0 || to_r == 7)
                    {
                        return 10;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    // ROOK 일때.
    else if (abs(selected_piece) == ROOK)
    {
        if (from_r == to_r && from_c == to_c)
        {
            return 0;
        }

        if (from_c != to_c && from_r != to_r)
        {
            return 0;
        }
        else
        {
            if ((selected_piece < 0 && board[to_r][to_c] < 0) || // 같은색 기물 못먹게
                (selected_piece > 0 && board[to_r][to_c] > 0))
            {
                return 0;
            }

            if (from_r == to_r && from_c != to_c)
            {                                        // 좌우
                int step = (to_c > from_c) ? 1 : -1; // 1 -> 오른쪽 / -1 -> 왼쪽

                for (int c = from_c + step; c != to_c; c += step)
                {
                    if (board[from_r][c] != 0)
                    { // 이동 경로에 막혀있을때
                        return 0;
                    }
                }
                return 1;
            }

            else
            {                                        // 앞뒤
                int step = (to_r > from_r) ? 1 : -1; // 1 -> 아래 / -1 -> 위

                for (int r = from_r + step; r != to_r; r += step)
                {
                    if (board[r][from_c])
                    {
                        return 0; // 막혀있을때
                    }
                }
                return 1;
            }
        }
    }

    // KNIGHT 일때
    else if (abs(selected_piece) == KNIGHT)
    {
        if ((abs((from_r - to_r)) == 1 && abs(from_c - to_c) == 2) ||
            (abs(from_c - to_c) == 1 && abs(from_r - to_r) == 2))
        {

            if (board[to_r][to_c] == 0)
            {
                return 1;
            }
            else
            {
                if ((selected_piece > 0 && board[to_r][to_c] > 0) ||
                    (selected_piece < 0 && board[to_r][to_c] < 0))
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            return 0;
        }
    }

    // BISHOP 일때
    else if (abs(selected_piece) == BISHOP)
    {
        if ((to_r - from_r == 0) &&
            (to_c - from_c == 0))
        {
            return 0;
        }
        if (abs(to_r - from_r) != abs(to_c - from_c))
        {
            return 0;
        }

        int step_r = (to_r > from_r) ? 1 : -1; // 1: 아래, -1: 위
        int step_c = (to_c > from_c) ? 1 : -1; // 1: 오른쪽, -1: 왼쪽

        for (int r = from_r + step_r, c = from_c + step_c;
             r != to_r && c != to_c;
             r += step_r, c += step_c)
        {
            if (board[r][c] != 0)
            {
                return 0;
            }
        }
        if ((board[to_r][to_c] > 0 && selected_piece > 0) ||
            (board[to_r][to_c] < 0 && selected_piece < 0))
        {
            return 0;
        }
        return 1;
    }

    // QUEEN 일때
    else if (abs(selected_piece) == QUEEN)
    {
        if ((to_r - from_r == 0) && // 제자리 이동
            (to_c - from_c == 0))
        {
            return 0;
        }
        if (abs(to_r - from_r) == abs(to_c - from_c))
        {                                          // 대각선 이동
            int step_r = (to_r > from_r) ? 1 : -1; // 1: 아래, -1: 위
            int step_c = (to_c > from_c) ? 1 : -1; // 1: 오른쪽, -1: 왼쪽

            for (int r = from_r + step_r, c = from_c + step_c;
                 r != to_r && c != to_c;
                 r += step_r, c += step_c)
            {
                if (board[r][c] != 0)
                {
                    return 0;
                }
            }

            if ((board[to_r][to_c] > 0 && selected_piece > 0) ||
                (board[to_r][to_c] < 0 && selected_piece < 0))
            {
                return 0;
            }
            return 1;
        }
        else if ((to_r == from_r && to_c != from_c) ||
                 to_r != from_r && to_c == from_c)
        {
            if ((selected_piece < 0 && board[to_r][to_c] < 0) || // 같은색 기물 못먹게
                (selected_piece > 0 && board[to_r][to_c] > 0))
            {
                return 0;
            }

            if (from_r == to_r && from_c != to_c)
            {                                        // 좌우
                int step = (to_c > from_c) ? 1 : -1; // 1 -> 오른쪽 / -1 -> 왼쪽

                for (int c = from_c + step; c != to_c; c += step)
                {
                    if (board[from_r][c] != 0)
                    { // 이동 경로에 막혀있을때
                        return 0;
                    }
                }
                return 1;
            }

            else
            {                                        // 앞뒤
                int step = (to_r > from_r) ? 1 : -1; // 1 -> 아래 / -1 -> 위

                for (int r = from_r + step; r != to_r; r += step)
                {
                    if (board[r][from_c])
                    {
                        return 0; // 막혀있을때
                    }
                }
                return 1;
            }
        }
        return 0;
    }

    // KING 일때
    else if (abs(selected_piece) == KING)
    {

        // 캐슬링
        if (abs(to_c - from_c) == 2)
        { // 옆으로 두칸 이동
            // 캐슬링
            if (has_moved[from_r][from_c] == false && has_moved[to_r][to_c] == false && to_r == from_r)
            { // 한번도 안움직였을때
                if (board[to_r][to_c] == ROOK && BLACK_KING_CHECK == false)
                {                                        // 흑색 룩, 킹일때
                    int step = (to_c > from_c) ? 1 : -1; // 1 -> 오른쪽 / -1 -> 왼쪽
                    for (int c = from_c + step; c != to_c; c += step)
                    {
                        if (board[from_r][c] != 0)
                        { // 이동 경로에 막혀있을때
                            return 0;
                        }
                    }
                    return -2; // 캐슬링 가능
                }
                if (board[to_r][to_c] == -ROOK && WHITE_KING_CHECK == false)
                {                                        // 백색 룩, 킹일때
                    int step = (to_c > from_c) ? 1 : -1; // 1 -> 오른쪽 / -1 -> 왼쪽
                    for (int c = from_c + step; c != to_c; c += step)
                    {
                        if (board[from_r][c] != 0)
                        { // 이동 경로에 막혀있을때
                            return 0;
                        }
                    }
                    return -2; // 캐슬링 가능
                }
                return 0;
            }
        }

        if (to_r == from_r && to_c == from_c)
        {
            return 0;
        }
        if ((abs(to_c - from_c) == 1 || abs(to_r - from_r) == 1))
        {                                             // 이동칸이 1칸일때
            if ((to_r == from_r && to_c != from_c) || // 상하좌우
                (to_r != from_r && to_c == from_c))
            {
                if (board[to_r][to_c] == 0)
                { // 이동 반경에 기물이 없을때
                    return 1;
                }
                else
                {
                    if ((board[to_r][to_c] > 0 && selected_piece > 0) || // 같은색일때
                        (board[to_r][to_c] < 0 && selected_piece < 0))
                    {
                        return 0;
                    }
                    return 1;
                }
            }
            if (abs(to_r - from_r) == abs(to_c - from_c))
            { // 대각선 이동
                if (board[to_r][to_c] != 0)
                {                                                        // 도착칸이 안비여 있을때
                    if ((board[to_r][to_c] > 0 && selected_piece > 0) || // 같은색일때
                        (board[to_r][to_c] < 0 && selected_piece < 0))
                    {
                        return 0;
                    }
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int is_check(int color)
{ // 킹이 체크 상태인지 1: 흑 -1: 백
    int king_r = -1, king_c = -1;

    for (int r = 0; r < 8; r++)
    { // 킹 위치 찾기
        for (int c = 0; c < 8; c++)
        {
            if (board[r][c] == color * KING)
            {
                king_r = r;
                king_c = c;
            }
        }
        if (king_r != -1)
        {
            break;
        }
    }
    if (king_r == -1)
    {
        return -1;
    }

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int selected_piece = board[r][c];
            if (selected_piece == 0)
            { // 선택 공간이 빈공간이면
                continue;
            }
            if ((selected_piece > 0 && color > 0) || // 같은색이면
                (selected_piece < 0 && color < 0))
            {
                continue;
            }
            int result = move(r, c, king_r, king_c);
            if (result == 1 || result == 10)
            { // 체크 상태
                return 1;
            }
        }
    }
    return 0;
}

int is_king_safe(int state, int turn, int from_r, int from_c, int to_r, int to_c)
{
    if (state == 1)
    { // 일반적인 움직임 일때.
        int selected_piece = board[from_r][from_c];
        int moved_piece = board[to_r][to_c];

        board[to_r][to_c] = selected_piece;
        board[from_r][from_c] = 0;

        int result = is_check(turn);

        board[from_r][from_c] = selected_piece;
        board[to_r][to_c] = moved_piece;

        if (result == 1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if (state == -2)
    {
        int selected_piece = board[from_r][from_c];
        int moved_piece = board[to_r][to_c];

        if (turn == -1)
        { // 백말
            if (to_c == 7 || from_c == 7)
            { // 오른쪽 캐슬링
                board[to_r][5] = -ROOK;
                board[to_r][6] = -KING;
                board[from_r][from_c] = 0;
                board[to_r][to_c] = 0;
            }
            else
            { // 왼쪽 캐슬링
                board[to_r][3] = -ROOK;
                board[to_r][2] = -KING;
                board[from_r][from_c] = 0;
                board[to_r][to_c] = 0;
            }
        }
        else
        { // 흑말
            if (to_c == 7 || from_c == 7)
            { // 오른쪽 캐슬링
                board[to_r][5] = ROOK;
                board[to_r][6] = KING;
                board[from_r][from_c] = 0;
                board[to_r][to_c] = 0;
            }
            else
            { // 왼쪽 캐슬링
                board[to_r][3] = ROOK;
                board[to_r][2] = KING;
                board[from_r][from_c] = 0;
                board[to_r][to_c] = 0;
            }
        }
        int result = is_check(turn);

        if (turn == -1)
        { // 백말
            if (to_c == 7 || from_c == 7)
            { // 오른쪽 캐슬링
                board[to_r][5] = 0;
                board[to_r][6] = 0;
                board[from_r][from_c] = selected_piece;
                board[to_r][to_c] = moved_piece;
            }
            else
            { // 왼쪽 캐슬링
                board[to_r][3] = 0;
                board[to_r][2] = 0;
                board[from_r][from_c] = selected_piece;
                board[to_r][to_c] = moved_piece;
            }
        }
        else
        { // 흑말
            if (to_c == 7 || from_c == 7)
            { // 오른쪽 캐슬링
                board[to_r][5] = 0;
                board[to_r][6] = 0;
                board[from_r][from_c] = selected_piece;
                board[to_r][to_c] = moved_piece;
            }
            else
            { // 왼쪽 캐슬링
                board[to_r][3] = 0;
                board[to_r][2] = 0;
                board[from_r][from_c] = selected_piece;
                board[to_r][to_c] = moved_piece;
            }
        }
        if (result == 1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 1;
}

bool is_square_attacked(int target_r, int target_c, int by_color)
{
    for (int from_r = 0; from_r < 8; from_r++)
    {
        for (int from_c = 0; from_c < 8; from_c++)
        {
            int piece = board[from_r][from_c];

            if (piece != 0 && (piece * by_color < 0))
            {

                int original_target_piece = board[target_r][target_c];

                board[target_r][target_c] = by_color * KING;

                // 현재 기물이 목표 칸으로 이동 가능한지 확인
                // 폰은 이동 규칙상 킹을 잡는 이동(대각선)이 아니면 유효하지 않으므로, 폰의 경우만 예외 처리
                int is_pawn = abs(piece) == PAWN;
                int move_result = 0;

                if (is_pawn)
                {
                    // 폰의 공격(대각선)을 확인
                    int direction = (piece > 0) ? 1 : -1;
                    if (target_r == from_r + direction && abs(target_c - from_c) == 1)
                    {
                        move_result = 1;
                    }
                }
                else
                {
                    move_result = move(from_r, from_c, target_r, target_c);
                }

                board[target_r][target_c] = original_target_piece;

                if (move_result == 1 || move_result == 10)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_checkmate(int color)
{
    for (int from_r = 0; from_r < 8; from_r++)
    {
        for (int from_c = 0; from_c < 8; from_c++)
        {
            int piece = board[from_r][from_c];

            // 현재 턴의 기물이 아니면 건너뛰기
            if (piece == 0 || (piece < 0 && color == 1) || (piece > 0 && color == -1))
            {
                continue;
            }

            // 모든 도착 칸을 시도
            for (int to_r = 0; to_r < 8; to_r++)
            {
                for (int to_c = 0; to_c < 8; to_c++)
                {
                    int move_result = move(from_r, from_c, to_r, to_c);

                    // 일반 이동(1) 또는 폰 승진(10) 검사
                    if (move_result == 1 || move_result == 10)
                    {
                        if (is_king_safe(1, color, from_r, from_c, to_r, to_c) == 1)
                        {
                            return true;
                        }
                    }
                    // 캐슬링(-2) 검사
                    else if (move_result == -2)
                    {
                        int attacker_color = color * -1;
                        int path_safe = 1;
                        int intermediate_c = from_c + (to_c > from_c ? 1 : -1);

                        if (is_square_attacked(from_r, from_c, attacker_color) ||
                            is_square_attacked(from_r, intermediate_c, attacker_color) ||
                            is_square_attacked(to_r, to_c, attacker_color))
                        {
                            path_safe = 0;
                        }

                        if (path_safe == 1)
                        {
                            return true; // 합법적인 캐슬링 이동을 찾음.
                        }
                    }
                }
            }
        }
    }
    return false;
}