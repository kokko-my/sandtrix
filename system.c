/**
 * ファイル名：system.c
 * 機能：ゲームロジックモジュール
*/

#include "header.h"


/* global values */
/* 入力状態 */
typedef struct {
    int down;
    int right;
    int left;
} InputStts;
static InputStts input;
/* 単位ベクトル */
static int dy[] = {0, -1, 0, 1, 1, -1, -1, 1};
static int dx[] = {1, 0, -1, 0, 1, 1, -1, -1};
/* タイマーID */
static SDL_TimerID timer_mino, timer_clps;
/* 経過時間 */
static time_t time_prev;    //  [s]

/* static functions */
static void InitTimer(void);
static int  InputEvent(void);
static void InitMino(void);
static MINO MakeMino(void);
static void MoveMino(void);
static void RotateMino(int);
static int  IntersectMino(void);
static void ReflectScreen(void);
static void RemoveCompleteSand(int, int, int);
static int  CheckSandComplete(int, int, Color);
static Uint32 FallMino(Uint32, void*);
static Uint32 CollapseMino(Uint32, void*);
static void ShuffleIntArray(int[], int);


/*-----初期化処理-----*/

/**
 * ゲームシステム初期化関数
 * 引数：なし
 * 返値：0...正常終了, -1...エラー
*/
void InitSystem(void) {

    srand(time(NULL));

    /* ゲーム情報 */
    game.stts = GS_Playing;
    game.lines = 0;
    game.score = 0;
    for ( int y = 0; y < SCN_HEI_NSAND + 1; y++ ) {
        for ( int x = 0; x < SCN_WID_NSAND; x++ ) {
            game.screen[y][x] = Black;
        }
    }

    /* ミノ情報 */
    next_mino = MakeMino();
    InitMino();

    /* タイマー関数 */
    InitTimer();

    /* 時間 */
    time_prev = time(NULL);
}

/**
 * タイマー関数初期化
 *  引数: なし
 *  返値: なし
 */
static void InitTimer(void) {
    timer_mino = SDL_AddTimer(15, FallMino, NULL);
    timer_clps = SDL_AddTimer(70, CollapseMino, NULL);
    if ( !timer_mino || !timer_clps ) {
        fprintf(stderr, "failed to add timer_id.\n");
        TerminateWindow();
        exit(-1);
    }
}


/*-----終了処理-----*/

/**
 * システム終了関数
 *  引数: なし
 *  返値: なし
 */
void TerminateSystem(void) {
    SDL_RemoveTimer(timer_mino);
    SDL_RemoveTimer(timer_clps);
}


/*-----ロジック・入力処理-----*/

/**
 * ゲームロジック関数
 * 引数: なし
 * 返値: なし
*/
void GameLogic(void) {

    /* 入力処理 */
    if ( InputEvent() == 0 ) {
        game.stts = GS_End;
        return;
    }
    if ( input.down ) {
        game.score++;
    }

    /* ミノ移動・補正 */
    MINO lastmino = mino;
    MoveMino();
    if ( IntersectMino() ) {
        mino = lastmino;
        if ( mino.y + mino.shape.height * BLOCK_SIZE >= SCREEN_H ) {
            ReflectScreen();
            InitMino();
            game.score += 100;
        }
    }

    /* 砂がそろっているかチェック */
    for ( int i = SCN_HEI_NSAND-1; i >= 0; i-- ) {
        Color c = MatchSimilarColor(game.screen[i][0]);
        if ( c == Black ) {
            continue;
        }
        if ( CheckSandComplete(0, i, c) ) {
            RemoveCompleteSand(0, i, c);
            game.score += 1000;
            game.lines++;
        }
    }
}

/**
 * 入力処理関数
 * 引数：なし
 * 返値：0...ゲーム終了, 1...ゲーム続行
*/
static int InputEvent(void) {
    int ret = 1;

    /* 入力読み取り */
    SDL_Event ev;
    if ( SDL_PollEvent(&ev) ) {

        if ( ev.type == SDL_KEYDOWN ) {
            switch ( ev.key.keysym.sym ) {
            case SDLK_ESCAPE:
                ret = 0;
                break;
            case SDLK_c:
                RotateMino(0);
                break;
            case SDLK_UP:
                RotateMino(1);
                break;
            case SDLK_DOWN:
                input.down = 1;
                break;
            case SDLK_RIGHT:
                input.right = 1;
                break;
            case SDLK_LEFT:
                input.left = 1;
                break;
            default:
                break;
            }
        }
        if ( ev.type == SDL_KEYUP ) {
            switch ( ev.key.keysym.sym ) {
            case SDLK_DOWN:
                input.down = 0;
                break;
            case SDLK_RIGHT:
                input.right = 0;
                break;
            case SDLK_LEFT:
                input.left = 0;
                break;
            default:
                break;
            }
        }
        if ( ev.type == SDL_QUIT ) {
            ret = 0;
        }
    }

    return ret;
}


/*-----ミノ関連処理------*/

/**
 * ミノを初期化
 * 引数：なし
 * 返値：なし
*/
static void InitMino(void) {

    mino = next_mino;
    next_mino = MakeMino();

    /* 既存のミノと最新のミノが衝突したら */
    if ( IntersectMino() && game.stts != GS_End ) {
        /* ゲーム終了 */
        fprintf(stderr, "You Lose...!!\n");
        game.stts = GS_End;
    }
}

/**
 * ミノを生成する
 * 引数：なし
 * 返値：ミノ
*/
static MINO MakeMino(void) {

    MINO new_mino;

    int m_type;
    do {
        m_type = rand() % MINO_MAX;
    } while ( m_type == Blank );
    new_mino.shape = shapes[m_type];
    new_mino.x = (SCREEN_W - new_mino.shape.width * BLOCK_SIZE) / 2;
    new_mino.y = 0 - new_mino.shape.height;

    int m_color;
    do {
        m_color = rand() % MAX_N_COLOR;
    } while ( m_color == Black || m_color == White);
    new_mino.color = MatchSimilarColor(m_color);

    return new_mino;
}

/**
 * ミノ移動処理関数
 * 引数：なし
 * 返値：なし
*/
static void MoveMino(void) {
    if ( input.down ) {
        mino.y += 2;
    }
    if ( input.right && !input.left ) {
        mino.x += SAND_SIZE;
    }
    if ( input.left && !input.right ) {
        mino.x -= SAND_SIZE;
    }
}

/**
 * ミノの衝突チェック
 * 引数：なし
 * 返値：衝突したら１, そうでないときは０.
*/
static int IntersectMino(void) {

    for ( int y = mino.shape.height - 1; y >= 0 ; y-- ) {
        for ( int x = 0; x < mino.shape.width; x++ ) {
            if ( mino.shape.pattern[y][x] == 0 ) {
                continue;
            }

            if ( mino.x + x*BLOCK_SIZE < 0 || mino.y + (y+1)*BLOCK_SIZE > SCREEN_H ) {
                return 1;
            }
            if ( mino.x + x*BLOCK_SIZE < 0 || mino.x + (x+1)*BLOCK_SIZE > SCREEN_W ) {
                return 1;
            }

            int x2 = mino.x + x * BLOCK_SIZE;
            int y2 = mino.y + y * BLOCK_SIZE;
            for ( int sy = 0; sy < BLOCK_NSAND; sy++ ) {
                if ( sy == BLOCK_NSAND - 1 ) {
                    for ( int sx = 0; sx < BLOCK_NSAND; sx++ ) {
                        if ( game.screen[y2/SAND_SIZE + sy][x2/SAND_SIZE + sx] != Black) {
                            return 1;
                        }
                    }
                } else {
                    if ( game.screen[y2/SAND_SIZE + sy][x2/SAND_SIZE] != Black ) {
                        return 1;
                    }
                    if ( game.screen[y2/SAND_SIZE + sy][x2/SAND_SIZE + BLOCK_NSAND-1] != Black ) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * ミノの回転
 * 引数：回転方向（0...時計まわり, 1...反時計回り）
 * 返値：なし
*/
static void RotateMino(int orient) {

    MINO newmino = mino;

    switch ( orient ) {
    case 0:
        for ( int y = 0; y < mino.shape.height; y++ ) {
            for ( int x = 0; x < mino.shape.width; x++ ) {
                int nx = y;
                int ny = mino.shape.width - 1 - x;
                newmino.shape.pattern[ny][nx] = mino.shape.pattern[y][x];
            }
        }
        break;
    case 1:
        for ( int y = 0; y < mino.shape.height; y++ ) {
            for ( int x = 0; x < mino.shape.width; x++ ) {
                int nx = mino.shape.height - 1 - y;
                int ny = x;
                newmino.shape.pattern[ny][nx] = mino.shape.pattern[y][x];
            }
        }
        break;
    default:
        break;
    }

    mino = newmino;
}


/**
 * 盤面にミノを書き込む
 * HACK: 5重forループを使っている. 計算量をすくなくしたい.
 * 引数：なし
 * 返値：なし
*/
static void ReflectScreen(void) {

    int bx, by, sx, sy;

    for ( by = 0; by < mino.shape.height; by++ ) {
        for ( bx = 0; bx < mino.shape.width; bx++ ) {

            if ( mino.shape.pattern[by][bx] == Blank ) {
                continue;
            }

            int x = (mino.x + bx * BLOCK_SIZE) / SAND_SIZE;
            int y = (mino.y + by * BLOCK_SIZE) / SAND_SIZE;
            int c = MatchSimilarColor(mino.color);

            for ( int n = 0; n < BLOCK_NSAND/2; n++ ) {
                for ( sy = 0; sy < 8-2*n; sy++ ) {
                    for ( sx = 0; sx < 8-2*n; sx++ ) {
                        game.screen[y+sy+n][x+sx+n] = c;
                    }
                }
                //  色を切り替え
                if (c == MatchSimilarColor(mino.color)) {
                    c++;
                } else {
                    c--;
                }
            }
        }
    }
}


/*-----砂関連処理-----*/

/**
 * 左右の端まで連なった砂の領域を削除する
 * 引数：マスの座標, 削除する色
 * 返値：なし
*/
static void RemoveCompleteSand(int _x, int _y, int c) {
    if ( c == Blank )   return;
    for ( int n = 0; n < 8; n++ ) {
        int x = _x + dx[n];
        int y = _y + dy[n];
        if ( x < 0 || x >= SCN_WID_NSAND || y < 0 || y >= SCN_HEI_NSAND ) {
            continue;
        }

        c = MatchSimilarColor(c);
        if ( game.screen[y][x] == c || game.screen[y][x] - 1 == c ) {
            game.screen[y][x] = Black;
            if ( game.screen[y][x] != c && game.screen[y][x] - 1 != c ) {
                RemoveCompleteSand(x, y, c);
            }
        }
    }
}


/**
 * 砂が左右の端までつながっているかどうかを調べる（左から右へ探索）
 * FIXME: たまに, つながっているものをつながっていないと判定することがある.
 *      （左端に新たな同色のミノを落とすことで解消する場合がほとんど）
 * 引数：座標, 調査する色
 * 返値：1...つながっている, 0...つながっていない
*/
static int CheckSandComplete(int x, int y, Color c) {

    int ret = 0;
    int dv = 0;                     //      進行方向（0:スクリーン右 1:上 2:左 3:下）
    int lv = (dv + 1) % 4;          //      進行方向に対する左方向

    /* 進行方向左に別の色があるように進む */
    while ( 1 ) {
        Color cl_l = game.screen[y + dy[lv]][x + dx[lv]];
        Color cl_d = game.screen[y + dy[dv]][x + dx[dv]];
        cl_l = MatchSimilarColor(cl_l);
        cl_d = MatchSimilarColor(cl_d);
        if ( c != cl_l ) {
            if ( c != cl_d ) {
                dv = (dv + 3) % 4;
                lv = (dv + 1) % 4;
                continue;
            }
        } else {
            dv = (dv + 1) % 4;
            lv = (dv + 1) % 4;
        }
        x += dx[dv];
        y += dy[dv];

        if ( x >= SCN_WID_NSAND - 1 ) {
            ret = 1;
            break;
        }
        if ( x <= 0 ) {
            break;
        }
    }

    return ret;
}


/*-----時間間隔を空ける処理-----*/

/**
 * ミノ落下タイマー関数
 * 引数：インターバル, ポインタ引数（渡す必要なし）
 * 返値：インターバル
*/
Uint32 FallMino(Uint32 interval, void *param) {
    MINO lastmino = mino;
    mino.y += 1;
    if ( IntersectMino() ) {
        mino = lastmino;
        ReflectScreen();
        InitMino();
        game.score += 100;
    }

    return interval;
}

/**
 * 着地したミノを崩壊させる
 * 引数：インターバル, ポインタ引数（渡す必要なし）
 * 返値：なし
*/
Uint32 CollapseMino(Uint32 interval, void *param) {
    int index[SCN_WID_NSAND];
    for ( int n = 0; n < SCN_WID_NSAND; n++ ) {
        index[n] = n;
    }
    ShuffleIntArray(index, SCN_WID_NSAND);

    for ( int row = SCN_HEI_NSAND-2; row >= 0; row-- ) {    //  下の段から落とす
        for ( int idx = 0; idx < SCN_WID_NSAND; idx++ ) {
            int col = index[idx];                           //  列は左右で偏りの出ないようランダムに
            if ( game.screen[row][col] == Black ) {
                continue;
            }
            if ( game.screen[row+1][col] == Black ) {                               //      下に空きがあるとき
                game.screen[row+1][col] = game.screen[row][col];
                game.screen[row][col] = Black;
                continue;
            }
            if ( col < SCN_WID_NSAND-1 && game.screen[row+1][col+1] == Black ) {    //      右下に空きがあるとき
                game.screen[row+1][col+1] = game.screen[row][col];
                game.screen[row][col] = Black;
            }
            if ( col >= 1 && game.screen[row+1][col-1] == Black ) {                 //      左下に空きがあるとき
                game.screen[row+1][col-1] = game.screen[row][col];
                game.screen[row][col] = Black;
            }
        }
    }

    return interval;
}


/*-----その他の処理-----*/

/**
 * int整数配列をシャッフルする
 * 引数: 配列の先頭ポインタ, 要素数
 * 返値: なし
*/
static void ShuffleIntArray(int array[], int n) {
    for ( int cnt = 0; cnt < n; cnt++ ) {
        int r1 = rand() % n;
        int r2 = rand() % n;
        int tmp = array[r1];
        array[r1] = array[r2];
        array[r2] = tmp;
    }
}

/**
 *  似ている色を統一する
 *  引数: カラー
 *  返値: 統一後のカラー
 */
int MatchSimilarColor(int c) {
    switch (c) {
    case Red1:
    case Red2:
        return Red1;
    case Yellow1:
    case Yellow2:
        return Yellow1;
    case Green1:
    case Green2:
        return Green1;
    case Blue1:
    case Blue2:
        return Blue1;
    default:
        return c;
    }
}

/* end of system.c */