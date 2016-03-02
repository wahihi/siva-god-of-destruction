# Introduction #

control by Raspberry Pi(RPI)


# Action Description #

  * SIVA control by Raspberry Pi
  * The first is action.
    * SIVA <=> Raspberry Pi (Data send, Data receive) Check
      * modify the tank control source.
> > > 기존에 main 함수에 있던 tank control 소스를 따로 뽑아서, 해당 함수를 호출해서 동작 시키도록 만든다.
        * [실행 결과는 동영상 촬영 후 YouTube에 남겨두기]
        * [뽀삐와 SIVA의 조우] 라는 제목으로 유투브 올리기]

  * Raspberry Pi send command "Go", "Stop", "Turn Right", "Turn Left" [구현완료]
  * SIVA act after Raspberry Pi command. [구현완료]
  * SIVA response the confirm message to Raspberry Pi [구현완료]
  * PC 키보드에서 단축키(예, go,stop등등은 방향키,센서는 1번등등) Raspberry Pi의 python에서 전체 메세지로 변환해서 SIVA에게 뿌려준다. (우선은 navi 키 입력 받는것을 모르니, 'w','d','a',s','x',]등으로 하고 진행하자.

  * Raspberry Pi가 SIVA에게, 센서 ( 온습도 센서, sound 센서 ) 데이터를 요청.
    * 데이타를 1초에 1회 요청, 반복적으로 시간간격으로 요청,
    * Raspberry Pi는 센서데이타를 DB에 넣고, 주기적으로 메일링 리스트에 등록된 사람에게 데이타를 보내 준다.

> > **~~바이올린 소리를 sound센서로 감지하여, 춤추는 SIVA [보류]~~**

  * 블루투스 전파 강도에 따른, 위치 지정
    * 정지 시켜야 할 지점별로 블루투스 강세를 기록.
    * RPI의 명령에 따라 해당 강도에 맞는 위치로 SIVA 이동.(현관앞, 목욕탕앞, 냉장고앞, 내 위치) 등으로 특정 위치를 지정하면, 기록된 해당 위치로 이동해서 대기하는 기능 구현.
    * 위치 이동시, 처음에는 빨리 가지만, 가까워질 수록 속도를 줄여서, 정확한 지점에 멈추는 기능 구현, DC모터 제어.

# 지형 탐색 기능 #
  * SIVA가 초기, 전체 지형을 이동 하며, 장애물의 위치 확인, 온습도 데이타, Bluetooth 강도 등을 보내면, Raspberry Pi의 Database에 저장 해 둔다.

# 사용자 명령 받는 기능 #
  * 리모컨으로 SIVA를 통해 Raspberry Pi에 명령을 주는 방법 구현.
  * 사용자 -> 리모컨 -> SIVA -> Raspberry Pi 절차로 사용자가 원하는 기능을 setting 하고, 실행하는 기능을 수행.

# 기타 기능 #

  * RPI가 블루투스를 통해, 블루투스 스피커를 control
    * 음악재생, TTS 기능. SIVA 기능에 맞춰 TTS 재생.

  * LG스마트 조명을 RPI가 제어하여, 새로운 기능을 추가

  * Raspberry Pi가 부팅이 완료되면, 자동으로 SIVA와 연결. 기본 정보 습득 후, normal mode에서 동작. ( normal mode에서 무엇을 할것인지를 정의 하자 )

  * 수동 모드로 pc에서 Raspberry Pi에 접속해서, SIVA 제어 하는 기능 구현