＃＃＃＃＃＃＃＃＃＃＃＃＃＃＃２０２４－１１－２２＃＃＃＃＃＃＃＃＃＃＃＃＃＃＃＃
ｓｔｍ３２ 펌웨어 기초 － ＵＳＢ ＣＤＣ 


프로젝트 진행 순서 

１。 가상 드라이버 설치 

２。 장치 관리자 포트 확인 ／ 가상 포트 （나의 경우 ＣＯＭ４）열려있는지 확인 

３。 ＣＵＢＥ ＭＸ로 ＵＳＢ 사용설정

３－１。 메뉴에서 ＵＳＢ들어가서 Ｄｅｖｉｃｅ（ＦＳ）체크 ／ 미들웨어에서 ＣＤＣ 모드로 변경 

３－２。 클록설정 리솔브 클록 이슈 버튼으로 자동으로 ＵＳＢ에 맞게 설정해준다。

４。 코드 생성 ／ 톱니바퀴 모양 클릭

５。ｍａｉｎ。ｃ 파일 수정 

５。１ MX_USB_DEVICE_Init(); // USB 장치 초기화

５。２  char *message = "Hello World\r\n"; ／／출력할 문자열 메시지 지정 

５。３ if (HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK) {
      
      Error_Handler();
５。３ HAL_UART_Transmit 함수: 이 함수는 UART를 통해 데이터를 전송하는 기능

５。３ (uint8_t*)message: 전송할 데이터의 포인터. message는 전송할 문자열 "Hello World\r\n"을 말한다。

５。３ (uint8_t*)는 문자열을 바이트 배열로 변환하는 캐스팅

５。３ strlen(message): 전송할 데이터의 길이. strlen 함수는 문자열의 길이를 반환

５。３ HAL_MAX_DELAY: 전송이 완료될 때까지 대기할 최대 시간. 이 값은 무한 대기

이 코드는 UART를 통해 "Hello World" 메시지를 전송하고, 전송이 실패할 경우 오류 처리를 수행하는 구조
