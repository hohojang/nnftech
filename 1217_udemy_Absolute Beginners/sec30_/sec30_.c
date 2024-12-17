/*
#include<stdio.h>
#include<stdint.h>

int main(void)
{
    char msg1[] = "Hello how are you?";  // 문자열 msg1 초기화
    char const *pmsg2 = "fastbitlab.com";  // 문자열 포인터 pmsg2 초기화 (읽기 전용)

    msg1[0] = 'b';  // msg1의 첫 번째 문자 'H'를 'b'로 변경
    //pmsg2[0] = 'b';  // 오류: pmsg2는 읽기 전용이므로 값을 변경할 수 없음

    // 문자열 출력
    printf("Message is : %s\n", msg1);  // msg1 출력
    printf("Message is : %s\n", pmsg2);  // pmsg2 출력
    printf("Address of 'pmsg2' variable = %p\n", &pmsg2);  // pmsg2의 주소 출력
    printf("Value of 'pmsg2' variable = %p\n", pmsg2);  // pmsg2가 가리키는 주소 출력

    return 0;
}
*/

/*
#include<stdio.h>
#include <stdint.h>

int main(void)
{
    char name[30];  // 이름을 저장할 배열 선언
    printf("Enter your name :");  // 사용자에게 이름 입력을 요청
    fflush(stdout);  // 출력 버퍼 비우기
    scanf("%s", name);  // 사용자로부터 이름 입력 받기
    printf("Hi,%s\n",name);  // 입력 받은 이름을 출력
    fflush(stdout);  // 출력 버퍼 비우기

    // 이름 배열의 각 문자의 ASCII 값 출력
    for(uint32_t i = 0; i < 30; i ++)
    {
        printf("%x\n",name[i]);  // 각 문자의 16진수 값 출력
    }

    return 0;
}

void wait_for_user_input(void) {
    // 입력 버퍼를 비우고 사용자로부터 Enter 키 대기
    while (getchar() != '\n') {
        // 입력 버퍼 비우기
    }
    getchar();  // 마지막 Enter 키 입력 대기
}
*/

/*
#include<stdio.h>

int main()
{
    int a , b , c;  // 세 개의 정수 변수 선언
    printf("Enter 3 number");  // 세 개의 숫자 입력을 요청
    fflush(stdout);  // 출력 버퍼 비우기
    scanf("%d%d%d",&a,&b,&c);  // 사용자로부터 세 개의 숫자 입력 받기
    printf("Numbers are : %d %d %d",a,b,c);  // 입력 받은 숫자 출력
    fflush(stdout);  // 출력 버퍼 비우기

    return 0;
}
*/

/*
#include<stdio.h>

int main()
{
    char name[30];  // 이름을 저장할 배열 선언
    printf("Enter your full name:");  // 사용자에게 전체 이름 입력을 요청
    fflush(stdout);  // 출력 버퍼 비우기
    scanf("%[^\n]s",name);  // 사용자로부터 전체 이름 입력 받기
    printf("Your name is : %s \n",name);  // 입력 받은 이름 출력
    fflush(stdout);  // 출력 버퍼 비우기

    return 0;
}
*/

#include<stdio.h>
#include<stdint.h>
#include <string.h>

// 학생 정보를 담을 구조체 정의
typedef struct
{
    int    rollNumber;   // 학번
    char   name[100];    // 이름
    char   branch[50];   // 전공
    char   dob[15];      // 생일
    int    semister;     // 학기
} STUDENT_INFO_t;

// 최대 학생 수 (레코드 개수)
int const max_record = 2;

// 학생 데이터 초기화
STUDENT_INFO_t students[2] =
{
    {9876, "ashok kumar","mechanical","12/11/1999",7},
    {9876, "ram kumar ","computer","12/11/1999",7}
};

// 학생 레코드를 출력하는 함수
void display_all_records(STUDENT_INFO_t *pBaseAddrofRecords, int max_record)
{
    // 모든 학생 레코드를 출력
    for(uint32_t i = 0; i < max_record ; i++)
    {
        printf("Roll number of %dth element of the array = %d\n", i + 1, pBaseAddrofRecords[i].rollNumber);
    }
}

int main()
{
    // 학생 레코드 출력 함수 호출
    display_all_records(students, max_record);

    return 0;
}
