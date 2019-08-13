#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

    
void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
typedef struct student{
    int x;
    int y;
}students;
 
void randomi(int x1, int y1){
     
      int pid1, stat;

      FILE *fp;
      const int BUFF_SIZE = 1024;
    char buff[BUFF_SIZE];
	char buff11[BUFF_SIZE];
      
     pid1 = fork();
        srand(getpid());

        if (-1 == pid1) {
            perror("fork"); /* произошла ошибка */
            exit(1); /*выход из родительского процесса*/
        } else if (0 == pid1) {
			
			
				if (NULL == (fp = fopen("buf.buf", "w")))
				{
					printf("File %s cannot open.\n", "buf.buf");
					exit(1);
				}
			x1 = x1 + rand() % 3-1;
			y1 = y1 + rand() % 3-1;
            sleep(rand() % 1);
           
		        itoa(x1, buff);
				strcat(buff, " ");
				
				itoa(y1, buff11);
				strcat(buff, buff11);
				
			
				strcat(buff, " ");
				strcat(buff, "\n");

				fputs(buff, fp);

				if(fclose(fp))
				{ 
					printf("Error close of file.\n");
					exit(1);
				}
            exit(0); /* выход из процесс-потомока */
        }
     waitpid(pid1, &stat, 0);   
} 
void chten(students *st){
	FILE *fp;
	const int BUFF_SIZE = 1024;
    char buff1[BUFF_SIZE];
	 if (NULL == (fp = fopen("buf.buf", "r")))
		{
			printf("File %s cannot open.\n", "buf.buf");
			exit(1);
		}


      
        fgets(buff1, BUFF_SIZE, fp);

			char *red = strtok(buff1, " ");
				st->x = atoi(red);

				red = strtok(NULL, " ");
				st->y = atoi(red);
}


 
int main(int argc, char **argv){
	
    if (argc < 2) {
		printf("Мало аргументов. Используйте <кол-во шариков>\n");
		exit(-1);
	}
	int count = atoi(argv[1]);
    students** st = (students**)malloc(sizeof(students*)*count);
    for (int i = 0; i < count ; i++){
        st[i] = (students*) malloc (sizeof(students));
        randomi(st[i]->x, st[i]->y);
        chten(st[i]);
       
    } 
     for (int i = 0; i < count ; i++) {
		printf("шарик:%d  ", i);
        printf("x:%d  ", st[i]->x);
        printf("y:%d\n", st[i]->y);
    }  
    
    for (int j = 0; j < count ; j++){
		
		while ((st[j]->x <= 3 && st[j]->x >= -1) && (st[j]->y <= 3 && st[j]->y >= -1)){
			printf("бросок\n");
    for (int i = 0; i < count ; i++){
		 
		
         if ((st[i]->x <= 3 && st[i]->x >= -1) && (st[i]->y <= 3 && st[i]->y >= -1)){
			 
			 randomi(st[i]->x, st[i]->y);
             chten(st[i]); 
             
            
             
			 printf("шарик:%d  ", i);	 
		     printf("x:%d  ", st[i]->x);
             printf("y:%d\n", st[i]->y);
		 }
		 
		 else{
		 printf("шарик:%d вылетел за доп область\n", i);
	 }
	 
	 }
		 
	}	 
	 } 
        printf("конечные координаты шариков:\n");
      for (int i = 0; i < count ; i++) {
		printf("шарик:%d  ", i);
        printf("x:%d  ", st[i]->x);
        printf("y:%d\n", st[i]->y);
    }  
    
    for (int i = 0; i < count; i++)
    {
        free(st[i]);
    }
    free(st);
    return 0;
}
