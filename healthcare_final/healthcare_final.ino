#include <Servo.h>
#include <LiquidCrystal.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;
Servo servo;

SoftwareSerial mySoftwareSerial(10, 11);
LiquidCrystal lcd(9,8,2,3,4,5);
int pulsePin = 0;                 // 심박수센서 아날로그 0핀

const int SERVO = 6;      // 서보모터 디지털 6핀

int endbutton=A2;        // 종료버튼 아날로그 2핀a
int end_prestate=0;
int endbuttonstate;

int upbutton=A3;        // 증가버튼 아날로그 3핀
int up_prestate=0;
int upbuttonstate;

int downbutton=A4;        // 감소버튼 아날로그 4핀
int down_prestate=0;
int downbuttonstate;

int savebutton=A5;        // 저장버튼 아날로그 5핀
int save_prestate=0;
int savebuttonstate;

volatile int BPM;                   // 심박수
volatile int Signal;                // 심박수 센서의 신호값
volatile int IBI = 600;             // 심장 박동 간격
volatile boolean Pulse = false;     // 실시간 심장 박동 여부
volatile boolean QS = false;        // 심장 박동 감지 여부

int age=0;              // 연령대
int intensity=40;           // 운동 강도
int max_heartrate = 0;           // 최대심박수
int target_heartrate = 0;            // 목표심박수

void setup(){
  Serial.begin(115200);

  mySoftwareSerial.begin(9600);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {                   // dfplayer 연결 여부 확인
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0);
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30);            // dfplayer 음량 설정 0~30

  pinMode(upbutton, INPUT);         // 증가버튼 입력모드 설정
  pinMode(downbutton, INPUT);       // 감소버튼 입력모드 설정
  pinMode(savebutton, INPUT);       // 저장버튼 입력모드 설정
  pinMode(endbutton, INPUT);        // 종료버튼 입력모드 설정

  lcd.begin(16,2);           // lcd 초기화
  
  myDFPlayer.play(1);        // 연령대와 운동강도 설정 알림 재생

  setage();          // 연령대 설정 함수 호출
  setintensity();        // 운동 강도 설정 함수 호출

  interruptSetup();                 // 2ms마다 심박 센서 신호를 읽음

  myDFPlayer.play(2);          // 운동 시작 알림 재생
  delay(3000);
}



void loop(){
  lcd.setCursor(0,0);       // lcd 첫번째줄에 심박수 출력
  lcd.print("Pulse: ");
  lcd.print(BPM);

  endbuttonstate = digitalRead(endbutton);      // 종료버튼 상태

  if(endbuttonstate==HIGH) {        // 종료버튼을 누르기 시작한 시점
    if(end_prestate==0) {
      delay(10);
      end_prestate =1;          // 종료버튼 이전 상태 '눌림'으로 설정
    }
  }
  if(endbuttonstate==LOW) {            // 종료버튼에서 손을 뗀 시점
    if(end_prestate==1) {
      delay(10);
      end_prestate =0;              // 종료버튼 이전 상태 '안 눌림'으로 설정
      lcd.setCursor(0,1);               // lcd 두번째줄에 finish 출력
      lcd.print("Finish!!!!!!!!!!");

      myDFPlayer.play(4);               // 운동 종료 알림 재생
      delay(10000);

      exit(0);               // 종료

    }
  }

  if(BPM == 0) {
    lcd.setCursor(0,1);               // lcd 두번째줄에 check sensor출력
    lcd.print("check sensor!!!!");
  }
  else {
    lcd.setCursor(0,1);               //lcd 두번째줄에 목표심박수 출력
    lcd.print("TargetPulse: ");
    lcd.print(target_heartrate);
  }

  Serial.print("1: ");
  Serial.println(BPM);

  if (QS == true){     // 심작박동을 감지했을 때
    lcd.setCursor(0,0);       // lcd 첫번째줄에 심박수 출력 
    lcd.print("Pulse: ");
    lcd.print(BPM);

    if (BPM > target_heartrate) {         // 심박수가 목표심박수를 초과했을 때
      Serial.println("overoverover");
    
      lcd.setCursor(0,1);                 // lcd 두번째줄에 운동중지 멘트 출력
      lcd.print("Stop exercise!!!");
 
      myDFPlayer.play(3);                // 심박수가 높다는 알림 재생
      delay(3000);
      myDFPlayer.play(4);               // 운동 종료 알림 재생
      delay(3000);

      openservo();
      delay(10000);
      Serial.println("servo end");

      exit(0);
    }

    if(BPM == 0) {
    lcd.setCursor(0,1);               // lcd 두번째줄에 finish 출력
    lcd.print("check sensor");
    }
    else {
      lcd.setCursor(0,1);               //lcd 두번째줄에 목표심박수 출력
      lcd.print("TargetPulse: ");
      lcd.print(target_heartrate);
    }

    Serial.println("2: ");
    Serial.println(BPM);

    QS = false;                      // 심작박동 감지 여부 false로 설정
  }
  
  delay(20);
  lcd.clear();
}



