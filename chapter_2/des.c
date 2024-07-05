#include <stdio.h>
#include <stdlib.h>

#define GET_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] & ( 0x80 >> ( bit % 8 ) ) )
#define SET_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] |= ( 0x80 >> ( bit % 8 ) ) )
#define CLEAR_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] &= ~( 0x80 >> ( bit % 8 ) ) )

static void xor( unsigned char *target, const unsigned char *src, int len )
{
  while ( len-- )
  {
    *target++ ^= *src++;
  }
}

// Initial Permutation

static void permute( unsigned char *target, const unsigned char *src, const int *permute_table, const int len ) {

  for ( int i = 0; i < len * 8; i++ ) {
    int dest_bit = src[ permute_table[ i] - 1 ];

    if ( dest_bit ) {
      SET_BIT( target, i);
    } else {
      CLEAR_BIT( target, i);
    }
  }
}

static const int initial_permute_table[] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 26, 8,
  57, 49, 41, 33, 25, 17, 9,  1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 25, 7,
};

void print_data( unsigned char *data, int len ) {
  for ( int i = 0; i < len; i++ ) {
    printf( "%02x ", data[ i ] );
  }
  printf("\n");
}


int main() {

  int block_len = 8;
  const unsigned char *plaintext = "ANKTHOOD";
  unsigned char *enc_text = (unsigned char *)malloc(block_len * sizeof(unsigned char));

  permute(enc_text, plaintext, initial_permute_table, block_len);

  print_data(plaintext, block_len);
  print_data(enc_text, block_len);

  return 0;
}
