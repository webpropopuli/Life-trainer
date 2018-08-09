/* Conway's GoL. Orig from https://rosettacode.org/wiki/Conway%27s_Game_of_Life#C
 * I've made changes to the timer as usleep is deprecated, as well ass
 * some other changes. You can always go look at the original.
 */
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
	_CURSORHOME;
	for_y {
		printf("\033[%d;0H", y);   // Home cursor
		for_x printf(univ[y][x] ? "\033[1;33;40m*\033[m" : " ");
	}
	fflush(stdout);
}

unsigned **univ;
//void evolve(void *u, int w, int h)
void evolve(int w, int h)
{
//	unsigned (*univ)[w] = u;
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

void game(unsigned *brd, int w, int h)
{
	//unsigned univ[h][w];

	struct timespec TS;
	TS.tv_nsec = 250000000;	TS.tv_sec = 0;  // 1/4 sec

	//for_xy univ[y][x] = (rand() % 2) ? 1 : 0;		// init board
	for_xy brd[y][x] = (rand() % 2) ? 1 : 0;		// init board

	while (1) {
		//show(univ, w, h);
		show(brd, w, h);
		nanosleep(&TS, NULL);	//brief pause
//		evolve(univ, w, h);
		evolve(w, h);

	}
}

int main(int argc, char **argv)
{
// Board defaults to 60x40 unless overridden by params
    _CLEAR;

	int w = 0, h = 0;
	if (argc > 1) w = atoi(argv[1]);
	if (argc > 2) h = atoi(argv[2]);
	if (w <= 0) w = 60;
	if (h <= 0) h = 40;
	unsigned *board = malloc( (h*w) * sizeof(unsigned));
	game(board, w, h);
	free(board);
}