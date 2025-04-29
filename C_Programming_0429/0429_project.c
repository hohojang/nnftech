//CHAPTER 06
/*
//산수 문제 자동 출제 
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int i, ans;
	printf("산수 문제를 자동으로 출제합니다. \n");

	while(1) {
		int x = rand() % 100;
		int y = rand() % 100;

		printf("%d + %d = ", x, y);
		scanf_s("%d", &ans);

		if (x + y == ans) {
			printf("맞았습니다.\n");
			break;
		}
		else
			printf("틀렸습니다.\n");
	}
	return 0;
}
*/
//앵그리 공 게임 
/*
#include <windows.h>
#include <stdio.h>

int main(void) {
    HWND hwnd = GetDesktopWindow(); // 바탕화면 핸들 가져오기
    HDC hdc = GetWindowDC(hwnd);     // 바탕화면에 그림
    int x = 30, y = 200;
    int vx = 50, vy = -50;
    int i;

    MoveToEx(hdc, 30, 200, NULL);
    LineTo(hdc, 800, 200);

    for (i = 0; i < 20; i++) {
        vy = vy + 10;
        x = x + vx;
        y = y + vy;

        Ellipse(hdc, x, y, x + 10, y + 10);
        Sleep(100);
    }

    ReleaseDC(hwnd, hdc);  // 리소스 해제
    system("pause");
    return 0;
}
*/
//CHAPTER 07 배열
/*
#include<stdio.h>
#define SIZE 5
int main(void) {
    int score[SIZE] = { 31, 63, 62, 87, 14 };
    int i;

    for (i = 0; i < SIZE; i++) {
        printf("score[%d] = %d\n", i, score[i]);
    }
    return 0;
}
*/
//배열에 출석 기록하기
/*
#include<stdio.h>
#define SIZE 16

int main(void) {
    int att_book[SIZE] = { 0 };
    int i, count = 0;

    for (i = 0; i < SIZE; i++) {
        printf("%d번째 강의에 출석하셨나요(출석은 1, 결석은 0) : ", i + 1);
        scanf_s("%d", &att_book[i]);
    }

    for (i = 0; i < SIZE; i++) {
        if (att_book[i] == 0)
            count++;
    }
    double ratio = count / 16.0;
    if (ratio > 0.3)
        printf("결석 비율이 너무 높습니다. 강의를 듣는 것이 좋겠습니다.\n");
    return 0;
}
*/
//버블 정렬을 그림으로 표시하기  
/*
#include<stdio.h>
#include<windows.h>
#define SIZE 10

int main(void) {
    int i, k;
    int list[SIZE] = { 100, 30, 20, 78, 89, 12, 56, 38, 99, 66};

    HDC hdc = GetWindowDC(GetDesktopWindow());

    for (k = 0; k < SIZE; k++) {
        Rectangle(hdc, 0,0, 800,800);
        for(i = 0; i < SIZE-1; i++){
            if(list[i]>list[i + 1]){
                int tmp = list[i];
                list[i] = list[i+1];
                list[i+1] = tmp;
            }
        }
        for (i = 0; i < SIZE; i++){
            Rectangle(hdc, 200 + i * 30, 200, 230 + i * 30, 200 - list[i]);
        }
        Sleep(1000);
    }
    return 0;
}
*/
//행렬의 덧셈
/*
#include<stdio.h>
#define ROWS 3
#define COLS 3

int main(void)
{
    int r, c;

    int A[ROWS][COLS] = { {1,0,0}, {0,1,0}, {0,0,1} };
    int B[ROWS][COLS] = { {1,0,0}, {0,1,0}, {0,0,1} };
    int C[ROWS][COLS];

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            C[r][c] = A[r][c] + B[r][c];
            printf("%d", C[r][c]);
        }
        printf("\n");
    }
    return 0;
}
*/

//Tic-Tac-Toe 게임 

/*
#include<stdio.h>

int main(void)
{
    char board[3][3];
    int x, y, k, i;

    for (x = 0; x < 3; x++)
        for (y = 0; y < 3; y++) board[x][y] = ' ';

    for (k = 0; k < 9; k++) {
        printf("(x,y) 좌표: ");
        scanf_s("%d %d", &x, &y);
        board[x][y] = (k % 2 == 0) ? 'X' : 'O';

        for (i = 0; i < 3; i++) {
            printf("---|---|---\n");
            printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        }
        printf("---|---|---\n");
    }
    return 0;
}
*/
//CHAPTER 08 함수
//사용자가 입력하는 두 수의 합을 계산
/*
#include <stdio.h>

int get_integer()
{
    int value;
    printf("정수를 입력하시오 : ");
    scanf_s("%d", &value);  
    return value;           
}

int main(void)
{
    int x = get_integer();
    int y = get_integer();

    int result = x + y;
    printf("두수의 합 + %d \n", result);

    return 0;
}
*/
//함수를 호출하여 사용자가 입력한 정수 중에서 큰 수 찾기 
/*
#include<stdio.h>

int get_integer()
{
    int value;
    printf("정수를 입력하시오 :");
    scanf_s("%d", &value);
    return value;
}

int get_max(int x, int y)
{
    if (x > y) return(x);
    else return(y);
}
int main(void)
{
    int a = get_integer();
    int b = get_integer();

    printf("두 숫자 중에서 큰 수는 %d입니다.\n", get_max(a, b));
    return 0;
}
*/
//온도 변환 함수
/*
#include<stdio.h>
double FtoC(double temp_f); // 함수 원형 정의

int main(void)
{
    double c, f;
    f = 32.0;
    c = FtoC(f);
    printf("화씨온도 %f는 섭씨온도 %f에 해당한다. \n", f, c);
    return 0;
}

double FtoC(double temp_f) // 함수 정의
{
    double temp_c;
    temp_c = (5.0 * (temp_f - 32.0)) / 9.0;
    return temp_c;
}
*/
//그래픽 함수 정의
/*
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>

// x1, y1에서 x2, y2까지 직선을 그리는 함수

void line(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

int randint(int x1, int x2)
{
    return x1 + rand() % (x2 - x1);
}

int main(void)
{
    int i;
    HWND hwnd = GetDesktopWindow(); // 바탕화면 핸들 가져오기
    HDC hdc = GetWindowDC(hwnd);     // 바탕화면에 그림

    for (i = 0; i < 30; i++) {
        int x1 = randint(0, 300);
        int y1 = randint(0, 300);
        int x2 = randint(0, 300);
        int y2 = randint(0, 300);
        line(hdc, x1, y1, x2, y2);

        Sleep(100);
    }
}
*/

//정적 변수
/*
#include<stdio.h>

void sub(void)
{
    int auto_count = 0;
    static int static_count = 0;

    auto_count++;
    static_count++;
    printf("auto_count=%d\n", auto_count);
    printf("static_count=%d\n", static_count);
}
int main(void)
{
    sub();
    sub();
    sub();
    return 0;
}
*/
// 동심원 그리기
/*
#include<Windows.h>
#include<stdio.h>

HDC hdc;

void draw_circle(int x, int y, double radius) {
    //화면에 원을 그림
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    if (radius > 2.0) {
        radius *= 0.75;
        draw_circle(x, y, radius);
    }
}

int main(void)
{
    HWND hwnd = GetDesktopWindow(); // 바탕화면 핸들 가져오기
    HDC hdc = GetWindowDC(hwnd);     // 바탕화면에 그림
    draw_circle(300, 200, 200.0);
    return 0;
}
*/
//CHAPTER 09 포인터 
/*
#include<stdio.h>

int main(void)
{
    char *pc;
    int  *pi;
    double  *pd;

    pc = (char*)10000;
    pi = (int*)10000;
    pd = (double*)10000;
    printf("증가 전 pc = %d, pi = %d, pd = %d\n", pc, pi, pd);

    pc++;
    pi++;
    pd++;
    printf("증가 후 pc = %d, pi = %d, pd = %d\n", pc, pi, pd);

    return 0;
}
*/
//값에 의한 호출 
/*
#include <stdio.h>

void modify(int value)
{
    value = 99; //매개 변수를 변경
}
int main(void)
{
    int number 1;

    modify(number);
    printf("number = %d\n", number);

    return 0;
}
*/
// swap()함수 작성하기
/*
#include<stdio.h>

void swap(int* px, int* py);

int main(void)
{
    int a = 100, b = 200;

    printf("swap() 호출 전 a=%d b=%d\n", a, b);
    swap(&a, &b);
    printf("swap() 호출 후 a=%d b=%d\n", a, b);

    return 0;
}
void swap(int* px, int* py)
{
    int tmp;

    tmp = *px;
    *px = *py;
    *py = tmp;
}
//CHAPTER 10 문자열 
*/
// 배열을 선언하고 배열의 이름과 첫 번째 원소의 주소를 나란히 출력
/*
#include<stdio.h>

int main(void)
{
    int a[] = { 10, 20, 30, 40, 50 };
    printf("배열의 이름 = %u\n", a);
    printf("첫 번째 원소의 주소 = %u\n", &a[0]);

    return 0;
}
*/
/*
#include <stdio.h>
int main(void)
{
    int a[10] = { 1, 2, 3, 4, 5, 6 };
    int* p = a;
    int* q = a + 3;

    printf("%d\n", *(a + 1));
    printf("%d\n", *(p + 2));
    printf("%d\n", *(q + 3));
    return 0;
}
*/
// 도형 크기 변경
/*
#include<Windows.h>
#include<stdio.h>

int main(void)
{
    int ch;
    int width = 200, height = 200;
    HDC hdc = GetWindowDC(GetForegroundWindow());

    while (1) {
        Rectangle(hdc, 100, 100, width, height);
        ch = getch();
        Rectangle(hdc, 0, 0, 800, 600);
        if (ch == 'b') {
            width += 10;
            height += 10;
        }
        else if (ch == 's') {
            width -= 10;
            height -= 10;
        }
        else if (ch == 'q') {
            return 0;
        }
        
    }
    return 0;
}
*/
// 사용자로부터 문자열 2개를 받아서 비교하는 프로그램 

#include<stdio.h>
#include<string.h>

int main(void)
{
    char s1[80];
    char s2[80];
    int result;

    printf("첫 번째 단어를 입력하시오:");
    scanf_s("%d", &s1);
    printf("두 번째 단어를 입력하시오:");
    scanf_s("%d", &s2);

    result = strcmp(s1, s2);
    if (result < 0)
        printf("%s가 %s보다 앞에 있습니다.\n", s1, s2);
    else if (result == 0)
        printf("%s가 %s와 같습니다.\n", s1, s2);
    else
        printf("%s가 %s보다 뒤에 있습니다.\n", s1, s2);
    return 0;
}