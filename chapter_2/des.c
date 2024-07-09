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
    int dest_bit = GET_BIT(src, (permute_table[ i ] - 1));

    if ( dest_bit ) {
      SET_BIT( target, i );
    } else {
      CLEAR_BIT( target, i );
    }
  }
}

static const int initial_permute_table[] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17,  9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7,
};

static const int final_permute_table[] = {
  40,  8, 48, 16, 56, 24, 64, 32,
  39,  7, 47, 15, 55, 23, 63, 31,
  38,  6, 46, 14, 54, 22, 62, 30,
  37,  5, 45, 13, 53, 21, 61, 29,
  36,  4, 44, 12, 52, 20, 60, 28,
  35,  3, 43, 11, 51, 19, 59, 27,
  34,  2, 42, 10, 50, 18, 58, 26,
  33,  1, 41,  9, 49, 17, 57, 25,
};

static const int key_permutation_1[] = {
  57, 49, 41, 33, 25, 17,  9,  1,
  58, 50, 42, 34, 26, 18, 10,  2,
  59, 51, 43, 35, 27, 19, 11,  3,
  60, 52, 44, 36, 63, 55, 47, 39,
  31, 23, 15,  7, 62, 54, 46, 38,
  30, 22, 14,  6, 61, 53, 45, 37,
  29, 21, 13,  5, 28, 20, 12,  4,
};

static const int key_permutation_2[] = {
  14, 17, 11, 24,  1,  5,  3, 28,
  15,  6, 21, 10, 23, 19, 12,  4,
  26,  8, 16,  7, 27, 20, 13,  2,
  41, 52, 31, 37, 47, 55, 30, 40,
  51, 45, 33, 48, 44, 49, 39, 56,
  34, 53, 46, 42, 50, 36, 29, 32,
};

// Rotates the 56-bit key into two 28-bit parts
static void rol( unsigned char *target, int c ) {

  int left_carry, right_carry;

  left_carry = right_carry = 0;

  for ( int i = 0; i < c; i++ ) {
    // Rotate the first 28-bits

    left_carry = (target[0] & 0x80) >> 3;

    target[0] = (target[0] << 1) | ((target[1] & 0x80) >> 7);
    target[1] = (target[1] << 1) | ((target[2] & 0x80) >> 7);
    target[2] = (target[2] << 1) | ((target[3] & 0x80) >> 7);

    right_carry = (target[3] >> 3) & 0x08;

    target[3] = (target[3] << 1) | ((target[4] & 0x80) >> 7);
    target[3] = target[3] | (left_carry >> 3);

    target[4] = (target[4] << 1) | ((target[5] & 0x80) >> 7);
    target[5] = (target[5] << 1) | ((target[6] & 0x80) >> 7);
    target[6] = (target[6] << 1) | right_carry;
  }
}


void print_ascii_data( unsigned char *data, int len ) {
  for ( int i = 0; i < len; i++ ) {
    printf( "%c ", data[ i ] );
  }
  printf("\n");
}
void print_hex_data( unsigned char *data, int len ) {
  for ( int i = 0; i < len; i++ ) {
    printf( "%02x ", data[ i ] );
  }
  printf("\n");
}

static void des_block_operate() {

}

int main() {

  int block_len = 8;
  unsigned char *plaintext  = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );
  unsigned char *enc_text   = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );
  unsigned char *final_text = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );

  plaintext[0] = 97;
  plaintext[1] = 110;
  plaintext[2] = 107;
  plaintext[3] = 105;
  plaintext[4] = 104;
  plaintext[5] = 111;
  plaintext[6] = 111;
  //plaintext[7] = 100;

  print_hex_data(plaintext, 7);
  rol(plaintext, 1);
  print_hex_data(plaintext, 7);

  /* permute( enc_text, plaintext, initial_permute_table, block_len ); */

  /* print_ascii_data( plaintext, block_len ); */
  /* print_hex_data( plaintext, block_len ); */

  /* print_hex_data( enc_text, block_len ); */
  /* permute( final_text, enc_text, final_permute_table, block_len ); */
  /* print_hex_data( final_text, block_len ); */
  /* print_ascii_data( final_text, block_len ); */

  return 0;
}
