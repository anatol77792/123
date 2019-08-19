#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#define NN 10
#define M 10
pthread_mutex_t mutex;
typedef struct DATA {
	int A[NN][M];
	int h;
} Data;
int B[NN][M];

void *square_of_triangle(void *arg)
{


	srand(pthread_self());
	int x = 1 + rand() % (10);
	int y = 1 + rand() % (10);
	int kolcel = 0;

	Data *a = (Data *) arg;
	pthread_mutex_lock(&mutex);
	if ((y >= x) && (y >= M / 2)) {
		printf("Разведчик %d\n", a->h);
		printf("x=%d, y=%d\n", x, y);
		fflush(stdout);
		for (int j = 1; j <= y; j++) {
			printf("%d ", a->A[x][j]);
			fflush(stdout);
			B[x][j] = a->A[x][j];
			if (a->A[x][j] == 1) {
				kolcel = kolcel + 1;
			}
		}
		puts("\n");

	} else {
		if ((y >= x) && (y <= M / 2)) {
			printf("Разведчик %d\n", a->h);
			printf("x=%d, y=%d\n", x, y);
			fflush(stdout);
			for (int j = y; j <= M; j++) {
				printf("%d ", (a->A[x][j]));
				fflush(stdout);
				B[x][j] = a->A[x][j];
				if (a->A[x][j] == 1) {
					kolcel = kolcel + 1;
				}
			}
			puts("\n");

		} else {
			if ((y <= x) && (x >= NN / 2)) {
				printf("Разведчик %d\n", a->h);
				printf("x=%d, y=%d\n", x, y);
				fflush(stdout);
				for (int ii = 1; ii <= x; ii++) {
					printf("%d ", a->A[ii][y]);
					fflush(stdout);
					B[ii][y] = a->A[ii][y];
					if (a->A[ii][y] == 1) {
						kolcel = kolcel + 1;
					}
					puts("\n");
				}
			} else {
				printf("Разведчик %d\n", a->h);
				printf("x=%d, y=%d\n", x, y);
				fflush(stdout);
				for (int ii = x; ii <= NN; ii++) {
					printf("%d ", a->A[ii][y]);
					fflush(stdout);
					B[ii][y] = a->A[ii][y];
					if (a->A[ii][y] == 1) {
						kolcel = kolcel + 1;
					}
					puts("\n");
				}
			}
		}

	}

	int *ps = malloc(sizeof (long int));

	*ps = kolcel;

	pthread_mutex_unlock(&mutex);
	sleep(1);
	pthread_exit((void *) ps);
	//return (void*)ps;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf
		    ("Usage: <количество разведчиков>\n");
		exit(-1);
	}
	int N = atoi(argv[1]);
	int result;
	pthread_t threads[N];
	Data data[N];
	void *status[N];
	int ii, j, i;
	int AA[NN][M];

	for (ii = 1; ii <= NN; ii++) {
		for (j = 1; j <= M; j++) {
			B[ii][j] = 7;

		}
	}

	for (ii = 1; ii <= NN; ii++) {
		for (j = 1; j <= M; j++) {
			AA[ii][j] = rand() % (2);
			printf("%d ", AA[ii][j]);
			fflush(stdout);
		}
		puts("\n");
	}
	for (i = 0; i < N; i++) {
		for (ii = 1; ii <= NN; ii++) {
			for (j = 1; j <= M; j++) {
				data[i].A[ii][j] = AA[ii][j];
			}
		}
	}
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < N; i++) {
		data[i].h = i;
		result =
		    pthread_create(&threads[i], NULL, square_of_triangle,
				   &data[i]);
		if (result != 0) {
			perror("Creating the first thread");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < N; i++) {
		result = pthread_join(threads[i], &status[i]);
		if (result != 0) {
			perror("Joining the first thread");
			return EXIT_FAILURE;
		} else {
			printf("Количество целей[%d]=%d\n", i,
			       *((int *) status[i]));
			fflush(stdout);
		}
		free(status[i]);
	}
	pthread_mutex_destroy(&mutex);
	for (ii = 1; ii <= NN; ii++) {
		for (j = 1; j <= M; j++) {

			printf("%d ", B[ii][j]);
			fflush(stdout);
		}
		puts("\n");
	}
	printf("Done..\n");
	return EXIT_SUCCESS;
}
