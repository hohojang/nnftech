//Chapter 1 
/*
#include <stdio.h>

int main(void)
{
	printf("Hello World");
	return 0;
}
*/
//Chapter 2

/*
#include <stdio.h>

int main(void)
{
	printf("Hello World\n");
	printf("from ChulSoo");
	return 0;
}
*/
//Chapter 2
/*
#include <stdio.h>

int main(void)
{
	printf("3 x 1 = 3\n");
	printf("3 x 2 = 6\n");
	printf("3 x 3 = 9\n");
}
*/
//Chapter 2
/*
#include <stdio.h>

int main(void)
{
	int x;
	int y;
	int sum;

	x = 100;
	y = 200;

	sum = x  +  y;
	printf("두수의 합 = %d \n", sum);

	return 0;
}
*/

//Chapter 2
//사용자로부터 2개의 정수를 받아서 더하기 
/*
#include <stdio.h>

int main(void)
{
	int x;
	int y;
	int sum;

	printf("첫 번째 숫자를 입력하시오:");
	scanf_s("%d", &x);

	printf("두 번째 숫자를 입력하시오:");
	scanf_s("%d", &y);

	sum = x + y;
	printf("두 수의 합= %d \n", sum);

	return 0;
}
*/
//Chapter 2
//해외여행 총 비용을 계산해주는 간단한 프로그램 작성 
/*
#include<stdio.h>

int main(void)
{
	int price;
	int sum;
	int days;

	sum = 0;
	printf("여행은 몇박인가요?: ");
	scanf_s("%d", &days);

	printf("항공권 가격: ");
	scanf_s("%d", &price);
	sum = sum + days * price;

	printf("============================================\n");
	printf("총 여행 비용: %d \n", sum);
	printf("============================================\n");

	return 0;
}
*/
//Chapter 2
// 사용자로부터 2개의 정수를 받아서 +,-,*,/ 연산을 하여 화면에 출력하는 프로그램을 작성 
/*
#include<stdio.h>

int main(void)
{
	int x;
	int y;
	int result;

	printf("첫 번째 숫자를 입력하시오:");
	scanf_s("%d", &x);
	printf("두 번째 숫자를 입력하시오:");
	scanf_s("%d", &y);

	result = x + y;
	printf("두 수의 합 = %d \n", result);
	result = x - y;
	printf("두 수의 차 = %d \n", result);
	result = x * y;
	printf("두 수의 곱 = %d \n", result);
	result = x / y;
	printf("두 수의 몫 = %d \n", result);

	printf("%d * %d = %d \n", 2, 3, 2 * 3);
	return 0;
}
*/

//Chapter 3
//달러를 원화로 계산하기 
/*
#include<stdio.h>
#define EXCHANGE_RATE  1120 

int main(void)
{
	int usd; // 달러화
	int Krw; // 원화 

	printf("달러와 금액을 입력하시오: ");
	scanf_s("%d", &usd);
	Krw = EXCHANGE_RATE * usd;
	printf("달러화 %d달러는 %d원 입니다.\n", usd, Krw);

	return 0;
}
*/

