#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Assume enough memory in dest
void base64_encode(char *src, int n, char *dest) {

  // Read in chunks of three chars

  // i tracks the index in the src.
  // j tracks the index in the dest.
  int j = 0;
  /* int i = 0; */

  for (int i = 0; i < n; i = i + 3) {

    // Three bytes in src encodes to 4 bytes in dest.

    // First byte of dest
    dest[j++] = base64[ (src[i] & 0XFC) >> 2];

    // Second byte of dest
    if (i + 1 < n) {
      dest[j++] = base64[ (src[i] & 0x3) << 4 | (src[i + 1] & 0xF0) >> 4 ];
    } else {
      dest[j++] = base64[ (src[i] & 0x3) << 4 ];
      dest[j++] = '=';
      dest[j++] = '=';
      break;
    }

    // Third byte of dest
    if (i + 2 < n) {
      dest[j++] = base64[ (src[i+1] & 0x0F) << 2 | (src[i+2] & 0xC0) >> 6 ];
    } else {
      dest[j++] = base64[ (src[i+1] & 0x0F) << 2 ];
      dest[j++] = '=';
      break;
    }

    // Fourth byte of dest
    dest[j++] = base64[ src[i+2] & 0x3F ];
  }

}

/* void base64_decode(char *src, int n, char *dest) { */

/* } */

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage : base64 <data to be decoded>\n");
    exit(1);
  }
  int src_len = strlen(argv[1]);
  int dest_len = 4 * (int)ceil((double)src_len / 3);
  char *dest = (char*)malloc(dest_len);

  printf("Allocated buffer of len %d\n", dest_len);

  base64_encode(argv[1], src_len, dest);
  printf("%s\n", argv[1]);
  printf("%s\n", dest);
  free(dest);
  return 0;
}
