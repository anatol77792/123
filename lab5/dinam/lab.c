
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int
main (int argc, char **argv)
{
  int x, y;
  void *library_handle;
  int (*powerfunc) (int x, int y);
  if (argc < 2)
    {
      fprintf (stderr,
	       "Недостаточно аргументов. Пропишите <путь к библиотеке>\n");
      exit (1);
    }
  library_handle = dlopen (argv[1], RTLD_LAZY);
  if (!library_handle)
    {
      fprintf (stderr, "dlopen() error: %s\n", dlerror ());
      exit (1);
    }
  printf ("Введите х и у\n");
  scanf ("%d %d", &x, &y);
  powerfunc = dlsym (library_handle, "sum");
  printf ("Сумма: %d\n", (*powerfunc) (x, y));
  powerfunc = dlsym (library_handle, "raz");
  printf ("Разность: %d\n", (*powerfunc) (x, y));
  dlclose (library_handle);
  return 0;
}

