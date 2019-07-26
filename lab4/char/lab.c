
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE_FILENAME 256
int
main (int argc, char *argv[])
{
  FILE *fp;
  if (argc < 2)
    {
      printf
	("Не достаточно аргументов, исрользуйте <Имя входного файла> <Количество обрабатываемых строк>\n");
      exit (1);
    }
  if ((fp = fopen (argv[1], "r")) == NULL)
    {
      printf ("Не удается открыть файл.\n");
      exit (1);
    }

  char filename[SIZE_FILENAME];

  char new_filename[SIZE_FILENAME];
  strcpy (filename, argv[1]);
  printf ("%s\n", filename);
  int i = 0;
  while (filename[i] != '.')
    {
      new_filename[i] = filename[i];
      i++;
    }
  strcat (new_filename, ".from");

  printf ("%s\n", new_filename);
  FILE *fpv = fopen (new_filename, "w");
  char buf;
  int car;
  int kolch = atoi(argv[2]);

  while (((buf = fgetc(fp)) != EOF))
	{
		fputc(buf, fpv);
		if (buf == '.' && car != kolch)
		{
			fputc('\n', fpv);
			car++;
		}
	}

  if (fclose (fp))
    {
      printf ("Ошибка при закрытии файла");
      exit (1);
    }
  if (fclose (fpv))
    {
      printf ("Ошибка при закрытии файла");
      exit (1);
    }
  return 0;
}
