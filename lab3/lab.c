#include <stdio.h>
#include <stdlib.h>
typedef struct student{
    char namekom[50];
    char nameigri[50];
    int balls;
    int summ;
}students;
 
void readStudent(students *st){
    printf("Введите название команды:");
    scanf("%s", st->namekom);
    printf("Введите Игры:");
    scanf("%s", st->nameigri);
     printf("Введите очки:");
    scanf("%d", &st->balls);
    printf("Введите сумму призового фонда:");
    scanf("%d", &st->summ);
    
}

static int cmp(const void *p1, const void *p2){
    students * st1 = *(students**)p1;
    students * st2 = *(students**)p2;
    return st1->summ - st2->summ;
}


 
int main(int argc, char **argv){
    int count = 3;
    printf("Введите кол-во команд:");
    scanf("%d", &count);
    students** st = (students**)malloc(sizeof(students*)*count);
    for (int i = 0; i < count ; i++){
        st[i] = (students*) malloc (sizeof(students));
        readStudent(st[i]);
    } 
    qsort(st, count, sizeof(students*), cmp);
    printf("%ld\n", sizeof(students*));
      for (int i = 0; i < count ; i++) {
		printf("Название команды:%s\n", st[i]->namekom);
		printf("Название игры:%s\n", st[i]->nameigri);
        printf("Очки:%d\n", st[i]->balls);
        printf("Сумма призового фонда:%d\n", st[i]->summ);
    }  
    
    for (int i = 0; i < count; i++)
    {
        free(st[i]);
    }
    free(st);
    return 0;
}
/ vkh
