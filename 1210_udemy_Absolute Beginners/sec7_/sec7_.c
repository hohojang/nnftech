/*
#include <stdio.h>

void function_add_numbers(int , int , int );

int main()
{
	//함수 호출
	function_add_numbers(12,13,14);
	function_add_numbers(-20,20,14);

	int valueA = 90, valueB = 70;

	function_add_numbers(valueA,valueB,90);

	return 0;
}
//함수 정의
void function_add_numbers(int a, int b, int c)
{
	int sum ;
	sum = a + b + c ;
	printf("Sum = %d\n",sum);
}
*/

#include <stdio.h>

int function_add_numbers(int , int , int );

int main()
{
	int retvalue;
	retvalue = function_add_numbers(12,13,0);

	printf("Sum = %d\n",retvalue);
	return 0;
}
int function_add_numbers(int a, int b, int c)
{
	int sum ;
	sum = a + b + c;
	return sum;//모든 데이터 유형을 반환 데이터 유형으로 사용 가능
}

