//CHAPTER 12: 파일 입출력
//텍스트 파일 읽고 쓰기 [한 글자씩 쓰기]
/*
#include <stdio.h>

int main(void)
{
    FILE* fp = NULL;

    // fopen_s()는 파일 포인터의 주소를 첫 번째 인수로 받는다
    errno_t err = fopen_s(&fp, "alphabet.txt", "w");
    if (err != 0 || fp == NULL) {
        fprintf(stderr, "파일 alphabet.txt를 열 수 없습니다.\n");
        return 0;
    }

    char c;
    for (c = 'a'; c <= 'z'; c++)
        fputc(c, fp);

    fclose(fp);
    return 0;
}
*/
//텍스트 파일 복사하기
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE* fp1 = NULL;
    FILE* fp2 = NULL;
    char file1[100] = { 0 }, file2[100] = { 0 };  // null으로 초기화

    printf("원본 파일 이름: ");
    scanf_s("%99s", file1, (unsigned)_countof(file1));

    printf("복사 파일 이름: ");
    scanf_s("%99s", file2, (unsigned)_countof(file2));

    if (fopen_s(&fp1, file1, "r") != 0 || fp1 == NULL) {
        fprintf(stderr, "원본 파일 %s를 열 수 없습니다.\n", file1);
        exit(1);
    }

    if (fopen_s(&fp2, file2, "w") != 0 || fp2 == NULL) {
        fprintf(stderr, "복사 파일 %s를 열 수 없습니다.\n", file2);
        fclose(fp1);
        exit(1);
    }

    int c;
    while ((c = fgetc(fp1)) != EOF)
        fputc(c, fp2);

    fclose(fp1);
    fclose(fp2);

    printf("복사가 완료되었습니다.\n");
    return 0;
}
*/
/*
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char* pc;

    pc = (char*)malloc(1 * sizeof(char));
    if (pc == NULL) {
        printf("메모리 할당 오류\n");
        exit(1);
    }
    *pc = 'a';
    printf("%c \n", *pc);
    free(pc);

    return 0;
}
*/
// 5개의 정수를 저장할 수 있는 동적 메모리 공간을 할당받고 정수 저장 
/*
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int* pi;

    pi = (int*)malloc(5 * sizeof(int));

    if (pi == NULL) {
        printf("메모리 할당 오류\n");
        exit(1);
    }
    *pi = 1;
    *(pi + 1) = 2;
    *(pi + 2) = 3;
    *(pi + 3) = 4;
    *(pi + 4) = 5;

    free(pi);
    return 0;
}
*/
//사용자가 입력하는 크기의 배열 만들기
/*
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int* p;
    int i, items;
    printf("항목의 개수는 몇개입니까? ");
    scanf_s("%d", &items);
    p = (int*)malloc(sizeof(int) * items);

    for (i = 0; i < items; i++) {
        printf("항목(정수)을 입력하시오: ");
        scanf_s("%d", &p[i]);
    }
    printf("\n입력된 값은 다음과 같습니다: \n");
    for (i = 0; i < items; i++)
        printf("%d", p[i]);
    printf("\n");
    free(p);

    return 0;
}
*/
//동적 구조체 배열 
/*
#include <stdio.h>
#include <stdlib.h>

struct movie {
    char title[100];
    double rating;
};

int main(void) {
    struct movie* ptr;
    int i, n;

    printf("영화의 개수: ");
    scanf_s("%d", &n);

    ptr = (struct movie*)malloc(n * sizeof(struct movie));
    if (ptr == NULL) {
        printf("메모리 할당 오류\n");
        exit(1);
    }

    for (i = 0; i < n; i++) {
        printf("영화 제목: ");
        scanf_s("%99s", ptr[i].title, (unsigned)_countof(ptr[i].title));  // 크기 명시!

        printf("영화 평점: ");
        scanf_s("%lf", &ptr[i].rating);
    }

    printf("\n===================================\n");
    for (i = 0; i < n; i++) {
        printf("영화 제목: %s \n", ptr[i].title);
        printf("영화 평점: %.1lf \n", ptr[i].rating);  // 소수점 1자리로 포맷팅
    }
    printf("===================================\n");

    free(ptr);
    return 0;
}
*/
//CHAPTER 14: 전처리기와 분할 컴파일 
//&&을 and로 바꾸기
/*
#include<stdio.h>

#define and &&
#define or || 

int main(void)
{
    int credits;
    double gpa;

    printf("지금까지 획득한 학점수를 입력하시오: ");
    scanf_s("%d", &credits);
    printf("지금까지 획득한 학점평균을 입력하시오: ");
    scanf_s("%lf", &gpa);

    if (credits >= 120 and gpa >= 2.0)
        printf("졸업 가능합니다. \n");
    else
        printf("좀 더 다녀야 합니다. \n");

    return 0;
}
*/
/*
#include<stdio.h>
#define SQUARE(x) ((x) * (x))

int main(void)
{
    int x = 2;

    printf("SQUARE(3) = %d\n", SQUARE(3));
    printf("SQUARE(1.2) = %f\n", SQUARE(1.2));
    printf("SQUARE(2+3) = %d\n", SQUARE(2 + 3));
    printf("x = %d\n", x);
    printf("SQUARE(x) = %d\n", SQUARE(x));
    printf("SQUARE(++x) = %d\n", SQUARE(++x));

    return 0;
}
*/
/*
#include <stdio.h>
#define LINUX

int main(void)
{
#ifdef LINUX
    printf("리눅스 버전입니다. \n");
#else
    printf("윈도우 버전입니다. \n");
#endif
    return 0;
}
*/
//함수 매크로를 사용하여 원의 면적 계산하는 프로그램

#include<stdio.h>

#define DEBUG
#define PI 3.141592
#define CIRCLE_AREA(r) (PI*(r)*(r))

int main(void)
{
    double radius, area;

#ifdef DEBUG
    printf("디버깅 모드입니다.\n");
#endif 

    printf("원의 반지름: ");
    scanf_s("%lf", &radius);
    area = CIRCLE_AREA(radius);
    printf("원의 면적=%2f", area);

    return 0;

}