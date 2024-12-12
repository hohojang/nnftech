/*
#include<stdio.h>

int main(void)
{
	float number1, number2, number3;
	float average;

	printf("Enter the first number: ");
	fflush(stdout);
	scanf("%f",&number1);
	printf("Enter the second number: ");
	fflush(stdout);
	scanf("%f",&number2);
	printf("Enter the third number: ");
	fflush(stdout);
	scanf("%f",&number3);

	average = (number1 + number2 + number3) / 3;

	printf("\nAverage is : %f\n", average);

	printf("Press enter key to exit the application \n");
	while(getchar()!= '\n')
	{

	}
	getchar();

}
*/
/*
//Exercise 6문자를 사용하는 ASCII CODE 예제
#include<stdio.h>

int main(void)
{
   char c1, c2, c3, c4, c5, c6;
   printf("Enter any 6 characters of your choice: ");
   scanf("%c %c %c %c %c %c", &c1, &c2, &c3, &c4, &c5, &c6);

   printf("\nASCII codes: %u, %u, %u, %u, %u, %u", c1, c2, c3, c4, c5, c6);

   printf("\nPress enter key to exit the application \n");
   while(getchar() != '\n')
   {
   }
   getchar();
}
*/
//Exercise 전하를 생성하는 전자의 수를 찾아라
#include<stdio.h>

int main(void)
{
	double charge , chargeE ;
	double electrons;
	printf("Enter the charge:");
	scanf("%lf",&charge);

	printf("Enter the charge of an electron:");
	scanf("%le",&chargeE);

	electrons = (charge / chargeE) * -1;
	printf("Total number of electrons = %le\n", electrons);
	printf("Total number of electrons = %lf\n", electrons);

	printf("press 'enter' key to exit the application\n");
	while(getchar() != '\n'){

	}
	getchar();
	return 0;
}

