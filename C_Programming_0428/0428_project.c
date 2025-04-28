// CHAPTER 03 , 04
//#include<stdio.h>
//
//int main(void)
//{
//	printf("\ac programming\nclass\n");
//	return 0;
//}
/*
#include<stdio.h>
int main(void)
{
	int x;
	float y;

	y = x = 2.5;
	printf("x=%d y=%f \n", x, y);
	return 0;
}
*/
/*
#include<stdio.h>

int main(void)
{
	int x, y, result;

	printf("두 개의 정수를 입력하시오: ");
	scanf_s("%d %d", &x, &y); // 2개의 정수를 동시에 입력받는다.

	result = x + y; // 덧셈 연산을 하여서 결과를 result에 대입
	printf("%d + %d = %d\n", x, y, result);

	result = x - y; // 뺄셈 연산을 하여서 결과를 result에 대입
	printf("%d - %d = %d\n", x, y, result);

	result = x * y; // 곱셈 연산을 하여서 결과를 result에 대입
	printf("%d * %d = %d\n", x, y, result);

	result = x / y; // 나눗셈 연산을 하여서 결과를 result에 대입
	printf("%d / %d = %d\n", x, y, result);
	result = x % y; // 나머지 연산
	printf("%d %% %d = %d\n", x, y, result); // %을 출력하려면 %%하여야 함

	return 0;
}
*/

/*
//최대로 살 수 있는 사탕의 개수 계산하기
#include<stdio.h>

int main(void)
{
	int money;
	int candy_price;

	printf("현재 가지고 있는 돈: ");
	scanf_s("%d", &money);
	printf("캔디의 가격: ");
	scanf_s("%d", &candy_price);

	// 최대한 살 수 있는 사탕 수
	int n_candies = money / candy_price;
	printf("최대로 살 수 있는 캔디의 개수=%d \n", n_candies);

	// 사탕을 구입하고 남은 돈 
	int change = money % candy_price;
	printf("캔디 구입 후 남은 돈=%d \n", change);
	return 0;
}
*/
/*
#include<stdio.h>

int main(void)
{
	int x, y;

	printf("두 개의 정수를 입력하시오: ");
	scanf_s("%d%d", &x, &y);

	printf("%d == %d의 결괏값: %d\n", x, y, x == y);
	printf("%d != %d의 결괏값: %d\n", x, y, x != y);
	printf("%d > %d의 결괏값: %d\n", x, y, x > y);
	printf("%d < %d의 결괏값: %d\n", x, y, x < y);
	printf("%d >= %d의 결괏값: %d\n", x, y, x >= y);
	printf("%d <= %d의 결괏값: %d\n", x, y, x <= y);

	return 0;
}

#include<stdio.h>
int main(void)
{
	const integer MAX = 1000;
	integer i = 10;
	unsigned double d = .2;

	return 0;
}
*/
//CHAPTER 05
//사용자가 입력한 현재의 온도를 검사하여 영상인지 영하인지 출력해주는 프로그램 
/*
#include<stdio.h>

int main(void)
{
	int temp;

	printf("온도를 입력하시오:");
	scanf_s("%d", &temp);

	if (temp > 0)
		printf("영상의 날씨입니다.\n");
	else
		printf("영하의 날씨입니다.\n");

	printf("현재 온도는 %d도입니다.\n", temp);
	return 0;
}
*/

//논리연산자 
/*
#include<stdio.h>

int main(void)
{
	int number;

	printf("정수를 입력하시오: ");
	scanf_s("%d", &number);

	if (number > 0 && number <= 100)
		printf("입력한 정수가 0에서 100 사이에 있습니다. \n");
	else
		printf("입력한 정수가 0에서 100 사이가 아닙니다. \n");

	return 0;
}
*/

//윤년 판단 프로그램 작성 
/*
#include <stdio.h>

int main(void)
{
	int year;

	printf("연도를 입력하시오: ");
	scanf_s("%d", &year);

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		printf("%d년은 윤년입니다.\n", year);
	else
		printf("%d년은 윤년이 아닙니다.\n", year);

	return 0;
}
*/
//산술 계산기 
/*
#include <stdio.h>

int main(void)
{
	char op;
	int x, y;

	printf("수식을 입력하시오 : ");
	scanf_s("%d %c %d", &x, &op, 1, &y);  

	if (op == '+')
		printf("%d\n", x + y);
	else if (op == '-')
		printf("%d\n", x - y);
	else if (op == '*')
		printf("%d\n", x * y);
	else if (op == '/')
	{
		if (y == 0)
			printf("0으로 나눌 수 없습니다.\n");
		else
			printf("%d\n", x / y);
	}
	else
		printf("지원되지 않는 연산자입니다.\n");

	return 0;
}
*/
// 달의 일수를 계산하는 프로그램
/*
#include <stdio.h>

int main(void)
{
	int month, days;

	printf("일수를 알고 싶은 달을 입력하시오: ");
	scanf_s("%d", &month);

	switch (month)
	{
	case 2:
		days = 28;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		days = 30;
		break;
	default:
		days = 31;
		break;
	}
	printf("%d월의 일수는 %d입니다. \n", month, days);
	return 0;
}
*/
//C언어 GUI로 원이나 사각형 그리기 
/*
#include<Windows.h>
#include<stdio.h>

int main(void) {
	HDC hdc = GetWindowDC(GetForegroundWindow());
	char command;

	printf("명령어를 입력하시오(r 또는 c): ");
	command = getchar();

	if (command == 'r') {
	   Rectangle(hdc, 100, 100, 200, 200);
	}
	else if (command == 'c') {
	   Ellipse(hdc, 100, 100, 200, 200);
	}
	else {
		printf("잘못된 명령어입니다. \n");
	}
	return 0;
}
*/
//CHAPTER 06
//반복 구조를 이용하여 1부터 1000까지의 합 구하기 
/*
#include<stdio.h>

int main(void)
{
	int i, sum;

	i = 1;
	sum = 0;
	while (i <= 1000)
	{
		sum += i;
		i++;
	}
	printf("합은 %d입니다.\n", sum);
	return 0;
}
*/
//숫자 맞추기
/*
#include<stdio.h>

int main(void)
{
	int answer = 41;
	int guess;
	int tries = 0;
	// 반복 구조 
	do {
		printf("정답을 추측하여 보시오: ");
		scanf_s("%d", &guess);
		tries++;

		if (guess > answer)
			printf("제시한 정수가 높습니다.\n");
		if (guess < answer)
			printf("제시한 정수가 낮습니다.\n");
	} while (guess != answer);

	printf("축하합니다. 시도횟수=%d\n", tries);
	return 0;
}
*/

//1부터 10까지의 정수를 더하여 합을 구하는 프로그램
/*
#include<stdio.h>

int main(void)
{
	int i , sum;

	sum = 0;
	for (i = 1; i <= 10; i++)
		sum += i;

	printf("1부터 10까지의 정수의 합= %d\n", sum);
	return 0;
}
*/
//중첩 for문을 이용하여 * 기호를 사각형 모양으로 출력하는 프로그램

#include<stdio.h>

int main(void)
{
	int x, y;

	for (y = 0;y < 5; y++)
	{
		for (x = 0;x < 10; x++)
			printf("*");
		printf("\n");
	}
	return 0;
}