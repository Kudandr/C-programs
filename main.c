#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define width 118
#define height 30
#define snowflakes_max 20

char mas[height][width+1];

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void init()
{
    for (int i=0; i<height; i++)
    {
        for (int j=0; j<width-1; j++)
            mas[i][j] = ' ';
        mas[i][width-1] = '\0';
    }
}

void first_line()
{
    int snowflakes_n = rand() % snowflakes_max + 1;
    for (int i=0; i<width; i++)
        mas[0][i] = ' ';
    for (int i=0; i<snowflakes_n; i++)
    {
        int x_coord = rand() % width;
        mas[0][x_coord] = '*';
    }
}

void move()
{
    for (int i=height-2; i>=0; i--)
    {
        strcpy(mas[i+1], mas[i]);
        for (int j=0; j<width; j++)
        {
            if (mas[i+1][j]=='*')
            {
             int prob = rand() % 20;
             if ((prob==1) && (j!=0))
             {
                 mas[i+1][j] = ' ';
                 mas[i+1][j-1] = '*';
             }
             if ((prob==2) && (j!=width-1))
             {
                 mas[i+1][j] = ' ';
                 mas[i+1][j+1] = '*';
             }
            }
        }
    }
}

void show()
{
    for (int i=0; i<height; i++)
    {
        printf("%s\n", mas[i]);
    }
}

int main()
{
    srand(time(NULL));
    init();
    do
    {
        setcur(0,0);
        first_line();
        move();
        show();
        Sleep(200);
    }
    while (GetKeyState(VK_ESCAPE)>=0);
    return 0;
}
