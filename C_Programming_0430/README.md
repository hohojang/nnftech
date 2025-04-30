#  C Programming

이 저장소는 C 언어 학습을 위한 저장소입니다. 각 장에서는 핵심 개념을 학습하고, 실습 코드를 통해 직접 구현합니다.

---

##  CHAPTER 10: 문자열
- 문자열 길이
  문자열의 길이를 계산하는 함수는 strlen()임. 문자열 안에 있는 문자들의 개수를 반환하지만 이때 NULL 문자는 포함되지 않음.
```C
#include<stdio.h>
#include<string.h>

int main(void)
{
  char s[] = "abcdefgh";
  int len = strlen(s);
  printf("문자열 %s의 길이=%d \n", s, len);
  return 0;
}
```
- 문자열 복사
  문자열을 복사하는 함수는 strcpy(dst, src)임. dst와 src는 모두 문자 배열로 src를 dst로 복사함.
```c
#include<stdio.h>
#include<string.h>

int main(void)
{
  char src[] = "Hello";
  char dst[6];

  strcpy(dst, src);
  printf("복사된 문자열 = %s \n", dst);
  return 0;
}
```
- 문자열 연결
- 문자열 뒤에 다른 문자열을 연결하려면 다음과 같이 strcat() 함수를 사용하면 됨.
```c
#include<stdio.h>
#include<string.h>

int main(void)
{
  char s[11] = "Hello";
  strcat(s, "World");
  printf("%s \n", s);

  return 0;
}
```
- 문자열 비교
  strcmp()는 문자열 s1과 s2를 비교하여 사전적인 순서에서 어떤 문자열이 앞에 있는지를 나타내는 숫자를 반환함.
  s1이 s2 앞에 있으면 음수가 반환되고 s1과 s2가 같으면 0이, s1이 s2의 뒤에 있으면 양수가 반환됨.
```c
int result = strcmp("dog","dog"); // 0이 반환됨.
```
##  CHAPTER 11: 구조체

- 구조체란 무엇인가?
  구조체는 프로그래머가 여러 개의 기초 자료형들을 묶어서 새로운 자료형을 정의할 수 있는 방법.
  예를 들어 학생들의 데이터를 표현해봄. 학생들의 데이터는 학번, 이름, 학점이라고함. 각 데이터마다 자료형이 다름.
  즉 학번은 정수이고 이름은 문자열, 학점은 실수임
```c
int number;    // 학번
char name[10]; // 이름
double grade;  // 학점
```
위의 데이터들은 서로 묶여 있지 않으므로 다루기가 불편함. 이럴때는 다음과 같이 구조체를 사용할 수 있음.
```c
struct student{
  int number;    //학번
  char name[10]; //이름
  double grade;  //학점
```
- 구조체의 정의, 초기화, 사용
- 구조체 정의
  서로 다른 자료형의 변수들을 묶어서 새로운 자료형을 만드는 것임. 구조체는 struct라는 키워드를 사용하여 정의함.
  struct 구조체 키워드에 이어서 나오는 student는 구조체 태그 라고 함. 구조체 태그는 구조체와 구조체를 구별하기 위해 붙이는 이름.
  어떠한 자료형의 변수도 구조체의 멤버가 될 수 있음. 그리고 구조체의 정의가 끝나면 반드시 세미콜론을 붙여야 함.
- 구조체 변수 생성
  구조체 변수는 struct student를 앞에 붙여서 변수들을 생성하면 됨
```c
struct student{
  int number;
  char name[10];
  double grade;
};
int main(void){
  struct student s1;
}
```
- 구조체의 초기화
  멤버들의 초깃값을 중괄호 안에서 나열하면 됨. 아래 코드에서 중괄호 안에 있는 값 24, "Kim", 4.3은 차례대로 구조체 멤버인 number, name,
  grade에 할당됨.
```c
struct student{
  int number;
  char name[10];
  double grade;
};
struct student s1 = { 24, "Kim", 4.3 };
```
- 구조체의 배열
  구조체의 배열이란 배열 원소가 구조체인 것을 말함. 즉 구조체가 여러 개 모인 것임.
- 구조체 배열의 선언
  구조체의 배열은 int나 char 배열을 선언하는 것과 비슷함.
```c
struct student{
  int number;
  char name[20];
  double grade;
};
struct student list[100];
```
위 코드는 student 구조체의 배열을 list[]란 이름으로 선엄함. 이 배열은 100명의 학생의 데이터를 저장할 수 있음.
배열의 첫 번째 요소에 값을 저장하기 
```c
list[0].number = 1;
strcpy(list[0].name, "Park");
list[0].grade = 3.42;
```
- 구조체와 함수
- 구조체를 함수의 인수로 넘기는 방법
  예) 두 명의 학생들의 데이터를 받아서 이름이 같은지 검사하는 함수 작성
```c
int equal(struct student s1, struct student s2)
{
  if( strcmp(s1.name, s2.name) == 0 )
    return 1;
  else
    return 0;
}
```
이 함수의 인수는 두 개이며 모두 구조체임. 만약 name이 같으면 1을 반환하고 다르면 0을 반환함. name은 문자형 배열이므로 strcmp()함수를 
사용하여 같은 지를 검사하여야 함.

- 공용체
  C언어에서는 같은 메모리 영역을 여러 개의 변수들이 공유할 수 있게 하는 기능이 있음. 이것을 공용체(union)라 불림.
  공용체는 멤버들이 같은 공간을 공유하기에 동시에 모든 멤버 변수들의 값을 저장할 수는 없음
```c
union example{
  char c;
  int i;
};
```
- 열거형
  열거형(enumeration)이란 변수가 가질 수 있는 값들을 미리 열거해놓은 자료형.
  열거형은 enum 이라는 키워드를 사용하여 만들어짐.
```c
enum 태크 {값1, 값2, . . .};
enum levels{low, medium, high};
//레벨을 나타내는 열겨형
enum levels{low, medium, high};
```
- typedef의 개념
  새로운 자료형을 정의 하는 것. 이 키워드는 C의 기본 자료형을 확장시키는 역할을 함. 즉 C가 기본적으로 제공하는 int형이나 float형 같은
  자료형에 사용자가 새로운 자료형을 추가할 수 있도록 함.

  ##  CHAPTER 12: 파일 입출력
-  표준 입출력 스트림
  프로그램의 동작에 필수적인 몇 개의 기본적인 스트림은 프로그램 실행 시에 자동으로 생성됨.
  이것은 표준 입출력 스트림이라고 함.
```c
stdin  / 표준 입력 스트림 / 키보드
stdout / 표준 출력 스트림 / 모니터의 화면
stderr / 표준 오류 스트림 / 모니터의 화면
```
- 파일의 개념
  파일도 스트림으로 취급되기 때문에 파일도 일련의 연속된 바이트라고 생각하면 됨.
  따라서 파일에 대한 입출력도 표준 입출력과 동일한 함수들로 이루어짐.
- 파일의 유형
  파일 유형에는 텍스트 파일과 이진 파일이 있음. 텍스트 파일(text file)은 사람이 읽을 수 있는 텍스트가 들어 있는 파일임.
  이진 파일(binary file)은 사람이 읽을 수는 없으나 컴퓨터는 읽을 수 있는 파일임. 즉 문자 데이터가 아니라 이진 데이터가 직접
  저장되어 있는 파일임. 이진 파일은 텍스트 파일과는 달리 라인들로 분리되지 않음.
```c
#include<stdio.h>

int main(void)
{
  printf("Hello World!");
  return 0;
}
```
 



