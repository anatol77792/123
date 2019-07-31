#include <stdio.h>
#include <stdlib.h>



int
main (int argc, char **argv)
{
  int x, y, result;
  printf ("Введите х и у\n");
  scanf ("%i %i", &x, &y);
  result = sum (x, y);
  printf ("Сумма: %i\n", result);
  result = raz (x, y);
  printf ("Разность: %i\n", result);
  return 0;
}
