// 시작 날짜: 12/04
// 종료 날짜: 12/14

// 총 소요일자: 10일

// 12/10: 틀 완성
// 12/11 ~ 12/14: 알고리즘 오류등 수정

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include "chessgame.h"
#include "chessgame.c"

int main()
{
    int row_in, col_in; // 입력받는 행 번호
    int from_r, from_c, to_r, to_c;
    char from_c_char, to_c_char;
    int current_turn = -1; // -1: 백색, 1: 흑색

    clear_to_move();

    while (1)
    {
        clear_screen();
        display();
        int BLACK_KING = is_check(1);  // 흑
        int WHITE_KING = is_check(-1); // 백

        if (BLACK_KING == 1)
        {
            BLACK_KING_CHECK = true;
            printf("\n+-- 흑색 킹이 체크 상태입니다! --+\n");
        }
        else if (BLACK_KING == -1)
        {
            printf("\n+-- 백색 말이 승리하였습니다! --+\n");
            return 0;
        }
        if (WHITE_KING == 1)
        {
            WHITE_KING_CHECK = true;
            printf("\n+-- 백색 킹이 체크 상태입니다! --+\n");
        }
        else if (WHITE_KING == -1)
        {
            printf("\n+-- 흑색 말이 승리하였습니다! --+\n");
            return 0;
        }

        bool has_legal_move = is_checkmate(current_turn);

        // 체크메이트/스테일메이트 확인
        if (!has_legal_move)
        {
            if (BLACK_KING == 1)
            {
                printf("\n+---- 체크메이트! -----+\n|흑색 말이 승리하였습니다!|\n+-------------------+\n");
                return 0;
            }
            if (WHITE_KING == 1)
            {
                printf("\n+---- 체크메이트! -----+\n|백색 말이 승리하였습니다!|\n+-------------------+\n");
                return 0;
            }
        }
        if (!has_legal_move)
        {
            if (BLACK_KING != 1 && WHITE_KING != 1)
            {
                printf("\n+--- 스테일메이트!! ----+\n|백색 말이 승리하였습니다!|\n+-------------------+\n");
                return 0;
            }
        }

        // 차례 -> -1: 백색말 / 1: 흑색말
        if (current_turn == -1)
        {
            printf("\n⚪ 백색말 차례입니다.\n");
        }
        else
        {
            printf("\n⚫ 흑색말 차례입니다.\n");
        }

        printf("이동할 기물의 위치 (예: E2): ");
        if (scanf(" %c%d", &from_c_char, &row_in) != 2)
        {
            printf("잘못된 입력 형식입니다. 다시 시도하세요.\n(ENTER키를 누르시오.)\n");
            // 입력 버퍼 지우기
            while (getchar() != '\n')
                ;
            continue;
        }

        // 입력 좌표 변환
        from_c = toupper(from_c_char) - 'A';
        from_r = 8 - row_in;

        // 범위 확인
        if (from_c < 0 || from_c > 7 || row_in < 1 || row_in > 8)
        {
            printf("잘못된 좌표입니다. 다시 시도하세요.\n(ENTER키를 누르시오.)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        // 선택 위치에 말 존재하는 확인.
        int selected_piece = board[from_r][from_c];
        if (selected_piece == 0)
        {
            printf("해당 위치에는 말이 없습니다.\n(ENTER키를 누르시오.)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        // 턴 검사
        if ((current_turn == -1 && selected_piece > 0) || (current_turn == 1 && selected_piece < 0))
        {
            printf("상대방의 말은 움직일 수 없습니다.\n(ENTER키를 누르시오.)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        // 도착좌표 받기
        printf("도착할 위치 (예: E4): ");
        if (scanf(" %c%d", &to_c_char, &row_in) != 2)
        {
            printf(" 잘못된 입력 형식입니다. 다시 시도하세요.\n(ENTER키를 누르시오.)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        to_c = toupper(to_c_char) - 'A';
        to_r = 8 - row_in;

        if (to_c < 0 || to_c > 7 || row_in < 1 || row_in > 8)
        {
            printf("잘못된 도착 좌표입니다. 다시 시도하세요.\n(ENTER키를 누르시오.)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        // 0: 잘못된 이동, 1: 올바른 이동, -2: 캐슬링, 10: 폰승진
        int value = move(from_r, from_c, to_r, to_c);
        if (value == 1)
        {
            // 이동 실행
            if (is_king_safe(1, current_turn, from_r, from_c, to_r, to_c) == 1)
            {

                has_moved[to_r][to_c] = true;

                selected_piece = board[from_r][from_c]; // 한번도 초기화
                board[to_r][to_c] = selected_piece;     // 도착지에 말 복사
                board[from_r][from_c] = 0;              // 출발지 빈 칸으로 설정

                // 턴 변경
                current_turn *= -1;
            }
            else
            {
                printf("+-- 해당 기물을 움직이면 체크입니다. (ENTER키를 누르시오.) --+\n");
                while (getchar() != '\n')
                    ;
                getchar();
            }
        }
        else if (value == -2)
        { // 캐슬링
            has_moved[to_r][to_c] = true;

            selected_piece = board[from_r][from_c]; // 한번도 초기화
            //  board[from_r][from_c] = board[to_r][to_c];
            //  board[to_r][to_c] = selected_piece; // 도착지에 말 복사
            //  current_turn *= -1;

            if (is_king_safe(-2, current_turn, from_r, from_c, to_r, to_c) == 1)
            {
                if (current_turn == -1)
                { // 백말
                    if (to_c == 7 || from_c == 7)
                    { // 오른쪽 캐슬링
                        board[to_r][5] = -ROOK;
                        board[to_r][6] = -KING;
                        board[from_r][from_c] = 0;
                        board[to_r][to_c] = 0;
                        has_moved[to_r][5] = true;
                        has_moved[to_r][6] = true;
                    }
                    else
                    { // 왼쪽 캐슬링
                        board[to_r][3] = -ROOK;
                        board[to_r][2] = -KING;
                        board[from_r][from_c] = 0;
                        board[to_r][to_c] = 0;
                        has_moved[to_r][3] = true;
                        has_moved[to_r][2] = true;
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
                        has_moved[to_r][5] = true;
                        has_moved[to_r][6] = true;
                    }
                    else
                    { // 왼쪽 캐슬링
                        board[to_r][3] = ROOK;
                        board[to_r][2] = KING;
                        board[from_r][from_c] = 0;
                        board[to_r][to_c] = 0;
                        has_moved[to_r][3] = true;
                        has_moved[to_r][2] = true;
                    }
                }
                current_turn *= -1;
            }
            else
            {
                printf("+-- 캐슬링시 체크입니다. (ENTER키를 누르시오.) --+\n");
                while (getchar() != '\n')
                    ;
                getchar();
            }
        }
        else if (value == 10)
        { // 폰 승진
            char choice;
            int new_piece;             // 승진할 기물
            board[from_r][from_c] = 0; // 출발지 빈 칸으로 설정
            while (1)
            {
                printf("폰이 승진하였습니다!\n");
                printf("원하는 기물을 선택하세요 (Q=퀸, R=룩, B=비숍, N=나이트): ");
                if (scanf(" %c", &choice) != 1)
                {
                    // 입력 이상하면 버퍼 비우고 다시
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                choice = toupper(choice);
                if (choice == 'Q')
                {
                    new_piece = QUEEN;
                    break;
                }
                else if (choice == 'R')
                {
                    new_piece = ROOK;
                    break;
                }
                else if (choice == 'B')
                {
                    new_piece = BISHOP;
                    break;
                }
                else if (choice == 'N')
                {
                    new_piece = KNIGHT;
                    break;
                }
                else
                {
                    printf("잘못된 입력입니다. Q / R / B / N 중에서 선택하세요.\n");
                }
            }
            // 현재 턴에 따라 색 부여
            if (current_turn == -1)
            { // 백 차례였다면 백말
                board[to_r][to_c] = -new_piece;
            }
            else
            { // 흑 차례
                board[to_r][to_c] = new_piece;
            }

            current_turn *= -1;
        }
        else if (value == 0)
        {
            printf("잘못된 이동입니다. (Enter키를 누르시오.) \n");
            while (getchar() != '\n')
                ;
            getchar();
        }
        else
        {
            printf("유효하지 않은 이동입니다.\n 체스 규칙을 확인하세요. (ENTER키를 누르시오.) \n");
            while (getchar() != '\n')
                ;
            getchar();
        }
    }
    return 0;
}