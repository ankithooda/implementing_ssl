#include <stdio.h>
#include <string.h>

int main() {
  int x = 123456789;
  x = 3494632738;

  char s[8];

  memcpy(s, &x, sizeof(int));

  printf("%x\n", x);

  for (int i = 0; i < 8; i++) {
    printf("%.2x", s[i]);
  }
  printf("\n");
  return 0;
}
