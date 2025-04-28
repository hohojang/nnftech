#  C Programming

이 저장소는 C 언어 학습을 위한 저장소입니다. 각 장에서는 핵심 개념을 학습하고, 실습 코드를 통해 직접 구현합니다.

---

##  CHAPTER 03: 변수와 자료형 

### 문자형
- 문자를 입출력 하는 프로그램
```C
#include<stdio.h>

int main(void)
{
  char c; //변수 선언

  printf("문자를 입력하시오: ");
  scanf("%c", &c);

  printf("입력된 문자는 %c입니다\n", c);
  return 0;
}
```
##  CHAPTER 04: 수식과 연산자 

- 수식(expression)이란 피연산자들과 연산자의 조합
- 산술연산자
  산술 연산자는 기본적인 산술 연산인 덧셈, 뺄셈, 곱셈, 나눗셈, 나머지 연산을 실행하는 연산자임.
- 사용자에게서 2개의 정수를 받아 여러 가지 연산을 한 후에 결과를 출력하는 프로그램
```c
#include<stdio.h>

int main(void)
{
  int x, y, result;

  printf("두 개의 정수를 입력하시오: ");
  scanf("%d %d", &x, &y); // 2개의 정수를 동시에 입력받는다.

  result = x + y; // 덧셈 연산을 하여서 결과를 result에 대입
  printf("%d + %d = %d\n", x, y, result);

  result = x - y; // 뺄셈 연산을 하여서 결과를 result에 대입
  printf("%d - %d = %d\n", x, y, result);

  result = x * y; // 곱셈 연산을 하여서 결과를 result에 대입
  printf("%d * %d = %d\n", x, y, result);

  result = x / y; // 나눗셈 연산을 하여서 결과를 result에 대입
  printf("%d / %d = %d\n", x, y, result);
  result = x % y; // 나머지 연산
  printf("%d %% %d = %d\n", x, y, result); // %을 출력하려면 %%하여야 함

  return 0;
}
```
