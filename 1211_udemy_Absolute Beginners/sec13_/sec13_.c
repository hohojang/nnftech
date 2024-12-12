/*
   1. Create a char type variable and initialize it to value 100
   2. Print the address of the above variable
   3. Create a pointer variable and store the address of the above variable
   4. Perform read operation on the pointer variable to fetch 1 byte of data form the pointer
   5. Print the data obtained from the read operation on the pointer
   6. Perform write operation on the pointer to store the value 65
   7. Print the value of the variable defined in step 1

   1. char 타입 변수를 생성하고 값 100으로 초기화
      char 자료형 변수 하나를 선언하고, 초기값으로 100을 설정합니다.

   2. 위에서 생성한 변수의 주소를 출력
      변수의 메모리 주소를 출력합니다.

   3. 포인터 변수를 생성하고 위 변수의 주소를 저장
      포인터 변수(주소를 저장하는 변수)를 선언한 후, 첫 번째 단계에서 생성한 변수의 주소를 이 포인터에 저장합니다.


   4. 포인터 변수를 통해 1바이트 데이터를 읽는 작업 수행
      포인터를 사용하여, 해당 주소에서 1바이트 데이터를 읽어옵니다.

   5. 포인터로 읽은 데이터를 출력
      읽어온 값을 출력합니다.

   6. 포인터를 통해 쓰기 작업 수행: 값 65 저장
      포인터를 사용하여, 해당 주소에 값 65를 씁니다.

   7. 1단계에서 정의한 변수의 값을 출력
      첫 번째 단계에서 생성한 변수의 값을 출력하여, 포인터를 통해 쓰기 작업 후 값이 변경되었는지 확인합니다.
 */
/*
int main(void)
{

	char data = 100; //변수를 정의/변수 값 100 초기화
	printf("Value of data is : %d\n", data);
	printf("Address of the variavle data is : %p\n", &data);
	char*pAddress = &data; //변수의 주소를 포인터에 저장
	char value = *pAddress; // 주소 역참조
	printf("read value is : %d\n",value); //아직 데이터 값 100

	*pAddress = 65; //포인터변수로 해당 주소에 새로운 값 저장
	printf("Value of data is : %d\n", data);


	return 0;
}
*/
/*
//포인터를 사용하여 동일한 변수의 메모리 내용을 다른 크기 및 방법으로 읽어들임
#include<stdio.h>

long long int g_data = 0xFFFEABCD11112345;
int main(void)
{
	char* pAddress1;
	pAddress1 = (char*)&g_data;
	printf("Value at address %p is : %x\n", pAddress1,*pAddress1);

	int*pAddress2;
	pAddress2 = (int*)&g_data;
	printf("Value at address %p is : %x\n", pAddress2,*pAddress2);

	short*pAddress3;
	pAddress3 = (short*)&g_data;
	printf("Value at address %p is : %x\n", pAddress3,*pAddress3);

	long long*pAddress4;
	pAddress4 = (long long*)&g_data;
	printf("Value at address %p is : %I64x\n", pAddress4,*pAddress4);


	return 0;
}
*/
//포인터 연산을 사용해 변수의 메모리 주소를 다루고 , 포인터를 증가시키는 것에 대한 메모리의 영향
#include<stdio.h>

long long int g_data = 0xFFFEABCD11112345;
int main(void)
{
	int *pAddress = (int*)&g_data;
	printf("Value of pAddress %p\n",pAddress);
	printf("Value of address %p is %d\n",pAddress, *pAddress);

	pAddress = pAddress + 1;

	printf("Value of pAddress %p\n",pAddress);
	printf("Value of address %p is %d\n",pAddress, *pAddress);
}
