#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>
#define MAX_LEN 1024 
 
int inp_str(char *string, int maxlen) {
    scanf("%[^\n]", string);
    getchar();
    return strlen(string);
}

char** readMas(int count, char **mas, int maxlen) {
    for (int i = 0; i < count ; i++) {
		char buf[MAX_LEN];
        int len = inp_str(buf, maxlen);
        mas[i] = (char *)malloc(sizeof(char)*len);
        strcpy(mas[i], buf);
        
    }
    return mas;
}

void printMas(char **mas, int count) {
    for (int i = 0; i < count ; i++) {
        printf("%s\n", mas[i]);
    }
}

void freeMas(char **mas, int count) {
    for (int i = 0; i < count; i++) {
        free(mas[i]);
    }
    free(mas);
}

int kolslov(char *str) {
    int len = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i]==' ') {
            len++;
        } else {
            ;
        }
    }
    return len;
}

int maxkolslov (char *str1) {
    int len1 = 0;
    for (int i = 0; i < strlen(str1); i++)
    {
        if (str1[i]==' ') {
            len1++;
        } else {
            ;
        }
    }
    return len1+1;
}



int bubleSort(char **mas, int n) {
    int c = 0;
    int d = 0;
    int swichcount = 0;
    char *swap;
    for (c = 0 ; c < ( n - 1 ); c++) {
        for (d = 0 ; d < n - c - 1; d++) {
            if (kolslov(mas[d+1]) > kolslov(mas[d])) {
                swap = mas[d];
                mas[d] = mas[d+1];
                mas[d+1] = swap;

                swichcount++;
            }
        }
    }
    return swichcount;
}

int main(int argc, char **argv) {
    char **mas = NULL;
    int count = 0;
    mtrace();
    printf("Введите кол-во строк:");
    scanf("%d", &count);
    printf("n=%i\n", count);
    getchar();
    mas = (char **)malloc(sizeof(char *)*count);
    mas = readMas(count, mas, MAX_LEN);
    printMas(mas, count);
    int swichcount = bubleSort(mas, count);

    printMas(mas, count);
    printf("Кол-во перестановок:");
    printf("%d\n", swichcount);
    printf ("Максимальное количество слов «%s» - %d\n", mas[0], maxkolslov(mas[0]) );
    freeMas(mas, count);
}
