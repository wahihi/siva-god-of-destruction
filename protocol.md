# Introduction #

라즈베리파이와 연결된 SIVA에서, bluetooth를 pairing 연결 및 데이타 교환시 넘어가는 메시지에 대한 정리를 한다.


# Details #

  * SIVA 대 RPI pairing 시 메시지

> +CONNECTING<<00:1a:7d:da:71:03
> +RNAME=raspberrypi-0
> +CONNECTED

  * 데이타 보냈을때 메시지
    * 11.0 22.0 +DISC:SUCCESS

  * SIVA 대 RPI disconnect시 메시지
> > +READY
> > +PAIRABLE

# Protocol 규칙 #

  * 6하원칙에 근거해서 작성
  * 육하원칙(六何原則)은 기사문에 반드시 들어가야 할 여섯가지 요소를 말한다.
> > 여섯가지 요소는 (순서대로)누가(who), 언제(when), 어디서(where), 무엇(what), 어떻게(how), 왜(why)이다.
> > 영어 단어의 머릿글자를 따서 5W1H라고도 한다.

~~= SIVA의 경우 3W1H 를 적용한다 =~~

**~~언제(when), 어디서(where), 무엇(what), 어떻게(how)~~**

~~= SIVA Protocol =~~
~~~~Header : 3W1H~~
 언제(when)어디서(where)무엇(what)어떻게(how)~~

# Main Station  -> SIVA #

  * Device Act (command) ->  Response (Device Act가 제대로 수행 유무 Ack 날림)
  * Status Info (모든 device의 상태 정보 요청) ->  Response ( 모든 정보 전달 )

# Device Act (Main Station 요청) #
  * Device Act (command) ->  Response (Device Act가 제대로 수행 유무 Ack 날림)
  * SIVA 본체에 대한 명령. SIVA-[GO,STOP,BACK,TURN\_LEFT,TURN\_RIGHT,TURBO]
  * SIVA는 action. 실행이 완료 된 후, 실행 명령을 command log에 남기고, Ack를 main station에 보내준다. 실행이 fail 후, 현재 status를 전송한다.

# Status Info (Main Station 요청) #
  * 아래의 정보들을 전송한다.
  * SoundSensor.
  * TemperatureSensor.
  * MoistureSensor.
  * ULTRA상태,
  * Turbo모드 상태.
  * BlueTooth 강도.(블루투스 강도로 SIVA의 위치를 측정 가능한지 확인하자)
  * Command Log를 전달.

# Example Code #
  * SIVA에서 header 와 tail 을 담당하는 part는 "SIVA" 와 "END" 이다.
  * header는 startsWith 메소드, tail은 endsWith 메소드로 체크 한다.

# SIVA message Read sequence #
  * header가 날라 오는지, startsWith 메소드를 사용해서 지켜 본다. [구현완료]
  * header를 확인하면, tail이 될때까지 1byte씩 적재를 한다. [구현완료]
  * endsWith 메소드로 tail 을 확인하고, 적재된곳에서 buffer에서 tail을 제거한다.[구현완료]
  * 다음 메시지를 처리하는 코드 구현완료. SIVAgoEND, SIVAstopEND, SIVAbackEND, SIVAturnleftEND, SIVAturnrightEND, SIVAturboEND  [구현완료]
  * 수령한 message를 message process에서 처리한다.  [구현완료]
  * message 처리가 정상적으로 완료, 혹은 비정상 처리에 대해 main station쪽으로 response를 보내 준다. [구현완료]