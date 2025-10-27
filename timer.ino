#include <LedControl.h>

LedControl lc = LedControl(11, 13, 10, 2);  // DIN, CLK, CS, デバイス数（2台）

// 0〜9の数字パターン（8×8ドット）
const byte number[10][8] = {
  {B00111100, B01100110, B01100110, B01100110, B01100110, B01100110, B00111100, B00000000}, // 0
  {B00011000, B00111000, B00011000, B00011000, B00011000, B00011000, B01111110, B00000000}, // 1
  {B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110, B00000000}, // 2
  {B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100, B00000000}, // 3
  {B00001100, B00011100, B00111100, B01101100, B01111110, B00001100, B00001100, B00000000}, // 4
  {B01111110, B01100000, B01111100, B00000110, B00000110, B01100110, B00111100, B00000000}, // 5
  {B00111100, B01100110, B01100000, B01111100, B01100110, B01100110, B00111100, B00000000}, // 6
  {B01111110, B00000110, B00001100, B00011000, B00110000, B00110000, B00110000, B00000000}, // 7
  {B00111100, B01100110, B01100110, B00111100, B01100110, B01100110, B00111100, B00000000}, // 8
  {B00111100, B01100110, B01100110, B00111110, B00000110, B01100110, B00111100, B00000000}  // 9
};

// 小数点（右下1点）
const byte dot[8] = {
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B10000000
};

// ピン設定
const int start = 2;
const int stop = 3;
const int timeup = 4;
const int bz = 5;
const int pi = 6;
// int ctime = A0;

bool started = false;

void setup() {
  int co;
  for (int i = 0; i < 2; i++) {
    lc.shutdown(i, false);    // 通常動作モード
    lc.setIntensity(i, 5);    // 輝度（0〜15）
    lc.clearDisplay(i);       // 表示クリア
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



void timer(int t) {
  int ten = t / 10;
  int one = t % 10;

  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, number[one][row]);
    lc.setRow(1, row, number[ten][row]);
  }
}

void loop() {
 
 
  delay(100);
  timer(0);
  if (digitalRead(start) == LOW && !started) {
    started = true;

    // カウントダウン表示（3→1）
    for (int i = 3; i > 0; i--) {
      timer(i);
      digitalWrite(pi, HIGH);
      tone(bz, 262, 200);  // ド
      delay(500);
      digitalWrite(pi, LOW);
      delay(500);
    }

    tone(bz, 523, 1000);  // ドー

    for (int t = 70; t >= 10; t--) {
      if (digitalRead(stop) == LOW) {
        tone(bz, 523, 1000);
        delay(3000);
        started = false;
        return;
      }
    delay(500);
      timer(t);
      digitalWrite(pi, HIGH);
      delay(500);
      
    }

    // 9〜0秒カウント
    for (int t = 9; t >= 0; t--) {
      if (digitalRead(stop) == LOW) {
        tone(bz, 523, 1000);
        started = false;
        return;
      }

      timer(t);
      digitalWrite(pi, HIGH);
      tone(bz, 523, 500);
      delay(500);
      digitalWrite(pi, LOW);
      tone(bz, 0, 100);
      delay(500);
    }

    // 終了音
    digitalWrite(pi, LOW);
    tone(bz, 262, 1000);
    started = false;
  }
}
