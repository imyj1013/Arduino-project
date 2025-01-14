volatile int rate[10];                      // 가장 마지막 10개의 ibi를 저장
volatile unsigned long sampleCounter = 0;           // 심장박동 시간을 결정할 때 사용
volatile unsigned long lastBeatTime = 0;           // 가장 마지막 박동시간, ibi계산 시 사용
volatile int P =512;                       // 심장박동 펄스 파형의 마루
volatile int T = 512;                      // 심장박동 펄스 파형의 골
volatile int thresh = 530;                  // 심장박동의 순간을 찾는데 사용
volatile int amp = 0;                     // 펄스 파형의 진폭을 유지하는데 사용
volatile boolean firstBeat = true;          // 합리적인 BPM으로 시작할 수 있도록 속도 배열을 시드하는 데 사용됨
volatile boolean secondBeat = false;           // 합리적인 BPM으로 시작할 수 있도록 속도 배열을 시드하는 데 사용됨

void interruptSetup(){  // 2ms마다 인터럽트 발생
  TCCR2A = 0x02;     // 디지털 핀 3 및 11에서 PWM을 비활성화하고 CTC 모드로 전환
  TCCR2B = 0x06;     // 강제 비교 금지, 256 프리스케일러
  OCR2A = 0X7C;      // 500Hz 샘플 속도의 경우 카운트 상단을 124로 설정
  TIMSK2 = 0x02;     // TIMER2와 OCR2A 간의 일치 시 인터럽트 활성화
  sei();             // 인터럽트 활성화
}


ISR(TIMER2_COMPA_vect){                         // Timer2가 124로 카운트될 때 트리거됨
  cli();                                      // 인터럽트 비활성화

  Signal = analogRead(pulsePin);              // 심박수센서에서 신호를 읽음
  sampleCounter += 2;                         // samplecounter 변수에 2ms 더함
  int N = sampleCounter - lastBeatTime;       // 마지막 박동 후 시간 확인

  if(Signal < thresh && N > (IBI/5)*3){       // 마지막 IBI의 3/5를 대기하여 이색성 노이즈를 방지
    if (Signal < T){                        // 가장 낮은 값 trough에 저장
      T = Signal;
    }
  }

  if(Signal > thresh && Signal > P){          // thresh는 노이즈를 줄여줌
    P = Signal;                             // 가장 높은 값 peak에 저장
  }

  if (N > 250) {
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){            // signal이 thresh보다 크고 pulse가 false이고 N>ibi의 3/5라면
      Pulse = true;                              // pluse에 true 저장
      
      IBI = sampleCounter - lastBeatTime;         // ibi에 마지막 박동 후 시간 저장
      lastBeatTime = sampleCounter;               // lastbeattime에 시간 저장, 다음 펄스에서 이용

      if(secondBeat) {                        // 두번째 박동을 감지한 경우
        secondBeat = false; 
        for(int i=0; i<=9; i++){             // rate 배열에 ibi 저장
          rate[i] = IBI;
        }
      }

      if(firstBeat){                         // 첫 박동 감지한 경우
        firstBeat = false;
        secondBeat = true;
        sei();                               // 인터럽트 활성화
        return;                              // ibi 값 버림
      }

      word runningTotal = 0;                  // ibi 평균 저장할 변수
      for(int i=0; i<=8; i++){                // rate에 저장된 ibi 한칸씩 미루고  runningtotal에 더함
        rate[i] = rate[i+1]; 
        runningTotal += rate[i];
      }
      rate[9] = IBI;
      runningTotal += rate[9];
      runningTotal /= 10;                     // 10개의 ibi 평균 구함
      
      int d = 60000/runningTotal - BPM;       // 새로운 bpm과 가장 최근 bpm의 차 구함
      if (d < 0) {
        d *= -1;
      }

      if (d < 100) {                          // bpm이 100이상 변할 때는 잘못된 심박수가 측정된 것으로 판단하고 제외시킴
        BPM = 60000/runningTotal;               // runningtotal에 저장된 평균 ibi를 이용해 bpm을 계산
        QS = true;                              // qs에 true저장

      }
    }
  }

  if (Signal < thresh && Pulse == true){   // 심박수센서의 signal이 thresh보다 낮고 pulse가 true 이면
    Pulse = false;                         // pulse에 false저장
    amp = P - T;                           // amp에 진폭 저장
    thresh = amp/2 + T;                    // thresh에 peek와 trough의 중간지점 저장
    P = thresh;
    T = thresh;
  }

  if (N > 2500){                           // 2.5초가 지나도 박동이 감지 되지 않은 경우
    thresh = 530;                          // 각 변수 기본값을 다시 설정
    P = 512; 
    T = 512; 
    lastBeatTime = sampleCounter;  
    firstBeat = true; 
    secondBeat = false;     
  }

  sei();                                   // 인터럽트 활성화
}// end isr

