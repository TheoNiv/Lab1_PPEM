#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "yuvDisplay.h"
#include "yuvRead.h"
#include "sobel.h"
#include <pthread.h>
#define NB_THREADS 1

int stopThreads = 0;
// 

int main(int argc, char** argv)
{
	// Declarations
	unsigned char y[HEIGHT*WIDTH], ySobel[HEIGHT*WIDTH], u[HEIGHT*WIDTH / 4], v[HEIGHT*WIDTH / 4];
	pthread_t Sobel_thread[NB_THREADS];
	struct args_thread_sobel args_sobel[NB_THREADS];

	// Init display
	yuvDisplayInit(0, WIDTH, HEIGHT);

	// Open file
	initReadYUV(WIDTH, HEIGHT);

	while (!stopThreads)
	{
		// Read a frame
		readYUV(WIDTH, HEIGHT, y, u, v);

		args_sobel[0].width = WIDTH;
		args_sobel[0].height = HEIGHT / NB_THREADS;
		args_sobel[0].input = y;
		args_sobel[0].output = ySobel;
		pthread_create(&(Sobel_thread[0]), NULL, sobel, (void *)&(args_sobel[0]));

		for (int i = 1; i < NB_THREADS; ++i)
		{
			args_sobel[i].width = WIDTH;
			args_sobel[i].height = 2 + (HEIGHT / NB_THREADS);
			args_sobel[i].input = y + WIDTH * (i*HEIGHT / NB_THREADS - 2);
			args_sobel[i].output = ySobel + WIDTH* (i*HEIGHT / NB_THREADS - 2);
			pthread_create(&(Sobel_thread[i]), NULL, sobel, (void *)&(args_sobel[i]));
		}
		for (int i = 0; i < NB_THREADS; ++i)
		{
			pthread_join(Sobel_thread[i], NULL);
		}
		//sobel(WIDTH, HEIGHT / 3, y, ySobel);
		//sobel(WIDTH, 2 + (HEIGHT / 3), y + WIDTH * (HEIGHT / 3 - 2), ySobel + WIDTH* (HEIGHT / 3 - 2));
		//sobel(WIDTH, 2 + (HEIGHT / 3), y + WIDTH * (2*HEIGHT / 3 - 2), ySobel + WIDTH* (2*HEIGHT / 3 - 2));

		//Border
		memset(ySobel, 0, WIDTH);
		memset(ySobel + (HEIGHT - 1)*WIDTH, 0, WIDTH);
		// Display it
		yuvDisplay(0, ySobel, u, v);
	}


	printf("Exit program\n");
	yuvFinalize(0);

	return 0;
}
