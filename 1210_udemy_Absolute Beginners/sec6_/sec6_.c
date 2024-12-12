/*
#include <stdio.h>

void file1_myFun1(void);
static int mainPrivateData; //이 파일 범위 내에서만 액세스 가능

int main()
{
	mainPrivateData = 100;
	printf("mainPrivateData = %d\n", mainPrivateData);

	file1_myFun1();
	printf("mainPrivateData = %d\n",mainPrivateData);

	return 0;
}

void change_system_clock(int system_clock)
{
	printf("system clock change to = %d\n",system_clock);
}
*/
#include <stdio.h>

int main()
{
	char a1 = 'A';
	char a2 = 'p';
	char a3 = 'p';
	char a4 = 'l';
	char a5 = 'e';
	char a6 = ':';
	char a7 = ')';

	printf("The string is : %c%c%c%c%c%c%c\n",a1,a2,a3,a4,a5,a6,a7);

	return 0;
}
