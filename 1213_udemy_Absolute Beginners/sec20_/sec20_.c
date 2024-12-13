/*Example
'while' 루프를 사용하여 1부터 10까지 출력하는 프로그램을 작성
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	uint8_t num = 1;
	while(num <= 10){
		printf("%d\n",num++);
	}
	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}

/*Example
0에서 100 사이의 모든 짝수를 출력하는 프로그램을 작성
'while' 루프 사용
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	int32_t start_num , end_num;
	uint32_t even;
	printf("Enter starting and ending numbers(give space between 2 nos):");
	scanf("%d %d", &start_num,&end_num);

	if(end_num < start_num)
	{
		printf("ending number should be > starting number\n");
		wait_for_user_input();
		return 0;
	}

	printf("Even numbers are :\n");
	even = 0;
	while (start_num <= end_num) {
	    if (!(start_num % 2)) {
	        printf("%4d ", start_num); // '\t' 대신 공백 사용
	        even++;
	    }
	    start_num++;
	}
	printf("\n");
	printf("\nTotal even numbers : %u\n",even);
	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/
/*Example
0에서 100 사이의 모든 짝수를 출력하는 프로그램을 작성
'for'문 사용
 */
/*
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	int32_t start_num , end_num;
	uint32_t even;
	printf("Enter starting and ending numbers(give space between 2 nos):");
	scanf("%d %d", &start_num,&end_num);

	if(end_num < start_num)
	{
		printf("ending number should be > starting number\n");
		wait_for_user_input();
		return 0;
	}

	for(printf("Even numbers are :\n"),even = 0;start_num <= end_num ; start_num++)
	{
		if( ! (start_num % 2)){
			printf("%d\t", start_num);
			even++;
		}
	}

	printf("\nTotal even numbers : %u\n",even);
	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/

//사용자가 입력한 높이에 맞춰 숫자로 이루어진 피라미드 모양을 출력하는 프로그램
#include <stdint.h>
#include <stdio.h>

void wait_for_user_input(void);

int main(void)
{
	uint32_t height;
	printf("Enter height of pyramid: ");
	scanf("%d",&height);

	//for 문 동작 부분
	for(uint32_t i = 1 ; i <= height ; i++)
	{
		for(uint32_t j = i; j > 0 ; j--)
		{
			printf("%d ", j);
		}
		printf("\n");
	}

	wait_for_user_input();
}
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
