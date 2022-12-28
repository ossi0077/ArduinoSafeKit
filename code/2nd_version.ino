/*

초기 비밀번호 0000, 비밀번호는 4자리로 고정
* 을 입력하여야 비밀번호 입력모드 시작
맞는 비밀번호 입력하여 open 된 경우 #을 눌러 비밀번호 변경 모드로 진입하거나
*를 눌러서 다시 잠글 수 있습니다.
비밀번호 변경방법 : 변경 모드 진입 후 숫자 4자리 입력 시 close 되며 변경 완료됩니다.

*/

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
char pwd[4];

char NoneNumberPressed(char m_key) {
  while ((m_key == '*') || (m_key == '#')) m_key = keypad.waitForKey();
  return m_key;
}

void setup() {
  for (int i = 0; i < pwdLength; i++) pwd[i] = '0';
  servo.attach(lock);
  servo.write(close);
  servoNow = close;
}

void loop() {
  char key = keypad.waitForKey();

  if (key != NO_KEY) {
    if (servoNow == open) {
      if (key == '#') {
        for (int i = 0; i < pwdLength; i++) {
          key = keypad.waitForKey();
          pwd[i] = NoneNumberPressed(key);
        }
        servoNow = close;
        servo.write(servoNow);
        return;
      } else if (key == '*') {
        servoNow = close;
        servo.write(servoNow);
        return;
      }
    } else if (servoNow == close) {
      if (key == '*') {
        char pwdInput[4];
        for(int i=0;i<pwdLength;i++){
          key = keypad.waitForKey();
          pwdInput[i] = NoneNumberPressed(key);
        }
        for (int j = 0; j < pwdLength; j++) if (pwd[j] != pwdInput[j]) return;
        servoNow = open;
        servo.write(servoNow);
      } else return;
    }
  }
}