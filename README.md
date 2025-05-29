# "XIAO ESP32-S3"にST7789ディスプレイをつなぐ
スイッチサイエンスで買った"<b>XIAO ESP32-S3</b>"にSPIディスプレイST7789をつなぐテスト<br>



<br>
最初に動作確認しようと書き込みしたらエラー！しばらくハマったがBootモードにして書き込んでリセットすれば大丈夫とわかった（はずれを引いたかな）それ以外は問題なさそう。<br>
<br>
一日遊んでいたらいつの間にか自動書き込みとリセットが正常に動作するようになった（謎）<br>

# XIAO ESP32-S3について

詳細は
[https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)<br>


# ディスプレイ PIN

* GND          GND
* VCC          3.3V
* SCL(SCLK) → GPIO07
* SDA(MOSI）→ GPIO09
* RES(RST)  → GPIO01
* DC        → GPIO02
* CS        → GPIO04
* BLK       → GPIO05 5 コードで指定

ピンは3.3vピンを基準に適当に配置。結構好きに変更可能。<br>
lovyan03/LovyanGFX@^1.2.7 を使用

GPIO03を最初使ってましたが、Strapping Pinなので良くないって記事読んだので変えました。<br>
[https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)<br>
データシートはちゃんと読まないとダメですね。<br>

<br>
LovyanGFXのヘッダ読んだらちゃんとバックライト処理があったので修正。<br>


## Dependency
Visual studio 2022 C#

## License
This software is released under the MIT License, see LICENSE

## Authors

bry-ful(Hiroshi Furuhashi)<br>
twitter:[bryful](https://twitter.com/bryful)<br>
Mail: bryful@gmail.com<br>

