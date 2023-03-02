# コードに関するあれこれ

備忘録の意味も込めて、また解析の連携のためにも、コード設計の意図などを書き連ねます。

## 忙しいあなたへ

以下のプロトコルで実行して下さい。

1. 適切なフォルダにデータを突っ込む
    * 命名規則は`2022b-exp/exp(実験開始日)/a(解析日)/exp(実験開始日).dat`です。
2. `halve_data.C`の***中身のファイルリンクを確認***
3. `halve_data.C`を実行
4. `tdc_calibrat.cpp`の***中身のファイルリンクを確認***
5. `tdc_calibrat.cpp`を実行
6. `adc_calibrat.cpp`の***中身のファイルリンクを確認***
7. `adc_calibrat.cpp`を実行
8. `draw_tdcadc.C`の***中身のファイルリンクを確認***
9. `draw_tdcadc.C`を実行
10. `time_energy.C`の***中身のファイルリンクを確認***
11. `time_energy.C`を実行

今できることはこれくらいです。

TQ補正も未完成なので仕上げておいて下さい。

* [x] input
* [x] $E$ 設定
* [x] ヒストグラム作成(TEST)
* [ ] ヒストグラム描画(TEST)
* [ ] ヒストグラム描画(本番)
* [ ] ヒストグラムをガウスフィッティング
* [ ] ガウスフィッティング中心 $dT$ を取得
* [ ] $dT$-$E$ グラフ描画
* [ ] $dT$-$E$ グラフフィッティング

回りはしますがデータが一切プロットされません。

TESTとコメントしている部分で1枚だけ描くようになっています。
もしうまくいったらその下にあるコメントアウトを外して実行してみて下さい。
あとチェックボックスもつけといて下さい。

## halve_data.C

* 目的
  * データが２連続で現れる
* 仕様
  * `exp0000.dat`で連続する重複を除去
  * 除去後のデータを`exp0000_halved.dat`に出力
  * 一人で現れる重複なしデータの行を標準出力
* 注意
  * データ数上限50万行
  * 重複しないデータあり
    * 一人でフラッと現れる重複しないデータの行を標準出力で羅列。
    * 0227実験を始めた時に佐々木君が気づいてくれました。みなさんでお礼を言いましょう。ありがとう。
  * 0216実験を参照の際はデータ数に注意

## tdc_calibrat.cpp

* 目的
  * TDCのcalibrationを実行
* 仕様
  * calibration式 $time=factor*TDC+ground$ から引数(factor, ground)を取得
    * デフォルト引数として[齊藤の02/27計算結果](#tdc-table)を使用
      * 下記の表の通り
  * コンパイルは以下の手順
    1. `g++ tdc_calibrat.cpp -o tdc_calibrat`
    2. `./tdc_calibrat`
    3. もし引数を変更したければ `./tdc_calibrat 0.3 0.5`
  * `exp0000_halved.dat`のTDCデータをcalibrate
  * calibrated dataを`exp0000_tcalib.dat`に出力
* 注意
  * ***cppファイル***
  * 誤差伝播は未考慮
    * 誤差解析するそこの君は頑張ってね
  * 時間反転は未対応
  * 0216実験を参照の際はデータ数に注意

<a id="tdc-table"></a>
齊藤のTDC calibration data

| 測定日 | factor | ground  |
|:------|:-------|:--------|
| 02/27 | 0.2126 | -6.4172 |
| 12/19 | 0.2459 | 11.313  |

## adc_calibrat.cpp

* 目的
  * ADCのcalibrationを実行
* 仕様
  * calibration式 $energy=factor*ADC+ground$ から引数(factor, ground)を取得
    * デフォルト引数として[齊藤の02/27計算結果](#tdc-table)を使用
      * 下記の表の通り
    * 第１変数から順に ADC1 factpr, ADC1 ground, ADC2 factor, ADC2 ground
  * コンパイルは以下の手順
    1. `g++ adc_calibrat.cpp -o adc_calibrat`
    2. `./adc_calibrat`
    3. もし引数を変更したければ `./adc_calibrat 0.3 0.5 0.2 0.1`
  * `exp0000_tcalib.dat`のADCデータをcalibrate
  * calibrated dataを`exp0000_acalib.dat`に出力
* 注意
  * ***cppファイル***
  * 誤差伝播は未考慮
    * 誤差解析するそこの君は頑張ってね
  * 0216実験を参照の際はデータ数に注意

<a id="tdc-table"></a>
齊藤のADC calibration data

| NaI No. | factor | ground  |
|------:|:-------|:--------|
| 1 | 0.7355 | -245.15 |
| 2 | 0.6937 | -162.87 |

## draw_tdcadc.C

* 目的
  * TDC, ADCのヒストグラムを描画
* 仕様
  * `exp0000_acalib.dat`のデータをヒストグラムに
  * 作成したヒストグラムを同時に出力
    * 1枚しか見えない時はウィンドウが重なっています
  * ヒストグラムをpdfで保存
    * ファイル名は `tdc.pdf`, `adc1.pdf`, `adc2.pdf`

## time_energy.C

* 目的
  * TDC と ADC の分布を2次元カラープロットで表示
* 仕様
  * `exp0000_acalib.dat` のデータを2次元ヒストグラムに
* 注意
  * ADC1, ADC2を同時に出力するのには未対応なので、2を見たければコードを書き換えて下さい。
  * 齊藤の個人パソコンで動きません。理由は知らん。

## TQcorrection.C

* 目的
  * TQ補正
* 仕様
  * `exp0000_acalib.dat`のデータをTQ補正
  * 各エネルギー幅でのヒストグラムを描画
* 注意
  * 現在鋭意製作中
  * なんかヒストグラムが描画されないですね。
