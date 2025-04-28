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
