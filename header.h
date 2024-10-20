/**
 * ファイル名：header.h
 * 機能：ヘッダーファイル
*/

#ifndef _HEADER_H_
#define _HEADER_H_

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

/* デバッグ */
#define DEBUG

/* ウィンドウサイズ */
#define WINDOW_WID      696
#define WINDOW_HEI      704
/* 砂の大きさ[px] */
#define SAND_SIZE       4
/* ブロックサイズ */
#define BLOCK_NSAND     8
#define BLOCK_SIZE      (SAND_SIZE * BLOCK_NSAND)       //      砂8px*8pxで１つのブロック
/* スクリーンサイズ */
#define SCREEN_X        32
#define SCREEN_Y        16
#define SCREEN_W        11 * BLOCK_SIZE
#define SCREEN_H        21 * BLOCK_SIZE
#define SCN_WID_NSAND   SCREEN_W / SAND_SIZE
#define SCN_HEI_NSAND   SCREEN_H / SAND_SIZE
#define SCREEN_RECT \
    (SDL_Rect){SCREEN_X, SCREEN_Y, SCREEN_W, SCREEN_H}
#define NEXT_MINO_RECT \
    (SDL_Rect){484, 84, 32*4, 32*4}
/* ミノのサイズ */
#define MINO_SIZE       4
/* デッドエンド */
#define DEAD_ENDLINE    128


/*-----structures-----*/

/* カラー */
enum {
    Black,
    Red1,
    Red2,
    Yellow1,
    Yellow2,
    Green1,
    Green2,
    Blue1,
    Blue2,
    MAX_N_COLOR
};
/* ゲームの状態 */
typedef enum {
    GS_End,
    GS_Playing,
} GameStts;
/* ブロックの種類 */
typedef enum minotype {
    Blank,
    I_MINO,
    O_MINO,
    S_MINO,
    Z_MINO,
    J_MINO,
    L_MINO,
    T_MINO,
    MINO_MAX,
} MinoType;
/* ミノの形状 */
typedef struct minoshape {
    int type;
    int width, height;
    int pattern[MINO_SIZE][MINO_SIZE];
} MinoShape;
/* ミノ情報 */
typedef struct mino {
    int x, y;
    int color;
    MinoShape shape;
} MINO;
/* ゲーム情報 */
typedef struct {
    GameStts stts;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int lines;
    int score;
    int screen[SCN_HEI_NSAND + 1][SCN_WID_NSAND];        //      砂用スクリーン
} GameInfo;


/*------values-----*/

extern GameInfo game;
extern MINO mino, next_mino;
extern MinoShape shapes[MINO_MAX];
extern SDL_Color colors[MAX_N_COLOR];


/*-----functions-----*/

/* system.c */
extern void InitSystem(void);
extern void TerminateSystem(void);
extern void GameLogic(void);
/* window.c */
extern void InitWindow(void);
extern void TerminateWindow(void);
extern void DrawGameScreen(void);


#endif

/* end of header.h */