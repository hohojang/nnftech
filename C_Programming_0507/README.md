#  C Programming

이 저장소는 C 언어 학습을 위한 저장소입니다. 각 장에서는 핵심 개념을 학습하고, 실습 코드를 통해 직접 구현합니다.

---

 ##  CHAPTER 12: 파일 입출력

 - 파일 모드
   "r" 읽기 모드로 파일을 연다.
   "w" 쓰기 모드로 파일을 생성함. 만약 파일이 존재하지 않으면 파일이 생성됨. 파일이 이미 존재하면 기존의 내용이 지워짐.
   "a" 추가 모드로 파일을 연다. 만약 똑같은 이름의 기존의 파일이 있으면 데이터가 파일의 끝에 추가됨. 파일이 없으면 새로운 파일을 만든다.

 - 파일을 쓰기 모드로 열었다가 닫기
```c
#include<stdio.h>

int main(void)
{
  FILE *fp = NULL;

  fp = fopen("sample.txt", "w");

  if (fp == NULL)
     printf("파일 열기 실패\n");
  else
     printf("파일 열기 성공\n");

  fclose(fp);
  return 0;
}
```
