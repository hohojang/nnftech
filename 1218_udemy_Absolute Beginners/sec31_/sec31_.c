/*
1. 매크로 함수와 상수 정의
   전처리기 지시문을 활용한 매크로 함수와 상수 정의를 사용해 원의 면적 계산.
*/
#include <stdio.h>

#define PI_VALUE (3.1415f) // π 값을 매크로 상수로 정의
#define AREA_OF_CIRCLE(x)	( (PI_VALUE) * (x) * (x) ) // 원의 면적 계산 매크로 함수

int main(void)
{
	float area_circle; // 원의 면적을 저장할 변수

	// 매크로 함수에서 인자로 수식을 전달했을 때의 계산 동작 확인
	area_circle = AREA_OF_CIRCLE(1+1); // 원의 반지름이 (1+1)일 때 면적 계산

	printf("Area = %f\n", area_circle); // 계산된 면적 출력

	return 0; // 프로그램 정상 종료
}

/*
2. 조건부 컴파일의 활용
   매크로를 사용해 특정 조건에서만 코드를 컴파일하고 실행.
*/
#include <stdio.h>

#define AREA_TRI // 삼각형 면적 계산 활성화

int main()
{
#ifdef AREA_TRI
	float radius = 0; // 원의 반지름 값을 저장할 변수
	printf("This is circle area calculation program\n"); // 원 면적 계산 프로그램 메시지 출력
	fflush(stdout); // 출력 버퍼 비우기

	printf("Enter the radius :"); // 사용자 입력 요청
	fflush(stdout);
	scanf("%f", &radius); // 반지름 입력 받기
	printf("Area of circle = %f\n", (3.1415 * radius * radius)); // 원 면적 계산 결과 출력
	fflush(stdout);
#endif

#ifdef AREA_TRI
	float base, height; // 삼각형의 밑변과 높이를 저장할 변수
	printf("This is Triangle area calculation program\n"); // 삼각형 면적 계산 프로그램 메시지 출력
	fflush(stdout);

	printf("Enter base and height: "); // 밑변과 높이 입력 요청
	fflush(stdout);
	scanf("%f%f", &base, &height); // 밑변과 높이 입력 받기
	printf("Area of triangle = %f\n", (0.5 * base * height)); // 삼각형 면적 계산 결과 출력
#endif

	return 0; // 프로그램 정상 종료
}

/*
3. #if와 defined 연산자의 사용
   매크로가 정의되지 않았을 때 조건부 컴파일과 경고 메시지 처리.
*/
#include <stdio.h>

//#define AREA_CIR  // 원 면적 계산 활성화
//#define AREA_TRI  // 삼각형 면적 계산 활성화

#if !defined(AREA_CIR) && !defined(AREA_TRI) // 두 매크로가 모두 정의되지 않았을 경우
    #warning "No macros defined." // 컴파일 시 경고 메시지 출력
#endif

int main()
{
#ifdef AREA_CIR
    printf("This is circle area calculation program\n"); // 원 면적 계산 프로그램 메시지 출력
    float radius = 0; // 원의 반지름 값을 저장할 변수

    printf("Enter the radius: "); // 반지름 입력 요청
    if (scanf("%f", &radius) != 1 || radius <= 0) { // 입력값 유효성 검사
        printf("Error: Invalid input. Radius must be a positive number.\n");
        return -1; // 입력 오류로 프로그램 종료
    }

    // 원의 면적 계산: π * 반지름^2
    printf("Area of circle = %f\n", (3.1415 * radius * radius)); // 계산 결과 출력
#endif

#ifdef AREA_TRI
    printf("This is Triangle area calculation program\n"); // 삼각형 면적 계산 프로그램 메시지 출력
    float base, height; // 삼각형의 밑변과 높이를 저장할 변수

    printf("Enter base and height: "); // 밑변과 높이 입력 요청
    if (scanf("%f%f", &base, &height) != 2 || base <= 0 || height <= 0) { // 입력값 유효성 검사
        printf("Error: Invalid input. Base and height must be positive numbers.\n");
        return -1; // 입력 오류로 프로그램 종료
    }

    // 삼각형의 면적 계산: 0.5 * 밑변 * 높이
    printf("Area of triangle = %f\n", (0.5 * base * height)); // 계산 결과 출력
#endif

    return 0; // 프로그램 정상 종료
}
