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

//////////2024 - 12 - 13//////////

/*Exercise 강의 98
사용자의 소득세를 계산하는 프로그램을 작성하여라
사용자가 연간 소득을 입력
사용자가 입력한 소득에 맞는 세금을 계산
세금 계산 후, 결과를 출력
*/
/*
#include<stdio.h>
#include<stdint.h> // uint64_t 데이터 타입 사용을 위한 헤더 파일

// 사용자 입력을 대기하는 함수 선언
void wait_for_user_input(void);

int main(void)
{
   uint64_t income; // 소득을 저장하는 변수
   uint64_t tax;    // 계산된 세금을 저장하는 변수

   double temp_income; // 사용자가 입력한 소득 (실수형 데이터로 임시 저장)

   // 사용자로부터 소득 입력
   printf("Enter your total income: "); // "총 소득을 입력하세요: "
   scanf("%lf", &temp_income); // 실수형 데이터를 입력받음

   // 소득이 음수인 경우 처리
   if(temp_income < 0){
      printf("Income cannot be negative.\n"); // "소득은 음수가 될 수 없습니다."
       wait_for_user_input();
       return 0; // 프로그램 종료
   }

   // 입력된 실수형 소득을 정수형으로 변환
   income = (uint64_t) temp_income;

   // 소득 구간에 따라 세금 계산
   if(income <= 9525){
       tax = 0; // 소득이 $9,525 이하인 경우 세금 없음
   } else if((income > 9525) && (income <= 38700)){
       tax = income * 0.12; // $9,525 ~ $38,700 구간: 12% 세율
   } else if((income > 38700) && (income <= 82500)){
       tax = income * 0.22; // $38,700 ~ $82,500 구간: 22% 세율
   } else if(income > 82500){
       tax = income * 0.32; // $82,500 초과 구간: 32% 세율
       tax = tax + 1000; // 추가 세금 $1,000 부과
   }

   // 계산된 세금 출력
   printf("Tax payable: $%I64u\n", tax); // "납부할 세금: $"

   // 프로그램 종료 전 사용자 입력 대기
   wait_for_user_input();

   return 0; // 프로그램 정상 종료
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void) {
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
*/
/*switch-case를 사용하여
Exercise 다양한 도형의 면적을 계산하는 프로그램*/
#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);

int main(void)
{
	int8_t code;
	float r , b , h , a;
	float area;
	printf("Area calculation program\n");
	printf("Circle        ---> c\n");
	printf("Triangle      ---> t\n");
	printf("Trapezoid     ---> z\n");
	printf("Square        ---> s\n");
	printf("Rectangle     ---> r\n");
	printf("Enter the code here:");
	scanf("%c",&code);

	switch(code){
	case 'c':
		printf("Circle Area calculation\n");
		printf("Enter radius(r) value:");
		scanf("%f", &r);
		if(r < 0){
			printf("radius cannot be -ve\n");
			area =-1;
		}else{
		area = 3.1415 * r * r;
		}
		break;
	case 't':
		printf("Triangle Area calculation\n");
		printf("Enter base(b) value:");
		scanf("%f", &b);
		printf("Enter height(h) value:");
		scanf("%f", &h);
		if((b < 0) || ( h < 0 )){
			printf("base or height cannot be -ve\n");
			area =-1;
		}else{
			area = (b * h)/2;
		}
		break;
	case 'z':
		printf("Trapezoid Area calculation\n");
		printf("Enter base1(a) value:");
		scanf("%f", &a);
		printf("Enter base2(b) value:");
		scanf("%f", &b);
		printf("Enter height(h) value:");
		scanf("%f", &h);
		if( ( (a < 0) || ( b < 0 ) || (h < 0) ) ){
			printf("base or height cannot be -ve\n");
			area =-1;
		}else{
			area = ((a + b)/2) * h;
		}
		break;
	case 's':
		printf("Square Area calculation\n");
		printf("Enter side(a) value:");
		scanf("%f", &a);
		if(a < 0){
			printf("side cannot be -ve\n");
			area =-1;
		}else{
			area = a * a;
		}
		break;
	case 'r':
		printf("Rectangle Area calculation\n");
		printf("Enter width(w) value:");
		scanf("%f", &a);
		printf("Enter height(1) value:");
		scanf("%f", &b);
		area = a * b;
		break;
	default:
		printf("Invalid input\n");
		area = -1;
	}
	if(!(area < 0)){
		printf("Area = %f\n", area);
	}
	wait_for_user_input();
}

// 사용자 입력 대기 함수 정의
void wait_for_user_input(void){
   printf("Press Enter key to exit this application"); // "Enter 키를 눌러 프로그램을 종료하세요"
   while (getchar() != '\n'); // 입력 버퍼 비우기
   getchar(); // 사용자 입력 대기
}
