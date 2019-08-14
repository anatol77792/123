#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: количество юнитов\n");
		exit(-1);
	}
	int count = atoi(argv[1]);
	int i, pid[count];
	int zoloto = 1000;

	

	int fd[count][2];
	int fd1[count][2];

	for (i = 1; i < count; i++) {
		pipe(fd[i]);
		pipe(fd1[i]);
		pid[i] = fork();
		srand(getpid());

		if (-1 == pid[i]) {
			perror("fork");	/* произошла ошибка */
			exit(1);	/*выход из родительского процесса */
		} else if (0 == pid[i]) {
			while (1) {
				/* процесс-потомок закрывает доступный для чтения конец канала 0 */
				close(fd[i][0]);
				close(fd1[i][1]);
				/* записывает в канал 1 */
				write(fd[i][1], 0, sizeof (int));
				sleep(rand() % 6);
				int len = 10;
				write(fd[i][1], &len, sizeof (int));
				int zol = 0;
				read(fd1[i][0], &zol, sizeof (int));
				printf
				    ("Юнит %d Остаток золота в шахте=%d\n",
				     i, zol);
				sleep(1);
			}
		}
	}
	// если выполняется родительский процесс
	printf("PARENT: Это процесс-родитель!\n");
	// ожидание окончания выполнения всех запущенных процессов

	while (zoloto > 0) {

		for (i = 1; i < count; i++) {



			/* процесс-родитель закрывает доступный для записи конец канала 1 */
			close(fd[i][1]);
			close(fd1[i][0]);
			/* читает из канала 0 */
			int len = 0;
			read(fd[i][0], &len, sizeof (int));
			zoloto = zoloto - len;
			write(fd1[i][1], &zoloto, sizeof (int));
			sleep(1);
			if (zoloto <= 0) {
				printf("Золото закончилось\n");
			}

		}
	}

	return 0;
}
