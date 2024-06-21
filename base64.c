#include <stdio.h>

static char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Assume enough memory in dest
void base64_encode(char *src, int n, char *dest) {

  // Read in chunks of three chars

  // i tracks the index in the src.
  // j tracks the index in the dest.
  int j = 0;
  int i = 0;

  /* for (int i = 0; i < n; i++) { */

    // Three bytes in src encodes to 4 bytes in dest.

    // First byte of dest
    dest[j] = base64[ (src[i] & 0XFC) >> 2];
    j++;
    i++;

    // Second byte of dest
    dest[j] = base64[ (src[i-1] & 0x3) << 4 | (src[i] & 0xF0) >> 4 ];
    j++;
    i++;

    // Third byte of dest
    dest[j] = base64[ (src[i-1] & 0x0F) << 2 | (src[i] & 0xC0) >> 6 ];
    j++;

    // Fourth byte of dest
    dest[j] = base64[ src[i] & 0x3F ];


  /* } */

}

/* void base64_decode(char *src, int n, char *dest) { */

/* } */

int main() {
  char *s = "ABC";
  char d[5];

  char x = 'X';

  printf("%ld - %ld \n", sizeof(x), sizeof(s));

  base64_encode(s, 3, d);
  printf("%s\n", s);
  printf("%s\n", d);
  return 0;
}
