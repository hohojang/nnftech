/*
Exercise
사용자의 나이를 입력받아, 사용자가 투표를 할 수 있는지 결정하는 프로그램을 작성하세요.
투표를 할 수 있는 최소 나이는 18세입니다.
적절한 메시지를 출력하세요.
 */
/*
#include<stdio.h>
#include<stdint.h>

int main(void)
{
	int age = 0;
	printf("Enter your age here :");
	scanf("%d",&age);

	if(age < 18){
		printf("Sorry ! you are not eligible to vote\n");
	}
	if(age >= 18){
		printf("Congrats ! you are eligible to vote\n");
	}


	printf("Press enter key to exit this application");

	while(getchar() != '\n')
	{

	}
	getchar();
}
*/
/*
#include<stdio.h>
#include<stdint.h>

int main(void)
{
	int age = 0;

	printf("Enter your age here :");
	scanf("%d",&age);

	if(age < 18){
		printf("Sorry ! you are not eligible to vote\n");
	}else{
		printf("Congrats ! you are eligible to vote\n");
	}


	printf("Press enter key to exit this application");

	while(getchar() != '\n')
	{

	}
	getchar();
}
*/
/*Exercise 두 숫자 중 더 큰 숫자를 출력하는 프로그램을 작성하는 문제
사용자로부터 두 개의 정수를 입력받고, 두 숫자 중 더 큰 숫자를 출력하세요.
만약 두 숫자가 같으면 "두 숫자는 같습니다."라고 출력
*/
/*
#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);

int main(void) {
    float num1, num2;

    // 첫 번째 숫자 입력 받기
    printf("Enter the first number(integer): ");
    if (scanf("%f", &num1) != 1) {
        printf("Invalid input! Exiting..\n");
        wait_for_user_input();
        return 0;
    }

    // 두 번째 숫자 입력 받기
    printf("Enter the second number(integer): ");
    if (scanf("%f", &num2) != 1) {
        printf("Invalid input! Exiting..\n");
        wait_for_user_input();
        return 0;
    }

    // 정수 값으로 변환
    int32_t n1, n2;
    n1 = num1;
    n2 = num2;

    // 정수 부분만 비교 시 경고 메시지 출력
    if ((n1 != num1) || (n2 != num2)) {
        printf("Warning! Comparing only integer part\n");
    }

    // 숫자 비교
    if (num1 == num2) {
        printf("Numbers are equal\n");
    } else {
        if (num1 < num2) {
            printf("%d is bigger\n", n2);
        } else {
            printf("%d is bigger\n", n1);
        }
    }

    printf("Press enter key to exit this application\n");
    wait_for_user_input();  // 프로그램 종료 대기

    return 0;
}

void wait_for_user_input(void) {
    // 입력 버퍼를 비우고 사용자로부터 Enter 키 대기
    while (getchar() != '\n') {
        // 입력 버퍼 비우기
    }
    getchar();  // 마지막 Enter 키 입력 대기
}
*/

/*Exercise 강의 98
사용자의 소득세를 계산하는 프로그램을 작성하여라
사용자가 연간 소득을 입력
사용자가 입력한 소득에 맞는 세금을 계산
세금 계산 후, 결과를 출력
*/
#include<stdio.h>
#include<stdint.h>

int main(void)
{

}
