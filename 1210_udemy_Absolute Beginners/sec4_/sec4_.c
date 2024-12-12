/*
#include <stdio.h>

int main()
{
	unsigned char distanceA2B = 160;
	unsigned char distanceB2C = 40;

	unsigned char distanceA2C;

	distanceA2C = distanceA2B = distanceB2C;

	printf("%d is the Total distance from A2C \n",distanceA2C);

	return 0;
}
*/
/*
#include <stdio.h>

int main()
{
	long long myLongHistory = 900;

	char size = sizeof(myLongHistory);

	printf("Size of char data type  = %d\n",sizeof(char));
	printf("Size of short data type = %d\n",sizeof(short));
	printf("Size of int data type   = %d\n",sizeof(int));
	printf("Size of long data type  = %d\n",sizeof(long));
	printf("Size of myLongHistory variable = %d\n",size);


	return 0;
}
*/
/*
#include <stdio.h>

extern int myExamScore;

int main()
{
	myExamScore = 540;

	printf("Hello world");

	return 0;
}
*/

#include <stdio.h>

//함수 프로토타입
void myFun1(void);
//전역변수로 선언
int myscore;

int main()
{

	myscore = 900;
	printf("001Value of the variable myScore = %d\n",myscore);
	//함수 호출
	myFun1();
	return 0;
}
//함수 정의
void myFun1(void)
{
	myscore = 800;
	printf("002Value of the variable myScore = %d\n",myscore);
}
