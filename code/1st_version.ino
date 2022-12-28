#include <Keypad.h>
#include <Servo.h>

int lock = 10;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 5, 3, 4, 2 };
byte colPins[COLS] = { 8, 7, 6 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo servo;
int open = 90;     //open degree
int close = 10;    //close degree
int servoNow = 0;  //open or close state

int pwdLength = 4;
char* pwd = new char[pwdLength];

int charToint(char x) {
  switch (x) {
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
  }
}

void setup() {
  for (int i = 0; i < pwdLength; i++) pwd[i] = '0';
  servo.attach(lock);
  servo.write(close);
  Serial.begin(9600);
  Serial.println(F("초기 비밀번호 : 0000"));
  Serial.println(F("========================================================================================================="));
  Serial.println(F("열린 상태에서 * 입력 시 잠금"));
  Serial.println(F("잠금해제 상태에서 # 입력 시 비밀번호 설정 모드로 진입"));
  Serial.println(F("모드 진입 후 원하는 비밀번호 길이 입력(최소 1자리, 최대 9자리) 이후 설정할 비밀번호 입력"));
  Serial.println(F("처음 입력한 자리수만큼 입력시 비밀번호 설정모드 종료"));
  Serial.println(F("========================================================================================================="));
  servoNow = close;
}

void loop() {
  char key = keypad.waitForKey();

  if (key != NO_KEY) {
    if (servoNow == open) {
      if (key == '#') {
        delete[] pwd;
        Serial.println(F("=================================비밀번호 설정 모드 시작================================="));
        Serial.println(F("몇자리 비밀번호로 설정하시겠습니까?"));
        key = keypad.waitForKey();
        while ((key == '*') || (key == '#') || (key == '0')) {
          Serial.println(F("Please press the number(except 0)"));
          key = keypad.waitForKey();
        }
        pwdLength = charToint(key);
        Serial.print(pwdLength);
        Serial.println(F(" 자릿수 비밀번호 사용."));
        Serial.println(F("사용할 비밀번호 입력 시작"));
        pwd = new char[pwdLength];
        for (int i = 0; i < pwdLength; i++) {
          key = keypad.waitForKey();
          while ((key == '*') || (key == '#')) {
            Serial.println(F("Please press the number"));
            key = keypad.waitForKey();
          }
          pwd[i] = key;
          Serial.print(i + 1);
          Serial.println(F("번째 숫자 입력 완료"));
        }
        Serial.println(F("===================================비밀번호 설정 완료==================================="));
      } else if (key == '*') {
        Serial.println(F("잠금이 설정되었습니다."));
        servoNow = close;
        servo.write(servoNow);
        return;
      } else {
        Serial.println(F("*을 눌러서 잠금"));
      }
    } else if (servoNow == close) {
      if ((key != '*') && (key != '#')) {
        char* pwdInput = new char[pwdLength];
        int i = 1;
        pwdInput[0] = key;
        Serial.println(F("1번째 자리 입력"));

        while (1) {
          if (i < pwdLength) {
            key = keypad.waitForKey();
            while ((key == '*') || (key == '#')) {
              Serial.println(F("Please press the number"));
              key = keypad.waitForKey();
            }
            pwdInput[i] = key;
            Serial.print(i+1);
            Serial.println(F("번째 자리 입력"));
          } else {
            Serial.print(F("입력한 비밀번호 : "));
            for (int k = 0; k < pwdLength; k++) {
              Serial.print(pwdInput[k]);
            }
            Serial.println();
            break;
          }
          i++;
        }

        for (int j = 0; j < pwdLength; j++) {
          if (pwd[j] != pwdInput[j]) {
            Serial.println(F("잘못된 비밀번호입니다."));
            return;
          }
        }
        Serial.println(F("잠금이 해제되었습니다."));
        servoNow = open;
        servo.write(servoNow);
      } else {
        Serial.println(F("숫자를 입력하세요."));
        return;
      }
    }
  }
}
