void setage() {          // 연령대 설정과 최대 심박수 계산
  while(1) {          // 연령대 설정
    upbuttonstate = digitalRead(upbutton);       // 증가버튼 상태
    downbuttonstate = digitalRead(downbutton);        // 감소버튼 상태
    savebuttonstate = digitalRead(savebutton);        // 저장버튼 상태

    if(savebuttonstate==HIGH) {        // 저장버튼을 누르기 시작한 시점
      if(save_prestate==0) {
        delay(10);
        save_prestate =1;         // 저장버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(savebuttonstate==LOW) {         // 저장버튼에서 손을 뗀 시점
      if(save_prestate==1) {
        delay(10);
        save_prestate =0;         // 저장버튼 이전 상태 '안 눌림'으로 설정
        break;                   // 반복종료(나이 설정 완료)
      }
    }

    lcd.setCursor(0,0);       // lcd에 연령대 출력
    lcd.print("age: ");
    lcd.print(age);

    if(upbuttonstate==HIGH) {        // 증가버튼을 누르기 시작한 시점
      if(up_prestate==0) {
        delay(10);
        up_prestate =1;        // 증가버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(upbuttonstate==LOW) {        // 증가버튼에서 손을 뗀 시점
      if(up_prestate==1) {
        age+=10;  
        Serial.println(age);                  // 나이 10 증가
        delay(10);
        up_prestate =0;      // 증가버튼 이전 상태 '안 눌림'으로 설정
      }
    }

    if(downbuttonstate==HIGH) {        // 감소버튼을 누르기 시작한 시점
      if(down_prestate==0) {
        delay(10);
        down_prestate =1;        // 감소버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(downbuttonstate==LOW) {        // 감소버튼에서 손을 뗀 시점
      if(down_prestate==1) {
        age-=10;
        Serial.println(age);                    // 나이 10 감소
        delay(10);
        down_prestate =0;       // 감소버튼 이전 상태 '안 눌림'으로 설정
      }
    }

    delay(20);
    lcd.clear();
  }

  max_heartrate = 207-age*0.7;      // 설정된 연령대의 최대심박수 계산
  Serial.println(max_heartrate);
  return;
}






void setintensity(){           // 운동 강도 설정(퍼센트)과 목표심박수 계산
  while(1) {           // 운동 강도 설정(퍼센트)
    upbuttonstate = digitalRead(upbutton);       // 증가버튼 상태
    downbuttonstate = digitalRead(downbutton);        // 감소버튼 상태
    savebuttonstate = digitalRead(savebutton);        // 저장버튼 상태

    if(savebuttonstate==HIGH) {        // 저장버튼을 누르기 시작한 시점
      if(save_prestate==0) {
        delay(10);
        save_prestate =1;         // 저장버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(savebuttonstate==LOW) {         // 저장버튼에서 손을 뗀 시점
      if(save_prestate==1) {
        delay(10);
        save_prestate =0;         // 저장버튼 이전 상태 '안 눌림'으로 설정
        break;                   // 반복종료(운동강도 설정 완료)
      }
    }

    lcd.setCursor(0,0);            // lcd에 운동강도 출력
    lcd.print("intensity: ");
    lcd.print(intensity);

    if(upbuttonstate==HIGH) {        // 증가버튼을 누르기 시작한 시점
      if(up_prestate==0) {
        delay(10);
        up_prestate =1;        // 증가버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(upbuttonstate==LOW) {        // 증가버튼에서 손을 뗀 시점
      if(up_prestate==1) {
        intensity+=10;
        Serial.println(intensity);           // 운동 강도 10 증가
        delay(10);
        up_prestate =0;      // 증가버튼 이전 상태 '안 눌림'으로 설정
      }
    }

    if(downbuttonstate==HIGH) {        // 감소버튼을 누르기 시작한 시점
      if(down_prestate==0) {
        delay(10);
        down_prestate =1;        // 감소버튼 이전 상태 '눌림'으로 설정
      }
    }

    if(downbuttonstate==LOW) {        // 감소버튼에서 손을 뗀 시점
      if(down_prestate==1) {
        intensity-=10;
        Serial.println(intensity);             // 운동 강도 10 감소
        delay(10);
        down_prestate =0;        // 감소버튼 이전 상태 '안 눌림'으로 설정
      }
    }

    delay(20);
    lcd.clear();
  }

  target_heartrate = max_heartrate * intensity * 0.01;        // 설정된 운동강도에 따른 목표심박수 계산
  Serial.println(target_heartrate);
  return;

}