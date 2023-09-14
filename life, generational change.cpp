#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
#include <windows.h>

#define msleep(x) Sleep(x)

using namespace std;


//Ширина игрового поля 
const int PLAYGROUND_HEIGTH = 32;

//Длина игрового поля 
const int PLAYGROUND_WIDTH = 32;

const int random = 30;

struct cell {
    unsigned is_live : 1;
};








//Инициализация первого поколения игры случайными значениями
void create_world(cell world[][PLAYGROUND_HEIGTH])
{
    unsigned rand_value = 11;
    srand(rand_value);
    unsigned int i, j;

    for (i = 0; i < PLAYGROUND_HEIGTH; i++) {
        for (j = 0; j < PLAYGROUND_WIDTH; j++) {
            unsigned int num = 1 + rand() % random;
            if (num % 2 == 0) {
                world[i][j].is_live = 1;
            }
            else {
                world[i][j].is_live = 0;
            }
        }
    }
}



//Вывести на экран игровое поле
void print_world(cell world[][PLAYGROUND_HEIGTH])
{
    unsigned int i, j;
    for (i = 0; i < PLAYGROUND_HEIGTH; i++) {
        for (j = 0; j < PLAYGROUND_WIDTH; j++) {
            if (world[i][j].is_live == 1) {
                cout << '+';
            }
            else {
                cout << ' ';
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << "_____________________________________________________________________" << endl;
}



//Счетчик жизни
unsigned int life_counter(cell world[][PLAYGROUND_HEIGTH])
{
    unsigned int count = 0;
    unsigned i, j;
    for (i = 0; i < PLAYGROUND_WIDTH; i++) {
        for (j = 0; j < PLAYGROUND_HEIGTH; j++) {
            if (world[i][j].is_live == 1) {
                count++;
            }
        }
    }
    return count;
}



//Получение координат соседей точки
void scan_neighbors(signed int nb[][2], unsigned int x, unsigned int y)
{
    unsigned int i, j;
    unsigned int k = 0;

    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) {
                continue;
            }
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
    }
}



//Количество живых соседей у клетки с координатами x, y
unsigned int life_neighbors_counter(cell world[][PLAYGROUND_HEIGTH], unsigned int x, unsigned int y)
{
    unsigned int count = 0;
    unsigned int i;
    int nb[8][2];
    int _x, _y;

    scan_neighbors(nb, x, y);

    for (i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (x > 0 && y > 0 && x < PLAYGROUND_WIDTH && y < PLAYGROUND_HEIGTH && world[_x][_y].is_live == 1) {
            count++;
        }
    }

    return count;
}



//Сгенерировать следующее поколение игрового мира
void next_gen(cell world[][PLAYGROUND_HEIGTH], cell prev_world[][PLAYGROUND_HEIGTH])
{
    unsigned int i, j;
    unsigned int live_nb;
    cell p;

    for (i = 0; i < PLAYGROUND_WIDTH; i++) {
        for (j = 0; j < PLAYGROUND_HEIGTH; j++) {
            p = prev_world[i][j];
            live_nb = life_neighbors_counter(prev_world, i, j);

            if (p.is_live == 0) {
                if (live_nb == 3) {
                    world[i][j].is_live = 1;
                }
            }
            else {
                if (live_nb < 2 || live_nb > 3) {
                    world[i][j].is_live = 0;
                }
            }
        }
    }
}



//Копирование игрового мира. Используется для сохранения предыдущего поколения
void copy_world(cell src[][PLAYGROUND_HEIGTH], cell dest[][PLAYGROUND_HEIGTH])
{
    unsigned int i, j;
    for (i = 0; i < PLAYGROUND_WIDTH; i++) {
        for (j = 0; j < PLAYGROUND_HEIGTH; j++) {
            dest[i][j] = src[i][j];
        }
    }
}



//Сравнение игровых миров текущего и предыдущего поколения
int cmp_world(cell w1[][PLAYGROUND_HEIGTH], cell w2[][PLAYGROUND_HEIGTH])
{
    unsigned int i, j;
    for (i = 0; i < PLAYGROUND_WIDTH; i++) {
        for (j = 0; j < PLAYGROUND_WIDTH; j++) {
            if (w1[i][j].is_live != w2[i][j].is_live) {
                return -1;
            }
        }
    }
    return 0;
}

int main()
{
    cell world[PLAYGROUND_WIDTH][PLAYGROUND_HEIGTH];
    cell prev_world[PLAYGROUND_WIDTH][PLAYGROUND_HEIGTH];
    cell prev_prev_world[PLAYGROUND_WIDTH][PLAYGROUND_HEIGTH];
    

    create_world(world);
    unsigned int alive_cells = -1;
    bool is_static = false;
    bool is_cycle = false;

    ofstream out_stream;
    out_stream.open("C:\\Users\\asus\\Desktop\\Число клеток.xls");
    int counter = 1;
    do {
        print_world(world);
        copy_world(prev_world, prev_prev_world);
        copy_world(world, prev_world);
        next_gen(world, prev_world);

        is_static = cmp_world(world, prev_world) == 0;
        if (cmp_world(world,prev_prev_world) == 0 ) {
            is_cycle = true; 
        }
        alive_cells = life_counter(world);

        out_stream << counter << '\t' << alive_cells << endl;

        if (is_static) {
            cout << "Static configuration" << endl;
        }
        if (is_cycle) {
            cout << "Cycle configuration" << endl;
        }
        if (alive_cells == 0) {
            cout << "All cells died" << endl;
        }
        counter++;
        msleep(1000);
    } while (alive_cells != 0 && !is_static && !is_cycle);
    out_stream.close();

    return 0;
}
