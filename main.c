#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

#define width 65
#define height 25
#define add_width 3
#define goal 5


char mas[height][width+1];
int counter = 0;
int lvl = 1;
BOOL start = FALSE;
BOOL END = FALSE;
typedef struct {int x, y, w;} TRacket;
typedef struct {int x, y, spdx, spdy;} TBall;
TBall ball;
TRacket racket;

void initBall()
{
    ball.x = 3;
    ball.y = 7;
    ball.spdx = 1;
    ball.spdy = -1;
}

void speedchange()
{
    if (mas[ball.y][ball.x+ball.spdx]=='#')
        ball.spdx = -ball.spdx;
    if (mas[ball.y+ball.spdy][ball.x]=='#')
        ball.spdy = -ball.spdy;

    if ((ball.y+ball.spdy==racket.y) &&
        (ball.x+ball.spdx>=racket.x-add_width) &&
        (ball.x+ball.spdx<=racket.x+racket.w+add_width-1))
        {
            ball.spdy = -ball.spdy;
            counter += 1;
        }
}

void putBall()
{
    if (!start)
    {
        counter = 0;
        ball.spdx = 1;
        ball.spdy = -1;
        ball.x = racket.x + racket.w / 2;
        ball.y = racket.y - 1;
    }
    else
    {
        ball.x += ball.spdx;
        ball.y += ball.spdy;
    }

    if (ball.y > height - 2)
    {
        start = FALSE;
    }

    mas[ball.y][ball.x] = '*';
}

void initRacket()
{
    racket.w = 7;
    racket.x = (width - racket.w) / 2;
    racket.y = height - 1;
}

void putRacket()
{
    for (int i=racket.x; i < racket.x + racket.w; i++)
        mas[racket.y][i] = '@';
}

void moveRacket()
{
    if (mas[racket.y][racket.x - 1]!='#')
        if (GetKeyState('A')<0)
            racket.x-=2;
    if (mas[racket.y][racket.x + racket.w]!='#')
        if (GetKeyState('D')<0)
            racket.x+=2;
}

void init(int lvl)
{
    for (int i=0; i<width; i++)
        mas[0][i] = '#';
    mas[0][width] = '\0';

    strncpy(mas[1], mas[0], width+1);
    for (int i=1; i<width-1; i++)
        mas[1][i] = ' ';

    for (int i=2; i<height; i++)
        strncpy(mas[i], mas[1], width+1);
    if (lvl==2)
        sprintf(mas[10], "#                   #########################                   #");
    if (lvl==3)
    {
        for (int i=1; i<8; i++)
            for (int j=4; j<width; j+=8)
                mas[i][j] = '#';
    }
}

void show()
{
    for (int i=0; i<height; i++)
    {
        printf("%s", mas[i]);
        if (i==3) printf("\tlevel: %d ", lvl);
        if (i==5) printf("\thits: %d ", counter);
        if (i==6) printf("\tgoal: %d", goal);
        if (i<height-1) printf("\n");
    }
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void checkgoal()
{
    if (counter>=goal)
        {
            system("cls");
            lvl++;
            if (lvl>3)
            {
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tEND");
                Sleep(2000);
                END=TRUE;
            }
            else
            {
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tLEVEL %d", lvl);
                Sleep(2000);
                system("cls");
                counter = 0;
                start = FALSE;
            }
        }
}

int main()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tLEVEL %d", lvl);
    Sleep(2000);
    system("cls");

    initRacket();
    initBall();
    do
    {
        setcur(0,0);
        init(lvl);
        moveRacket();
        putRacket();
        if (GetKeyState('W')<0)
            start = TRUE;
        speedchange();
        putBall();
        checkgoal();
        if (END==TRUE)
            return 0;
        show();
        Sleep(11);
    }
    while (GetKeyState(VK_ESCAPE)>=0);

    return 0;
}
