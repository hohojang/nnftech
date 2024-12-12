/*
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

	printf("Address of variable a1 = %p\n",&a1);
	printf("Address of variable a2 = %p\n",&a2);
	printf("Address of variable a3 = %p\n",&a3);
	printf("Address of variable a4 = %p\n",&a4);
	printf("Address of variable a5 = %p\n",&a5);
	printf("Address of variable a6 = %p\n",&a6);
	printf("Address of variable a7 = %p\n",&a7);


	return 0;
}
*/
#include <stdio.h>

int main()
{
	char a1 = 'A';
	/*
	char a2 = 'p';
	char a3 = 'p';
	char a4 = 'l';
	char a5 = 'e';
	char a6 = ':';
	char a7 = ')';
	*/
	unsigned long int addressOfa1 = (unsigned long int)&a1;
	printf("Address of variable a1 = %ld\n",addressOfa1);

	return 0;
}
