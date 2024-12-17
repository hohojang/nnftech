/*
#include<stdio.h>
#include<stdint.h>

int main(void)
{
    uint32_t studentsAge[100];  // 학생들의 나이를 저장할 배열 선언
    printf("Size of an array = %u\n", sizeof(studentsAge));  // 배열의 크기 출력
    printf("Base address of the array = %p\n", studentsAge);  // 배열의 시작 주소 출력
    return 0;
}
*/

// 10개 항목의 배열을 0xff로 초기화 후 출력하는 프로그램
/*#include<stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t someData[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};  // 배열 초기화

    printf("Before : 2nd data item = %x\n",*(someData+1));  // 2번째 데이터 항목 출력
    *(someData+1)= 0x9;  // 2번째 항목 값을 0x9로 수정
    printf("After : 2nd data item = %x\n",*(someData+1));  // 수정된 2번째 항목 출력
    return 0;
}
*/

/*
#include<stdio.h>
#include <stdint.h>

void array_display(uint8_t *pArray, uint32_t nItems);

int main(void)
{
    uint8_t someData[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};  // 배열 초기화

    // 배열의 모든 항목을 0x33으로 변경
    for(uint32_t i = 0; i < 10; i++)
    {
        someData[i] = 0x33;
    }

    uint32_t nItems = sizeof(someData) / sizeof(uint8_t);  // 배열의 항목 수 계산
    array_display(someData, nItems);  // 배열 출력 함수 호출

    return 0;
}

// 배열 항목을 출력하는 함수
void array_display(uint8_t *pArray, uint32_t nItems)
{
    for(uint32_t i = 0; i < nItems ; i++)
    {
        printf("%x\t",*(pArray + i));  // 각 항목을 16진수로 출력
    }
}
*/

// 두 배열을 교환하는 프로그램 작성
#include<stdio.h>
#include <stdint.h>

// 함수 선언
void wait_for_user_input(void);  // 사용자 입력 대기 함수
void display_array(int32_t *pArray1, uint32_t nitem);  // 배열 출력 함수
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t nitem1, uint32_t nitem2);  // 배열 교환 함수

int main(void)
{
    int32_t nItem1, nItem2;
    printf("Array swapping program\n");
    printf("Enter number of elements of Array-1 and Array-2:");  // 두 배열의 크기 입력 요청
    scanf("%d %d", &nItem1, &nItem2);  // 배열 크기 입력 받기

    // 배열 크기가 음수일 경우 오류 처리
    if((nItem1 < 0) || (nItem2 < 0))
    {
        printf("Number of elements cannot be negative\n");
        wait_for_user_input();  // 사용자 입력 대기
        return 0;
    }

    int32_t array1[nItem1];  // 첫 번째 배열
    int32_t array2[nItem2];  // 두 번째 배열

    // 첫 번째 배열의 항목 입력 받기
    for(uint32_t i = 0; i < nItem1; i++)
    {
        printf("Enter %d element of array1:", i);
        scanf("%d", &array1[i]);
    }

    // 두 번째 배열의 항목 입력 받기
    for(uint32_t i = 0; i < nItem2; i++)
    {
        printf("Enter %d element of array2:", i);
        scanf("%d", &array2[i]);
    }

    // 배열 출력 (교환 전)
    printf("Contents of array before swap\n");
    display_array(array1, nItem1);  // 첫 번째 배열 출력
    printf("\n");
    display_array(array2, nItem2);  // 두 번째 배열 출력
    printf("\n");

    // 배열 교환 후 출력
    printf("Contents of arrays after swap\n");
    swap_arrays(array1, array2, nItem1, nItem2);  // 배열 교환 함수 호출

    display_array(array1, nItem1);  // 첫 번째 배열 출력
    printf("\n");
    display_array(array2, nItem2);  // 두 번째 배열 출력
    printf("\n");

    wait_for_user_input();  // 사용자 입력 대기

    return 0;
}

// 배열 출력 함수
void display_array(int32_t *pArray1, uint32_t nitem)
{
    for(uint32_t i = 0; i < nitem ; i ++)
    {
        printf("%4d ", pArray1[i]);  // 각 항목을 4자리씩 출력
    }
}

// 두 배열을 교환하는 함수
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t nitem1, uint32_t nitem2)
{
    uint32_t len = nitem1 < nitem2 ? nitem1 : nitem2;  // 더 작은 배열 크기 선택

    for(uint32_t i = 0; i < len; i++)
    {
        int32_t temp = array1[i];  // 임시 변수에 값 저장
        array1[i] = array2[i];  // 배열1의 항목을 배열2의 항목으로 교환
        array2[i] = temp;  // 배열2의 항목을 임시 변수에 저장된 값으로 교환
    }
}

// 사용자 입력 대기 함수
void wait_for_user_input(void)
{
    printf("Press enter key to exit this application");  // 종료를 위한 안내 메시지

    // Enter 키가 입력될 때까지 대기
    while(getchar() != '\n')
    {
    }
    getchar();  // 마지막 Enter 키 입력 대기
}
