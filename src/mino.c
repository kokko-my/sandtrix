/**
 * ファイル名：mino.c
 * 機能：ミノ関連の情報を定義
*/

#include "header.h"

/* カラーコード */
SDL_Color colors[MAX_N_COLOR] = {
    //  Black（描画しない）
    {  0,   0,   0,   0},
    //  White
    { 230, 230, 230, 255},
    //  Red1
    {204,  20,  82, 255},
    //  Red2
    {153,  15,  61, 255},
    //  Yellow1
    {202, 171,  20, 255},
    //  Yellow2
    {153, 130,  15, 255},
    //  Green1
    { 57, 153,  15, 255},
    //  Green2
    { 94, 255,  25, 255},
    //  Blue1
    { 20, 155, 204, 255},
    //  Blue2
    { 15, 116, 153, 255},
};

MINO mino;
MINO next_mino;

MinoShape shapes[MINO_MAX] = {
    /* Blank */
    {
        Blank,
        0, 0,
        {
            {0}
        }
    },
    /* I_MINO */
    {
        I_MINO,                 //      type
        4, 4,                   //      width, height
        {
            {0, 0, 0, 0},       //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }
    },
    /* O_MINO */
    {
        O_MINO,                 //      type
        2, 2,                   //      width, height
        {
            {1, 1},             //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {1, 1},
        }
    },
    /* S_MINO */
    {
        S_MINO,                 //      type
        3, 3,                   //      width, height
        {
            {0, 1, 0},          //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {0, 1, 1},
            {0, 0, 1},
        }
    },
    /* Z_MINO */
    {
        Z_MINO,                 //      type
        3, 3,                   //      width, height
        {
            {0, 0, 1},          //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {0, 1, 1},
            {0, 1, 0},
        }
    },
    /* J_MINO */
    {
        J_MINO,                 //      type
        3, 3,                   //      width, height
        {
            {0, 0, 1},          //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {0, 0, 1},
            {0, 1, 1},
        }
    },
    /* L_MINO */
    {
        L_MINO,                 //      type
        3, 3,                   //      width, height
        {
            {0, 1, 0},          //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {0, 1, 0},
            {0, 1, 1},
        }
    },
    /* T_MINO */
    {
        T_MINO,                 //      type
        3, 3,                   //      width, height
        {
            {0, 1, 0},          //      pattern[MINO_HEIGHT_MAX][MINO_WIDTH_MAX]
            {1, 1, 1},
            {0, 0, 0},
        }
    },
};

/* end of mino.c */