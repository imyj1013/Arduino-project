void openservo()          // 약상자 뚜껑 열기
{
  Serial.println("servo start");
  servo.attach(SERVO);  

  for (int i=180;i>=30;i-=2){          // 뚜껑 천천히 열기
    servo.write(i);
    delay(30);
  }
  delay(2000);

  while(1) {
    endbuttonstate = digitalRead(endbutton);     // 종료 버튼 상태

    if(endbuttonstate==HIGH) {    // 버튼을 누르기 시작한 시점
      if(end_prestate==0) {
        delay(10);
        end_prestate =1;      // 버튼 이전 상태 '눌림'으로 설정
      }
    }
    
    if(endbuttonstate==LOW) {      // 버튼에서 손을 뗀 시점
      if(end_prestate==1) {
        delay(10);
        end_prestate =0;      // 버튼 이전 상태 '안 눌림'으로 설정
        break;             // 반복 종료
      }
    }

  }

  for (int i=30;i<=180;i+=2){          // 뚜껑 천천히 열기
    servo.write(i);
    delay(30);
  }
  delay(2000);

  servo.detach();
  return;

}

