/**
 * ファイル名：mino.c
 * 機能：ミノ関連の情報を定義
*/

#include "header.h"

/* カラーコード */
SDL_Color colors[MAX_N_COLOR] = {
    //  黒（描画しない）
    {  0,   0,   0,   0},
    //  赤１
    {204,  20,  82, 255},
    //  赤２
    {153,  15,  61, 255},
    //  黄１
    {202, 171,  20, 255},
    //  黄２
    {153, 130,  15, 255},
    //  緑１
    { 57, 153,  15, 255},
    //  緑２
    { 94, 255,  25, 255},
    //  青１
    { 20, 155, 204, 255},
    //  青２
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