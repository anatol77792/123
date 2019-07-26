
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_FILENAME 256
#define SIZE_BUF 5000
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
  char buf[SIZE_BUF];
  int kolstr = atoi (argv[2]);

  for (int tstr = 0; tstr != kolstr; tstr++)
    {
      int strsches = 0;
      printf ("%d\n", tstr);
      fgets (buf, SIZE_BUF, fp);
      for (int i = 0; buf[i] != '\n'; i++)
	{
	  if (buf[i] >= '0' && buf[i] <= '9')
	    strsches = 1;

	}

      if (strsches != 1)
	fputs (buf, fpv);
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
