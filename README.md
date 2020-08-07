ほぼ高校時代に作ったコンピューターオセロの思考部分。かなり古いので作りは悪くソースも汚い。一部例外あり。

## ビルド

Microsoft Visual C++ Toolkit 2003、Platform SDK等をインストールしてパスを通した後、各batを実行。

## tool

### 001_\*.c ～ 007_\*.c

基本となる評価値データを生成する。7種類。0:空所,1:黒,2:白で、白にとっての評価値。

### bin2bmp.c

バイナリファイルからbmpを生成する。別途、bmpをpngに変換後、JavaScriptでcanvasからバイナリを読むのに利用。
[bthello](https://github.com/kozok-dev/bthello/)向けに開発。

### expand1.c、expand2.c

評価値データを15段階にする。

### kec.c

評価値データからC言語の配列(eval.c)を生成する。

### kobbook.c、kobook.c

定石棋譜データから定石盤面データを生成する。
kobbook.cは[bthello](https://github.com/kozok-dev/bthello/)向けに開発。

### koefl.c

機械学習で評価値データを生成する。

### kompc.c

MPCデータを生成する。

### nke.c、nkeip.c

0:空所,1:黒,2:白 → 0:黒,1:白,2:空所、白 → 黒にとっての評価値に変換する。

### reduce.c

評価値データを15 → 13段階にする。

## private

もともと非公開だった分析機能等を追加したバージョン。
