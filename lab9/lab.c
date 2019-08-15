#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 1024
union semun {
	int val;		/* значение для SETVAL */
	struct semid_ds *buf;	/* буферы для  IPC_STAT, IPC_SET */
	unsigned short *array;	/* массивы для GETALL, SETALL */
	/* часть, особенная для Linux: */
	struct seminfo *__buf;	/* буфер для IPC_INFO */
};

/* сумма элементов массива в строке */
double execMath(char *str)
{

	double result = 0;
	for (int i = 0; i < strlen(str); ++i)
		result += str[i];

	return result;
}

void sumMatrixFromFile(char *fileName)
{
	FILE *fp;
	char str[MAX_LEN];
	int cntStr = 0;

	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("Не удается открыть файл.\n");
		exit(1);
	}

	/* Выводим содержимое и считае кол-во строк */
	printf("File %s:\n", fileName);
	while (!feof(fp)) {
		if (fgets(str, MAX_LEN, fp)) {
			printf("%s", str);
			cntStr++;
		}
	}
	rewind(fp);		//перемещает указатель файла в начало файла

	pid_t pid;
	pid_t wpid;
	int status = 0;

	int shmid;
	key_t key = 69;
	double *shm;

	int semid;
	union semun arg;
	struct sembuf lock_res = { 0, -1, 0 };	//блокировка ресурса
	struct sembuf rel_res = { 0, 1, 0 };	//освобождение ресурса

	/* Получим ключ, Один и тот же ключ можно использовать как
	   для семафора, так и для разделяемой памяти */
	if ((key = ftok(".", 'S')) < 0) {
		printf("Невозможно получить ключ\n");
		exit(1);
	}

	/* Создадим семафор - для синхронизации работы с разделяемой памятью. */
	semid = semget(key, 1, 0666 | IPC_CREAT);

	/* Установить в семафоре № 0 (Контроллер ресурса) значение "1" */
	arg.val = 1;
	semctl(semid, 0, SETVAL, arg);

	/* Создадим область разделяемой памяти */
	if ((shmid = shmget(key, sizeof (double), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}


	if (fgets(str, MAX_LEN, fp)) {
		pid = fork();
		srand(getpid());
		if (0 == pid) {


			double sum = execMath(str);


			/* Получим доступ к разделяемой памяти */
			if ((shm =
			     (double *) shmat(shmid, NULL,
					      0)) == (double *) -1) {
				perror("shmat");
				exit(1);
			}


			fflush(stdout);

			/* Заблокируем разделяемую память */
			if ((semop(semid, &lock_res, 1)) == -1) {
				fprintf(stderr, "Lock failed\n");
				exit(1);
			} else {

			}

			/* Запишем в разделяемую память сумму */
			*(shm) = sum;
			sleep(rand() % 4);

			/* Освободим разделяемую память */
			if ((semop(semid, &rel_res, 1)) == -1) {
				fprintf(stderr, "Unlock failed\n");
				exit(1);
			} else {

			}

			printf("Контрольная сумма = %f\n",
			       *(shm));
			fflush(stdout);

			/* Отключимся от разделяемой памяти */
			if (shmdt(shm) < 0) {
				printf("Ошибка отключения\n");
				exit(1);
			}
			exit(0);
		} else if (pid < 0) {
			perror("fork");	/* произошла ошибка */
			exit(1);	/*выход из родительского процесса */
		}
	}



	if (fclose(fp)) {
		printf("Ошибка при закрытии файла.\n");
	}

	wpid = waitpid(pid, &status, 0);
	if (pid == wpid) {
		printf("процесс-потомок done \n");
		fflush(stdout);
	}


	/* Получим доступ к разделяемой памяти */
	if ((shm = (double *) shmat(shmid, NULL, 0)) == (double *) -1) {
		perror("shmat");
		exit(1);
	}

	printf("------------------\n");
	printf("Контрольная сумма=%f\n", *(shm));
	fflush(stdout);

	if (shmdt(shm) < 0) {
		printf("Ошибка отключения\n");
		exit(1);
	}

	/* Удалим созданные объекты IPC */
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		printf("Невозможно удалить область\n");
		exit(1);
	} else
		printf
		    ("Сегмент памяти помечен для удаления\n");

	if (semctl(semid, 0, IPC_RMID) < 0) {
		printf("Невозможно удалить семафор\n");
		exit(1);
	}

}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: ./ filename filename filename.....\n");
		exit(-1);
	}
	for (int i = 1; i < argc; i++) {
		sumMatrixFromFile(argv[i]);
	}
	return 0;
}
