#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define MapWidth 300
#define MapHeight 25

#define xShow 80
#define yShow 25
#define jump_speed -1.2

#define g 0.15


char map[MapHeight][MapWidth + 1];
typedef struct Sobj {int x, y, h, w;
                    float speed_x, speed_y;
                    char type} TObj;

int corr = 0;
int brick_n;
TObj mario;
TObj *brick = NULL;

void ClearMap()
{
    for (int i = 0; i < MapWidth; i += 1)
        map[0][i] = ' ';
    map[0][MapWidth] = '\0';
    for (int j = 0; j < MapHeight; j += 1)
        sprintf(map[j], map[0]);
}

void InitObj(TObj *obj, int x, int y, int h, int w, float speed_x, float speed_y, char type)
{
    (*obj).x = x;
    (*obj).y = y;
    (*obj).h = h;
    (*obj).w = w;
    (*obj).speed_x = speed_x;
    (*obj).speed_y = speed_y;
    (*obj).type = type;
}

void PutObj(TObj *obj)
{
    for (int j = (*obj).y; j < (*obj).y + (*obj).h; j += 1)
        for (int i = (*obj).x; i < (*obj).x + (*obj).w; i += 1)
            map[j][i] = (*obj).type;
}

BOOL IsCollision(TObj obj1, TObj obj2)
{
    return ((obj1.x + obj1.w > obj2.x) && (obj1.x < obj2.x + obj2.w) && (obj1.y + obj1.h > obj2.y) && (obj1.y < obj2.y + obj2.h));
}

BOOL ScreenOut(TObj obj)
{
    return ((obj.x < 0) || (obj.x > MapWidth - 1) || (obj.y > MapHeight - 2) || (obj.y < 0));
}

void MoveObj(TObj *obj)
{
    if ((GetKeyState('W') < 0) && (map[(*obj).y + (*obj).h][(*obj).x] == '#'))
        (*obj).speed_y = jump_speed;
    (*obj).y += (int)round((*obj).speed_y);
    for (int i = 0; i < brick_n; i++)
        if (IsCollision(mario, *(brick + i)))
        {
            (*obj).y -= (*obj).speed_y;
            (*obj).speed_y = 0;
            if ((*(brick + i)).type == '+')
            {
                CreateLevel();
                corr = 0;
            }
            break;
        }
    (*obj).speed_y += g;

    if (GetKeyState('D') < 0)
        (*obj).x += round((*obj).speed_x);
    if (GetKeyState('A') < 0)
        (*obj).x -= round((*obj).speed_x);
    for (int i = 0; i < brick_n; i++)
        if (IsCollision(mario, *(brick+i)))
        {
            (*obj).x -= (*obj).speed_x;
            break;
        }

    if (ScreenOut(*obj))
    {
        CreateLevel();
        corr = 0;
    }
}

void ShowMap(TObj *player, int *corr)
{
    if ((*player).x >= 0.8 * xShow + *corr)
        *corr += 1;
    if ((*player).x <= 0.2 * xShow + *corr)
        *corr -= 1;

    char new_map[yShow][xShow + 1];
    for (int j = 0; j < yShow; j += 1)
    {
        for (int i = 0; i < xShow + 1; i++)
            {
                if (i != xShow)
                    new_map[j][i] = map[j][i + *corr];
                else
                    new_map[j][i] = '\0';
            }
        printf("%s\n", new_map[j]);
    }
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void CreateLevel()
{
    brick_n = 7;
    brick = malloc( sizeof(*brick) * brick_n );

    InitObj(&mario, 40, 15, 3, 2, 1, 0, '@');
    InitObj(brick, 40, 20, 5, 30, 0, 0, '#');
    InitObj(brick + 1, 70, 15, 10, 50, 0, 0, '#');
    InitObj(brick + 2, 120, 10, 15, 50, 0, 0, '#');
    InitObj(brick + 3, 170, 15, 10, 50, 0, 0, '#');
    InitObj(brick + 4, 220, 20, 5, 50, 0, 0, '#');
    InitObj(brick + 5, 280, 20, 5, 10, 0, 0, '+');
    InitObj(brick + 6, 0, 0, 25, 1, 0, 0, '|');
}

int main()
{
    CreateLevel();
    do
    {
        setcur(0, 0);
        ClearMap();
        PutObj(&mario);
        for (int i = 0; i < brick_n; i++)
            PutObj(brick + i);

        MoveObj(&mario);

        ShowMap(&mario, &corr);
        Sleep(20);
    }
    while (GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
