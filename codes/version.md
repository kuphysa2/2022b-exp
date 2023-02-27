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

今できることはこれくらいです。

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
  * コンパイルは以下の手順
    1. `g++ adc_calibrat.cpp -o adc_calibrat`
    2. `./adc_calibrat`
    3. もし引数を変更したければ `./adc_calibrat 0.3 0.5`
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
|:------|:-------|:--------|
| 1 | 0.0007 | -0.2451 |
| 2 | 0.0007 | -0.1629 |

## draw_tdcadc.C

* 目的
  * TDC, ADCのヒストグラムを描画
* 仕様
  * `exp0000_acalib.dat`のデータをヒストグラムに
  * 作成したヒストグラムを同時に出力
    * 1枚しか見えない時はウィンドウが重なっています
  * ~~ヒストグラムをpdfで保存~~
    * ファイル名は...
    * キャパってるので未対応です。昔はできたけどな。

## time_energy.C

なんか画像が生成されないので、後で対応します。