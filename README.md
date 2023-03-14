# コードに関するあれこれ

備忘録の意味も込めて、また解析の連携のためにも、コード設計の意図などを書き連ねます。

## とりま使えればいいあなたへ

以下のプロトコルで実行して下さい。

1. 適切なフォルダにデータを突っ込む
    * 命名規則は`2022b-exp/exp(実験開始日)/a(解析日)/exp(実験開始日).dat`
2. `halve_data.cpp`の***実験日・解析日を確認***
3. `halve_data.cpp`を実行
4. `tdc_calibrat.cpp`の***実験日・解析日を確認***
5. `tdc_calibrat.cpp`を実行
6. `adc_calibrat.cpp`の***実験日・解析日を確認***
7. `adc_calibrat.cpp`を実行
8. `draw_tdcadc.C`の***実験日・解析日を確認***
9. `draw_tdcadc.C`を実行
10. `time_energy.C`の***実験日・解析日を確認***
11. `time_energy.C`をch1に対して実行
12. `time_energy.C`をch2に対して実行
13. `TQcorrection.C`の***実験日・解析日を確認***
14. `TQcorrection.C`をch1に対して実行
15. `draw_tdcadc_TQ.C`の***実験日・解析日を確認***
16. `draw_tdcadc_TQ.C`をch1に対して実行
19. `time_energy_TQ.C`の***実験日・解析日を確認***
19. `time_energy_TQ.C`をch1に対して実行
17. `TQcorrection.C`をch2に対して実行
18. `draw_tdcadc_TQ.C`をch2に対して実行
19. `time_energy_TQ.C`をch2に対して実行

今あなたにできることはこれくらいです。
他の計算なり資料作成なりをお願いします。

## 全体を通して

* ファイル内リンク一括変更
  * `exp_date` に実験日、`ana_date` に解析日
  * 3/1なら301, 11/22なら1122と記述
* ファアイル内チャンネル一括変更
  * ADC ch1 は変数の宣言にて1, ch2 は 2
  * そのあとコードでは1減らして実行
* ターミナル上cppファイル実行手順
  1. `g++ (ファイル名).cpp -o (ファイル名除cpp)`
  2. `./(ファイル名除cpp) (必要に応じて引数) (引数) ...`
  * 例1 `halve_data`
    1. `g++ halve_data.cpp -o halve_data`
    2. `./halve_data`
  * 例2 `tdc_calib` で引数を 0.1, 0.1 に変更
    1. `g++ tdc_calib.cpp -o tdc_calib`
    2. `./tdc_calib 0.1 0.1`

## halve_data.C

* 目的
  * データが２連続で現れるのを解消
* 仕様
  * `exp????.dat`で連続する重複を除去
  * 除去後のデータを`exp????_halved.dat`に出力
  * 一人で現れる重複なしデータの行を標準出力
* 注意
  * 重複しないデータあり
    * 一人でフラッと現れる重複しないデータの行を標準出力で羅列。
    * 0227実験を始めた時に佐々木君が気づいてくれました。みなさんでお礼を言いましょう。ありがとう。
  * 0216実験を参照の際はデータ数に注意

## tdc_calibrat.cpp

* 目的
  * TDCのcalibrationを実行
* 仕様
  * calibration式 $time=factor*TDC+ground$ から引数(factor, ground)を取得
    * デフォルト引数として[03/03計算結果](#tdc-table)を使用
  * `exp????_halved.dat`のTDCデータをcalibrate
  * calibrated dataを`exp????_tcalib.dat`に出力
* 注意
  * 誤差伝播は未考慮
    * 誤差解析するそこの君は頑張ってね
  * 時間反転は未対応
  * 0216実験を参照の際はデータ数に注意

<a id="tdc-table"></a>
TDC calibration data

| 測定日 | factor | ground  |
|:------|:-------|:--------|
| 02/27 | 0.2126 | -6.4172 |
| 12/19 | 0.2459 | 11.313  |
| 03/03 | 0.2460 | -22.618 |

## adc_calibrat.cpp

* 目的
  * ADCのcalibrationを実行
* 仕様
  * calibration式 $energy=factor*ADC+ground$ から引数(factor, ground)を取得
    * デフォルト引数として[03/03計算結果](#tdc-table)を使用
      * 下記の表の通り
    * 第１変数から順に ADC1 factpr, ADC1 ground, ADC2 factor, ADC2 ground
  * `exp????_tcalib.dat`のADCデータをcalibrate
  * calibrated dataを`exp????_acalib.dat`に出力
* 注意
  * 誤差伝播は未考慮
    * 誤差解析するそこの君は頑張ってね
  * 0216実験を参照の際はデータ数に注意

<a id="tdc-table"></a>
3/3 data

| NaI No. | factor | ground  |
|------:|:-------|:--------|
| 1 | 0.7472 | -113.58 |
| 2 | 0.755 | -134.09 |

(参考)齊藤のADC calibration data

| NaI No. | factor | ground  |
|------:|:-------|:--------|
| 1 | 0.7355 | -245.15 |
| 2 | 0.6937 | -162.87 |

## draw_tdcadc.C

* 目的
  * TDC, ADCのヒストグラムを描画
* 仕様
  * `exp????_acalib.dat`のデータをヒストグラムに
  * 作成したヒストグラムを同時に出力
    * 1枚しか見えない時はウィンドウが重なっています
  * ヒストグラムをpdfで保存
    * ファイル名は `tdc.pdf`, `adc1.pdf`, `adc2.pdf`

## time_energy.C

* 目的
  * TDC と ADC の分布を2次元カラープロットで表示
* 仕様
  * 変数 `channel` で描画したいADCのチャンネルを変更可能
  * `exp????_acalib.dat` のデータを2次元ヒストグラムに
* 注意
  * ADC1, ADC2を同時に出力するのには未対応なので、2を見たければコード内 `channel` を書き換えてください
    * 最上段では **`channel = 1 or 2`**
  * 齊藤の個人パソコンで動きません。理由は知らん。
    * 大学のPCでは動くのでバージョンの問題?

## TQcorrection.C

* 目的
  * TQ補正
* 仕様
  * 引数にADCチャンネルを入力
    * 入力は **1 or 2**
    * デフォルトは 1
  * 各エネルギー幅でのヒストグラムを描画
  * 各ヒストグラムをガウスフィッティング
    * フィッティングは`TQ_Tdistrib1.pdf`に出力
  * エネルギーとガウス曲線の中心をプロット・フィット
    * 結果は`EdT.pdf`に出力
  * `exp????_acalib.dat`のTDCデータを補正
    * 補正データを`exp????_TQcor.dat`に出力
* 注意
  * フィッティングがうまくいっているか都度確認

## draw_tdc_TQ.C

* 目的
  * TQ補正後のTDCデータ描画
* 仕様
  * `exp????_TQcor.dat`のTDCを描画
  * ほかは全て`draw_tdcadc.C`と同じ
* 注意
  * ADCは変わっていないので変更しない
