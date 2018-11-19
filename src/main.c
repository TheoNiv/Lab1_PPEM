#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "yuvDisplay.h"
#include "yuvRead.h"
#include "sobel.h"
#include <pthread.h>

int stopThreads = 0;
// 
struct args_thread_sobel
{
	int width;
	int height;
	unsigned char *input;
	unsigned char *output;
};

int main(int argc, char** argv)
{
	// Declarations
	unsigned char y[HEIGHT*WIDTH], ySobel[HEIGHT*WIDTH], u[HEIGHT*WIDTH / 4], v[HEIGHT*WIDTH / 4];
	pthread_t Sobel_thread[2];

	// Init display
	yuvDisplayInit(0, WIDTH, HEIGHT);

	// Open file
	initReadYUV(WIDTH, HEIGHT);

	while (!stopThreads)
	{
		// Read a frame
		readYUV(WIDTH, HEIGHT, y, u, v);
		/*for (int i = 0; i < 2; ++i)
		{
			args_thread_sobel args[i] = {};
			pthread_create(&Sobel_thread[i], NULL, sobel, )
		}*/
		sobel(WIDTH, HEIGHT / 2, y, ySobel);
		sobel(WIDTH, 2 + (HEIGHT / 2), y + WIDTH * (HEIGHT / 2 - 2), ySobel + WIDTH* (HEIGHT / 2 - 2));

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
