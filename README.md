# Arduino 8x8 LEDマトリクス タイマー制御 説明書

## 使用ライブラリ

* `LedControl.h` : 8x8 LEDマトリクス制御用ライブラリ

## ピン接続

| ピン | 接続        |
| -- | --------- |
| 11 | DIN       |
| 13 | CLK       |
| 10 | CS        |
| 2  | デバイス数（2台） |
| 2  | Startボタン  |
| 3  | Stopボタン   |
| 4  | TimeUp出力  |
| 5  | Buzzer    |
| 6  | Piezo     |

## LEDパターン定義

* 数字0〜9を8x8ドットで表示
* 小数点用のパターンも定義

```cpp
const byte number[10][8] = { ... };
const byte dot[8] = { ... };
```

## 変数

```cpp
bool started = false;
```

* `started` : タイマーが開始中かどうかのフラグ

## setup()

```cpp
void setup() {
  for (int i = 0; i < 2; i++) {
    lc.shutdown(i, false); // 通常動作モード
    lc.setIntensity(i, 5); // 輝度設定
    lc.clearDisplay(i);    // 表示クリア
  }
  pinMode(start, INPUT_PULLUP);
  pinMode(stop, INPUT_PULLUP);
  pinMode(timeup, OUTPUT);
  pinMode(pi, OUTPUT);
  pinMode(bz, OUTPUT);

  digitalWrite(timeup, LOW);
  digitalWrite(bz, LOW);
  digitalWrite(pi, LOW);
}
```

* LEDマトリクス初期化
* ボタン・出力ピン設定

## timer() 関数

```cpp
void timer(int t) {
  int ten = t / 10;
  int one = t % 10;

  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, number[one][row]);
    lc.setRow(1, row, number[ten][row]);
  }
}
```

* 引数 `t` の数字を2桁でLEDマトリクスに表示

## loop() 処理

* Startボタン押下でタイマー開始
* カウントダウン 3→1 の表示と音出力
* メインカウントダウン（70〜10秒）の表示と音出力
* 最後の9〜0秒のカウント
* Stopボタンで途中停止可能
* タイマー終了時に終了音を鳴らす

```cpp
void loop() {
  delay(100);
  timer(0);
  if (digitalRead(start) == LOW && !started) {
    started = true;

    // カウントダウン 3→1
    for (int i = 3; i > 0; i--) { ... }

    // メインカウントダウン 70〜10秒
    for (int t = 70; t >= 10; t--) { ... }

    // 最後の9〜0秒カウント
    for (int t = 9; t >= 0; t--) { ... }

    // 終了音
    digitalWrite(pi, LOW);
    tone(bz, 262, 1000);
    started = false;
  }
}
```

## 注意点・改善ポイント

1. `delay()` が多用されており、他の処理を同時に行えない
2. 音やLED点灯を非同期にするとスムーズに動作
3. LEDマトリクスの数字表示は2台使用で2桁表示
4. Start/Stopボタンはプルアップを使用し、押下でLOWを検出
