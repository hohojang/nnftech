/*Exercise
사용자로부터 두 개의 정수 입력 받기.
코드 설명
각 비트 연산 수행 (&, |, ^, ~).
연산 결과 출력.
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	int32_t num1 , num2 ;
	printf("Enter 2 numbers(give space between 2 nos)");
	scanf("%d %d", &num1, &num2);

	printf("Bitwise AND(&):%d\n",(num1 & num2));
	printf("Bitwise OR(|):%d\n",(num1 | num2));
	printf("Bitwise XOR(^):%d\n",(num1 ^ num2));
	printf("Bitwise NOT(~):%d\n",(~num1));


	wait_for_user_input();
}
// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/
/*Exercise
비트 연산을 사용하여 정수가 짝수인지 홀수인지 판별하는 프로그램
코드 설명
입력 받기:   scanf로 정수를 입력받습니다.
비트 연산: num & 1은 num의 최하위 비트를 확인합니다. / 결과가 1이면 홀수, 0이면 짝수입니다.
출력: 적절한 메시지를 콘솔에 출력
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{

	int32_t num1;
	printf("Enter a number:");
	scanf("%d",&num1);

	if(num1 & 1){
		printf("%d is odd number\n",num1);
	}else{
		printf("%d is even number\n",num1);
	}

	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/
/*Exercise
특정 비트를 설정(set)하는 방법
코드설명
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	int32_t num1,output;
	printf("Enter a number:");
	scanf("%d",&num1); //정수 입력

	output = num1 | 0x90; // (16진수 144)비트 or 연산
	printf("[input] [output] : 0x%x 0x%x\n",num1, output); //%x 16진수 출력 포맷


	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/
