//%f는 소수점 이하 6자리 숫자를 인쇄
/*
#include <stdio.h>

int main(void)
{
	double number = 45.78976834578;
	printf("Number = %0.2f\n",number);
	printf("Number = %0.2e\n",number);

	return 0;
}
*/

//double / 소수점 이하 15자리까지
//소수점 숫자의 출력을 하고싶으면 %0.28lf\n"[소수점 28자리] %0.8le\n"[소수점8자리]
#include <stdio.h>

int main(void)
{
	double chargeE = -1.60217662e-19;
	printf("chargeE = %0.28lf\n", chargeE);
	printf("chargeE = %0.8le\n", chargeE);

	return 0;
}

