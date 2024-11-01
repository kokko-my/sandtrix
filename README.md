# 砂テトリス - SANDTRIX

C言語で作成した砂テトリスゲームです．SDL2ライブラリを使用しています．

## 概要

- テトリスのようにブロックを落下させ，行を揃えるゲームです．
- しかし，ブロックは着地と同時に砂へと崩壊します．
- うまく操作してスコアを稼ぎましょう．

![gameplay](https://github.com/user-attachments/assets/6f47e541-251d-420b-b9b4-cee1bdac9fad)

## ビルド方法
### 必要環境

- C99準拠のCコンパイラ
- SDL2ライブラリ

### ビルド手順

1. このリポジトリをクローンします．
2. SDL2ライブラリをインストールします．
3. srcディレクトリに移動し，`make`コマンドで実行してビルドします．

### 実行方法

コマンドラインで`./sand`と入力し実行すると，ゲームが起動します．

## ゲームの操作方法

- 矢印キー（←↓→）: ブロックの移動
- 矢印キー（↑），Cキー: ブロックの回転（それぞれ反時計回り，時計回り）
- Escキー: ゲームの強制終了

## ライセンス

### 使用したライブラリ

- SDL2ライブラリ（https://www.libsdl.org/license.php ）

### 使用音源

- BGM: 都会的なBGM1 by スプリンギン（https://www.springin.org/ ）
- 効果音: 回復魔法1 by 効果音ラボ（https://soundeffect-lab.info/)

### 参考にしたもの

- ゲーム「SANDTRIS」（FRANCO MIRANDA氏による）
    - ゲーム画面の外観を参考にしました．
- 動画「テトリスを小一時間で作ってみたⅡ【C言語ゲームプログラミング実況】Programming Tetris」
（https://www.youtube.com/watch?v=BJs29RicyPw&t=891s ）
    - テトリスの基本的なロジックを参考にしました．
