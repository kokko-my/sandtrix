/**
 * ファイル名：window.c
 * 機能：ウィンドウ描画モジュール
*/

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "header.h"

#define IMAGE_PATH  "Images/game_sheet.png"
#define MUSIC_PATH  "Musics/UrbanBGM_01.mp3"

/* global values */
static SDL_Texture *game_sheet;
static Mix_Music   *game_bgm;
static SDL_Rect     src, dst;

/* static functions */
static void DrawWindow(void);
static void DrawMinoBlock(int, int, int);
static void DrawMino(void);
static void DrawNextMino(void);
static void DrawSands(void);
static void DrawNumber(int, int, int);
static void DrawStrings(void);


/*-----初期化処理-----*/

/**
 * ウィンドウ初期化関数
 * 引数：なし
 * 返値：なし
*/
void InitWindow(void) {

    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        goto DESTROY;
    }
    game.window = SDL_CreateWindow("SANDTRIX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WID, WINDOW_HEI, 0);
    if ( game.window == NULL ) {
        goto DESTROY;
    }
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_SOFTWARE);
    if ( game.renderer == NULL ) {
        goto DESTROY;
    }

    /* テクスチャ作成 */
    SDL_Surface *s = IMG_Load(IMAGE_PATH);
    game_sheet = SDL_CreateTextureFromSurface(game.renderer, s);

    /* 音源作成 */
    if ( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 256) < 0 ) {
        goto DESTROY;
    }
    game_bgm = Mix_LoadMUS(MUSIC_PATH);
    Mix_PlayMusic(game_bgm, -1);

    SDL_RenderPresent(game.renderer);
    return;

DESTROY:
    fprintf(stderr, "failed to initialize window.\n");
    TerminateWindow();
    exit(-1);
}

/*-----終了処理-----*/

/**
 * SDLを終了させる
 * 引数：なし
 * 返値：なし
*/
void TerminateWindow(void) {
    Mix_HaltMusic();
    Mix_FreeMusic(game_bgm);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
}


/*-----描画処理-----*/

/**
 * ゲーム画面描画関数
 * 引数：なし
 * 返値：なし
*/
void DrawGameScreen(void) {
    DrawWindow();
    DrawSands();
    DrawMino();
    DrawNextMino();
    DrawStrings();
    SDL_RenderPresent(game.renderer);
}

/**
 * ウィンドウ描画
 * 引数: なし
 * 返値: なし
*/
static void DrawWindow(void) {
    SDL_SetRenderDrawColor(game.renderer, 129, 85, 240, 255);       //      ウィンドウ
    SDL_RenderClear(game.renderer);
    dst = SCREEN_RECT;
    dst.x -= 4; dst.y -= 4;
    dst.w += 8; dst.h += 8;

    SDL_SetRenderDrawColor(game.renderer, 240, 240, 240, 255);      //      スクリーン
    SDL_RenderFillRect(game.renderer, &dst);
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game.renderer, &SCREEN_RECT);

    SDL_SetRenderDrawColor(game.renderer, 100, 100, 100, 50);          //      デッドエンド
    SDL_RenderDrawLine(game.renderer, SCREEN_X, DEAD_ENDLINE, SCREEN_X + SCREEN_W, DEAD_ENDLINE);

    dst = NEXT_MINO_RECT;                                           //      ネクスト欄
    dst.x -= 4;
    dst.y -= 4;
    dst.w += 8;
    dst.h += 8;
    SDL_SetRenderDrawColor(game.renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(game.renderer, &dst);
    dst = NEXT_MINO_RECT;
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game.renderer, &dst);
}

/**
 * 砂を描画
 * 引数: なし
 * 返値: なし
*/
static void DrawSands(void) {
    dst = (SDL_Rect){0, 0, SAND_SIZE, SAND_SIZE};
    for ( int y = 0; y < SCN_HEI_NSAND; y++ ) {
        dst.y = SCREEN_Y + y * SAND_SIZE;
        for ( int x = 0; x < SCN_WID_NSAND; x++ ) {
            if ( game.screen[y][x] == Blank ) {
                continue;
            }
            dst.x = SCREEN_X + x * SAND_SIZE;
            SDL_Color cl = colors[game.screen[y][x]];
            SDL_SetRenderDrawColor(game.renderer, cl.r, cl.g, cl.b, cl.a);
            SDL_RenderFillRect(game.renderer, &dst);
        }
    }
}

/**
 * ミノブロックを描画する
 * 引数：ブロック左上座標, 色
 * 返値：なし
*/
static void DrawMinoBlock(int x, int y, int _c) {
    if ( _c == Black )   return;

    _c -= (_c % 2 == 0) ? 1 : 0;
    int c = _c;

    for ( int n = 0; n < BLOCK_NSAND/2; n++, x += SAND_SIZE, y += SAND_SIZE ) {
        SDL_Color cl = colors[c];
        SDL_Rect d = {x, y, SAND_SIZE*(8-n*2), SAND_SIZE*(8-n*2)};
        SDL_SetRenderDrawColor(game.renderer, cl.r, cl.g, cl.b, cl.a);
        SDL_RenderFillRect(game.renderer, &d);
        c += (n % 2 == 0) ? 1 : -1;
    }
}

/**
 * ミノを描画
 * 引数: ミノ
 * 返値: なし
*/
static void DrawMino(void) {
    for ( int y = 0; y < mino.shape.height; y++ ) {
        dst.y = SCREEN_Y + mino.y + (y * BLOCK_SIZE);
        for ( int x = 0; x < mino.shape.width; x++ ) {
            if ( mino.shape.pattern[y][x] == 0 ) {
                continue;
            }
            dst.x = SCREEN_X + mino.x + (x * BLOCK_SIZE);
            DrawMinoBlock(dst.x, dst.y, mino.color);
        }
    }
}

/**
 * ネクストミノを描画
 * 引数: ミノ
 * 返値: なし
*/
static void DrawNextMino(void) {
    SDL_Rect rect = NEXT_MINO_RECT;
    switch ( next_mino.shape.type ) {         //      位置を微調整
    case I_MINO:
    case S_MINO:
    case Z_MINO:
    case J_MINO:
    case L_MINO:
        rect.y += BLOCK_SIZE / 2;
        break;
    case O_MINO:
        rect.x += BLOCK_SIZE;
        rect.y += BLOCK_SIZE;
        break;
    case T_MINO:
        rect.x += BLOCK_SIZE / 2;
        rect.y += BLOCK_SIZE;
        break;
    default:
        break;
    }

    for ( int y = 0; y < next_mino.shape.height; y++ ) {
        dst.y = rect.y + (y * BLOCK_SIZE);
        for ( int x = 0; x < next_mino.shape.width; x++ ) {
            if ( next_mino.shape.pattern[y][x] == 0 ) {
                continue;
            }
            dst.x = rect.x + (x * BLOCK_SIZE);
            DrawMinoBlock(dst.x, dst.y, next_mino.color);
        }
    }
}

/**
 * 数字を指定座標に描画する
 * 引数：描画する数字, その左上座標
 * 返値：なし
*/
static void DrawNumber(int number, int x, int y) {
    for ( int i = 1; i <= 5; i++ ) {
        if ( number < 1 && i != 1 ) {
            continue;
        }
        int n = number % 10;
        src = (SDL_Rect){32*n, 16*9, 32, 32};
        dst = (SDL_Rect){x + 32*(5-i), y, 32, 32};
        SDL_RenderCopy(game.renderer, game_sheet, &src, &dst);
        number /= 10;
    }
}

/**
 * 文字を描画
 * 引数: なし
 * 返値: なし
*/
static void DrawStrings(void) {
    src = (SDL_Rect){0, 0, 16*9, 16*3};
    dst = (SDL_Rect){474, 32, 16*9, 16*3};
    SDL_RenderCopy(game.renderer, game_sheet, &src, &dst);      //  'NEXT'
    src = (SDL_Rect){0, 16*3, 16*10, 16*3};
    dst = (SDL_Rect){469, 262, 16*10, 16*3};
    SDL_RenderCopy(game.renderer, game_sheet, &src, &dst);      //  'LINES'
    src = (SDL_Rect){0, 16*6, 16*11, 16*3};
    dst = (SDL_Rect){460, 392, 16*11, 16*3};
    SDL_RenderCopy(game.renderer, game_sheet, &src, &dst);      //  'SCORE'
    src = (SDL_Rect){0, 16*11, 16*19, 16*8};
    dst = (SDL_Rect){452, 522, 16*12, 16*5};
    SDL_RenderCopy(game.renderer, game_sheet, &src, &dst);      //  'CATACLYSM:'
    DrawNumber(game.lines, 474, 314);
    DrawNumber(game.score, 474, 444);
    DrawNumber(game.ct_remain, 442, 558);
}


/* end of window.c */