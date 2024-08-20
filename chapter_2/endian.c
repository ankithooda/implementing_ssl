#include <stdio.h>
#include <string.h>

int
main ()
{
  int x = 123456789;
  x = 3494632738;

  char s[4];

  char a, b, c, d;

  a = 'a';
  b = 'b';
  c = 'c';
  d = 'd';
  printf ("%c-%x\n", a, a);
  printf ("%c-%x\n", b, b);
  printf ("%c-%x\n", c, c);
  printf ("%c-%x\n", d, d);

  printf ("Size of int is %ld\n", sizeof (int));
  printf ("Size of char is %ld\n", sizeof (char));

  memcpy (s, &x, sizeof (int));

  printf ("%x\n", x);

  for (int i = 0; i < 4; i++)
    {
      printf ("%x,", s[i]);
    }
  printf ("\n");
  return 0;
}
