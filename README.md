# QuickJS_ESP32_IoT_Device

電子書籍「M5StackとJavascriptではじめるIoTデバイス制御」のサポートサイトです。<br>
https://poruruba.booth.pm/items/3735175
<br>
<br>
<br>
以下のQiitaでも適時機能拡張を紹介しています！<br>
https://qiita.com/poruruba

## 更新履歴
- 2022-3-26
  - UDP通信機能の追加
  - SDをセマフォで排他制御
  - モジュールサイズオーバーチェックを追加
- 2022-3-27
  - Audio機能の追加
  - Javascript実行の一時停止/再開の追加
- 2022-03-31
  - 不揮発メモリ操作の追加 
  - 一部動作不備の修正
- 2022-4-2
  - DAC出力のAudioに対応
  - M5Stack Fireでの動作を確認
- 2022-4-8
  - Syslog出力を追加
  - 試験的にBlocklyに対応
- 2022-4-9
  - WebAPI・BlocklyからLCDを操作できるように追加
- 2022-4-10
  - MQTT機能の追加
- 2022-4-16
  - 温湿度センサ(SHT30、DH12)の追加
- 2022-4-29
  - LCDにスプライト表示を追加
- 2022-5-1
  - audioにpause/resumeを追加
  - udpのcheckRecvの仕様を変更
- 2022-5-7
  - 各種M5ユニット用ライブラリを追加
- 2023-2-24
  - AudioのI2SポートをI2S_NUM_1に変更
  - 8ポテンショメータユニットを追加
  - M5StampC3を追加

## 誤記訂正
- 2022-03-31
  - 赤外線送受信とUDPのサンプルを追記 
- 2022-4-2
  - DAC出力のAudio対応に伴い、audio.beginの入力パラメータ仕様を変更 
- 2022-4-8
  - Syslog出力を追加
  - IPアドレス指定方法を変更
  - Blocklyにおいて、Javascript出力でpressになっていたのを修正
- 2022-5-1
  - Audioのdispose方法を修正

## 補足
- 2023-2-24 コンパイル時に「I2S: register I2S object to platform failed」と表示される場合は、以下を修正してください。
  - ファイル：QuickJS_ESP32_IoT_Device\QuickJS_ESP32_Firmware\.pio\libdeps\XXXXXX\ESP Async WebServer\src\WebAuthentication.cpp
  - 74行目付近の「mbedtls_md5_starts(&_ctx);」をコメントアウト
