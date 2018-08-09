#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

#define _CLEAR printf("\033[2J")		//clear screen
#define _CURSORHOME printf("\033[H")	//cursor to upper left

void show(void *u, int w, int h)
{
	int (*univ)[w] = u;
	printf("\033[H");
	for_y {
		printf("\033[%d;0H", y);   // Home cursor
		for_x printf(univ[y][x] ? "\033[1;33;40m*\033[m" : "  ");
	}
	fflush(stdout);
}

void evolve(void *u, int w, int h)
{
	unsigned (*univ)[w] = u;
	unsigned new[h][w];

	for_y for_x {
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++)
			for (int x1 = x - 1; x1 <= x + 1; x1++)
				if (univ[(y1 + h) % h][(x1 + w) % w])
					n++;

		if (univ[y][x]) n--;
		new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
	}
	for_y for_x univ[y][x] = new[y][x];
}

void game(int w, int h)
{
	unsigned univ[h][w];

	struct timespec TS;
	TS.tv_nsec = 250000000;	TS.tv_sec = 0;  // 1/4 sec

	for_xy univ[y][x] = (rand() %2 == 1) ? 1 : 0;
	while (1) {
		show(univ, w, h);
		evolve(univ, w, h);
		nanosleep(&TS, NULL);	//brief pause
	}
}

int main(int c, char **v)
{
	int w = 0, h = 0;
	if (c > 1) w = atoi(v[1]);
	if (c > 2) h = atoi(v[2]);
	if (w <= 0) w = 60;
	if (h <= 0) h = 25;
	_CLEAR;
	game(w, h);
}