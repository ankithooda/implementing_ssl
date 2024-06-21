#include <stdio.h>

static char *base64 = "ABCEFGHIJKLMNOPQSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Assume enough memory in dest
void base64_encode(char *src, int n, char *dest) {

  // Read in chunks of three chars

  // i tracks the index in the src.
  // j tracks the index in the dest.
  int j = 0;
  for (int i = 0; i < n; i++) {

    // Three bytes in src encodes to 4 bytes in dest.

    // First byte of dest
    dest[j] = base64[src[i] >> 2];
    j++;
    i++;

    // Second byte of dest
    if (i + 1 < n) {

    }
  }

}

void base64_decode(char *src, int n, char *dest) {

}

int main() {

  return 0;
}
