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
	printf("�μ��� �� = %d \n", sum);

	return 0;
}
*/

//Chapter 2
//����ڷκ��� 2���� ������ �޾Ƽ� ���ϱ� 
/*
#include <stdio.h>

int main(void)
{
	int x;
	int y;
	int sum;

	printf("ù ��° ���ڸ� �Է��Ͻÿ�:");
	scanf_s("%d", &x);

	printf("�� ��° ���ڸ� �Է��Ͻÿ�:");
	scanf_s("%d", &y);

	sum = x + y;
	printf("�� ���� ��= %d \n", sum);

	return 0;
}
*/
//Chapter 2
//�ؿܿ��� �� ����� ������ִ� ������ ���α׷� �ۼ� 
/*
#include<stdio.h>

int main(void)
{
	int price;
	int sum;
	int days;

	sum = 0;
	printf("������ ����ΰ���?: ");
	scanf_s("%d", &days);

	printf("�װ��� ����: ");
	scanf_s("%d", &price);
	sum = sum + days * price;

	printf("============================================\n");
	printf("�� ���� ���: %d \n", sum);
	printf("============================================\n");

	return 0;
}
*/
//Chapter 2
// ����ڷκ��� 2���� ������ �޾Ƽ� +,-,*,/ ������ �Ͽ� ȭ�鿡 ����ϴ� ���α׷��� �ۼ� 
/*
#include<stdio.h>

int main(void)
{
	int x;
	int y;
	int result;

	printf("ù ��° ���ڸ� �Է��Ͻÿ�:");
	scanf_s("%d", &x);
	printf("�� ��° ���ڸ� �Է��Ͻÿ�:");
	scanf_s("%d", &y);

	result = x + y;
	printf("�� ���� �� = %d \n", result);
	result = x - y;
	printf("�� ���� �� = %d \n", result);
	result = x * y;
	printf("�� ���� �� = %d \n", result);
	result = x / y;
	printf("�� ���� �� = %d \n", result);

	printf("%d * %d = %d \n", 2, 3, 2 * 3);
	return 0;
}
*/

//Chapter 3
//�޷��� ��ȭ�� ����ϱ� 
/*
#include<stdio.h>
#define EXCHANGE_RATE  1120 

int main(void)
{
	int usd; // �޷�ȭ
	int Krw; // ��ȭ 

	printf("�޷��� �ݾ��� �Է��Ͻÿ�: ");
	scanf_s("%d", &usd);
	Krw = EXCHANGE_RATE * usd;
	printf("�޷�ȭ %d�޷��� %d�� �Դϴ�.\n", usd, Krw);

	return 0;
}
*/

