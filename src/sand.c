/**
 * ファイル名：sand.c
 * 機能：メインルーチン
*/

#include "header.h"

/* global values */
GameInfo game;


int main(void) {

    /* 初期化処理 */
    InitWindow();
    InitSystem();

    /* ゲームプレイ */
    while ( game.stts == GS_Playing ) {
        DrawGameScreen();
        GameLogic();
        SDL_Delay(10);
    }

    /* 終了処理 */
    TerminateSystem();
    TerminateWindow();
    return 0;
}

/* end of sand.c */