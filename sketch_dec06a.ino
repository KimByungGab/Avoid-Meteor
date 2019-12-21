#include <pitches.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// 플레이 스타일 변수
#define NOTHING 0  //게임을 이용중이 아닐 때
#define JOYSTICK 1 //조이스틱으로 이용
#define LIGHTSENSOR 2  //센서로 이용
#define IRSENSOR 3
int playStyle = 0;

// 스위치 관련 변수
int sw1 = 12;

// 조이스틱 관련 변수
int joyStickX = A0; //A0에 조이스틱 X
int joyStickY = A1; //A1에 조이스틱 Y

// 조도 센서 변수
int lightSensor = A3; //조도 센서 핀번호
int touch = 200;  //손가락을 댔을 때 일정 수치 값.

// 적외선 센서 변수
int ir = 3;

// 스피커 관련 변수
int speaker = 13; // 13번에 피에조 스피커
int numTone = 1;  // 한번만 비프음을 줄 거기 때문에 길이는 1번만
int upTone = NOTE_A4;   //가온 라
int downTone = NOTE_C4; //가온 도
int noteDuration = 125; //8분음표(1000 나누기 8)

//4 digit 4 세그먼트 변수
//int position_pin[] = {1, 2, 3, 4};  //4자리 결정 핀
//int segment_pin[] = {5, 6, 7, 8, 9, 10, 11, 12};  //세그먼트 제어 핀
//byte data[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6}; //0~9를 표현하는 세그먼트 값
int val = 0;    //세그먼트에 표시할 점수
int beforeVal = 0;  //난이도를 위해 이전 점수를 저장하는 임시 변수
int gameStartInitVal = 0;  //게임시작 직후의 점수. val이 millis()함수를 사용하기 때문에 게임을 다시 시작할 때 여기서 마이너스를 하여 val 값을 사용한다.
int val4 = 0;   //천 자리 수 담당
int val3 = 0;   //백 자리 수
int val2 = 0;   //십 자리 수
int val1 = 0;   //일 자리 수
//int digitpin[5] = {-1, position_pin[3], position_pin[2], position_pin[1], position_pin[0]};
//#define _0 {    HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}
//#define _1 {    LOW, HIGH, HIGH, LOW, LOW, LOW, LOW}
//#define _2 {    HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH}
//#define _3 {    HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH}
//#define _4 {    LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH}
//#define _5 {    HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH}
//#define _6 {    HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH}
//#define _7 {    HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW}
//#define _8 {    HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}
//#define _9 {    HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH}
//int num_bit[10][7] = { _0, _1, _2, _3, _4, _5, _6, _7, _8, _9 };
int DELAY = 1000;

// TextLCD 관련 변수
int charactorPosition = 1;  // 캐릭터 위치. 1: 상단, 2: 중단, 3: 하단
int difficulty = 1;
int difficultDelay = 100;   // 난이도를 조절하기 위해 만든 딜레이. 10초가 지날때마다 10씩 줄어든다.
// 캐릭터 상단에 있을 때
byte charactor1_up[] = {
  B10000,
  B01000,
  B11110,
  B01000,
  B10000,
  B00000,
  B00000,
  B00000
};
byte charactor1_down[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
// 캐릭터가 중단에 있을 때
byte charactor2_up[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B01000,
};
byte charactor2_down[] = {
  B01000,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
// 캐릭터가 하단에 있을 때
byte charactor3_up[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte charactor3_down[] = {
  B00000,
  B00000,
  B00000,
  B10000,
  B01000,
  B11110,
  B01000,
  B10000
};
// 상단 운석
byte meteor1_up[] = {
  B00111,
  B01111,
  B11111,
  B01111,
  B00111,
  B00000,
  B00000,
  B00000
};
byte meteor1_down[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
// 중단 운석
byte meteor2_up[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111,
  B11111
};
byte meteor2_down[] = {
  B11111,
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
// 하단 운석
byte meteor3_up[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte meteor3_down[] = {
  B00000,
  B00000,
  B00000,
  B00111,
  B01111,
  B11111,
  B01111,
  B00111
};
// 상중단 운석
byte meteor4_up[] = {
  B00111,
  B01111,
  B11111,
  B01111,
  B00111,
  B00111,
  B01111,
  B11111
};
byte meteor4_down[] = {
  B11111,
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
// 상하단 운석
byte meteor5_up[] = {
  B00111,
  B01111,
  B11111,
  B01111,
  B00111,
  B00000,
  B00000,
  B00000
};
byte meteor5_down[] = {
  B00000,
  B00000,
  B00000,
  B00111,
  B01111,
  B11111,
  B01111,
  B00111
};
// 중하단 운석
byte meteor6_up[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111,
  B11111
};
byte meteor6_down[] = {
  B11111,
  B01111,
  B00111,
  B00111,
  B01111,
  B11111,
  B01111,
  B00111
};
//충돌
byte crash[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
//화살표
byte choice[] = {
  B00000,
  B00000,
  B11000,
  B11110,
  B11110,
  B11000,
  B00000,
  B00000
};

LiquidCrystal_I2C lcd(0x27, 16, 2);   //TextLCD에 뿌릴 객체
int meteor_position = 11;  //  운석 위치
int meteor_shape = 1;   // 운석 모양. meteorN의 숫자를 따라간다. ex : meteor_shape = 1이면 meteor1_up과 meteor1_down이 뿌려져야 한다.
int playing = 0;

void initSetting() {
  val = 0;    //세그먼트에 표시할 점수
  val4 = 0;
  val3 = 0;
  val2 = 0;
  val1 = 0;

  meteor_position = 11;  // 운석 위치
  meteor_shape = 1;   // 운석 모양. meteorN의 숫자를 따라간다. ex : meteor_shape = 1이면 meteor1_up과 meteor1_down이 뿌려져야 한다.

  charactorPosition = 1;  // 캐릭터 위치. 1: 상단, 2: 중단, 3: 하단
  
  beforeVal = 0;  //난이도를 위해 이전 점수를 저장하는 임시 변수
  playing = 1;

  gameStartInitVal = millis() / 100;
}
void showSegment() {
  val = (millis() / 100) - gameStartInitVal;
  
  val4 = (val / 1000) % 10;
  val3 = (val / 100) % 10;
  val2 = (val / 10) % 10;
  val1 = val % 10;
//
//  if (val >= 1000)
//  {
//    clearLEDs();
//    pickDigit(4);
//    pickNumber(val4);
//    delayMicroseconds(DELAY);
//  }
//
//  if (val >= 100)
//  {
//    clearLEDs();
//    pickDigit(3);
//    pickNumber(val3);
//    delayMicroseconds(DELAY);
//  }
//
//  if (val >= 10)
//  {
//    clearLEDs();
//    pickDigit(2);
//    pickNumber(val2);
//    delayMicroseconds(DELAY);
//  }
//
//  if (val >= 0)
//  {
//    clearLEDs();
//    pickDigit(1);
//    pickNumber(val1);
//    delayMicroseconds(DELAY);
//  }
}
//void pickDigit(int x)
//{
//  for(int i = 0; i < 4; i++)
//    digitalWrite(position_pin[i], HIGH);
//
//  digitalWrite(digitpin[x], LOW);
//}
//
//void pickNumber(int n) 
//{
//  for(int i = 0; i < 7; i++)
//    digitalWrite(segment_pin[i], num_bit[n][i]);
//}
//
//void clearLEDs()
//{
//  for(int i = 0; i < 8; i++)
//    digitalWrite(segment_pin[i], LOW);
//}

void showLCD() {
  meteor_position--;
  
  if(charactorPosition == 1) {
    lcd.setCursor(0, 0);
    lcd.write(0);
  }
  else if(charactorPosition == 2) {
    lcd.setCursor(0, 0);
    lcd.write(1);
    lcd.setCursor(0, 1);
    lcd.write(2);
  }
  else if(charactorPosition == 3){
    lcd.setCursor(0, 1);
    lcd.write(3);
  }

  if(meteor_shape == 1) {
    lcd.setCursor(meteor_position, 0);
    lcd.write(4);
  }
  else if(meteor_shape == 2) {
    lcd.setCursor(meteor_position, 0);
    lcd.write(5);
    lcd.setCursor(meteor_position, 1);
    lcd.write(6);
  }
  else if(meteor_shape == 3) {
    lcd.setCursor(meteor_position, 1);
    lcd.write(7);
  }
  else if(meteor_shape == 4) {
    lcd.setCursor(meteor_position, 0);
    lcd.write(4);
    lcd.setCursor(meteor_position, 1);
    lcd.write(7);
  }

  if(meteor_position == 0) {
    if(meteor_shape == charactorPosition) {
      lcd.setCursor(0, 0);
      lcd.print("Your score :");
      lcd.setCursor(1, 1);
      lcd.write(3);
      lcd.setCursor(6, 1);
      lcd.print(2);
      lcd.setCursor(11, 1);
      lcd.print(3);
      playing = 0;
      playStyle = 0;
      difficulty = 1;
    }
    if(meteor_shape == 4) {
      if(charactorPosition == 1 || charactorPosition == 3) {
        lcd.setCursor(0, 0);
        lcd.print("Your score :");
        lcd.setCursor(1, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(2);
        lcd.setCursor(11, 1);
        lcd.print(3);
        playing = 0;
        playStyle = 0;
        difficulty = 1;
      }
      else {
        meteor_position = 11;
        meteor_shape = random(1, 5);
      }
    }
    else {
      meteor_position = 11;
      meteor_shape = random(1, 5);
    }
  }

  lcd.setCursor(12, 0);
  lcd.print(val4);
  lcd.setCursor(13, 0);
  lcd.print(val3);
  lcd.setCursor(14, 0);
  lcd.print(val2);
  lcd.setCursor(15, 0);
  lcd.print(val1);

  Serial.println(val);
}
void moveCharactor(int movement) {
  int up = 1;
  int down = 2;

  if(movement == up)
    charactorPosition--;
  else if(movement == down)
    charactorPosition++;

  showLCD();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(speaker, OUTPUT);
  pinMode(ir, OUTPUT);
  pinMode(sw1, INPUT_PULLUP);
  Serial.begin(9600);

//  //4자리 결정 핀 출력용으로 설정
//  for(int i = 0; i < 4; i++) {
//     pinMode(position_pin[i], OUTPUT);
//  }
//
//  //세그먼트 제어 핀 출력용으로 설정
//  for(int i = 0; i < 8; i++) {
//    pinMode(segment_pin[i], OUTPUT);
//  }
  
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Avoid Meteor!");
  lcd.setCursor(0, 1);
  lcd.createChar(0, charactor1_up);
  lcd.createChar(1, charactor2_up);
  lcd.createChar(2, charactor2_down);
  lcd.createChar(3, charactor3_down);
  lcd.createChar(4, meteor1_up);
  lcd.createChar(5, meteor2_up);
  lcd.createChar(6, meteor2_down);
  lcd.createChar(7, meteor3_down);
  delay(2000);
  
}

void loop() {
  int irv = digitalRead(ir);

  Serial.print("irv : ");
  Serial.println(irv);
    
  if(playing) { //플레이 중일 시
    // put your main code here, to run repeatedly:
    int x, y;
    int light = analogRead(lightSensor);
    int irValue = digitalRead(ir);

    x = analogRead(joyStickX);
    y = analogRead(joyStickY);

//    if((val - beforeVal) == 3) {
//      difficultDelay -= 10;
//      beforeVal = val;
//
//      if(difficultDelay == 20)
//        difficultDelay = 100;
//        difficulty = 2;
//    }
    if(difficulty == 1)
      difficultDelay = 150;
    else if(difficulty == 2)
      difficultDelay = 100;
    else if(difficulty == 3)
      difficultDelay = 50;
    
    delay(difficultDelay);
    lcd.clear();
    showLCD();
  
    if(playStyle == JOYSTICK) {
      if(x > 1000 && charactorPosition > 1 && charactorPosition <= 3) {
      moveCharactor(1);
      tone(speaker, upTone, noteDuration);
      }
      else if(x < 10 && charactorPosition < 3 && charactorPosition >= 1) {
        moveCharactor(2);
        tone(speaker, downTone, noteDuration);
      }
    }
//    else if(playStyle == LIGHTSENSOR) {
//      if(light < touch && charactorPosition > 1 && charactorPosition <= 3) {
//        moveCharactor(1);
//        tone(speaker, upTone, noteDuration);
//      }
//      else if(light >= touch && charactorPosition < 3 && charactorPosition >= 1) {
//        moveCharactor(2);
//        tone(speaker, downTone, noteDuration);
//      }
//    }
//    else if(playStyle == IRSENSOR) {
//      if(irValue == HIGH && charactorPosition > 1 && charactorPosition <= 3) {
//        moveCharactor(1);
//        tone(speaker, upTone, noteDuration);
//      }
//      else if(irValue == LOW && charactorPosition < 3 && charactorPosition >= 1) {
//        moveCharactor(2);
//        tone(speaker, downTone, noteDuration);
//      }
//    }

    showSegment();
  }
  else {  //플레이 중이 아닐 시.
    int x, y;
    int light = analogRead(lightSensor);
    int irValue = digitalRead(ir);
    
    x = analogRead(joyStickX);
    y = analogRead(joyStickY);

    if(y > 1000) {
      Serial.print("difficulty : ");
      Serial.print(difficulty);
      if(difficulty == 1) {
        difficulty++;
        lcd.setCursor(1, 1);
        lcd.print(1);
        lcd.setCursor(6, 1);
        lcd.write(3);
        lcd.setCursor(11, 1);
        lcd.print(3);
      }
      else if(difficulty == 2) {
        difficulty++;
        lcd.setCursor(1, 1);
        lcd.print(1);
        lcd.setCursor(6, 1);
        lcd.print(2);
        lcd.setCursor(11, 1);
        lcd.write(3);
      }
    }
    else if(y < 10) {
      Serial.print("difficulty : ");
      Serial.print(difficulty);
      if(difficulty == 3) {
        difficulty--;
        lcd.setCursor(1, 1);
        lcd.print(1);
        lcd.setCursor(6, 1);
        lcd.print(2);
        lcd.setCursor(11, 1);
        lcd.write(3);
      }
      else if(difficulty == 2) {
        difficulty--;
        lcd.setCursor(1, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(2);
        lcd.setCursor(11, 1);
        lcd.print(3);
      }
    }

    Serial.print("sw1 : ");
    Serial.println(sw1);
    if(digitalRead(sw1) == LOW) {
      initSetting();
      lcd.clear();
      lcd.print("Avoid Meteor!");
      playStyle = JOYSTICK;
      delay(2000);
    }

    delay(500);
  }
}
