#include <stdio.h>
#include <stdlib.h>

// This does not return a 1 for a 1 bit; it just returns non-zero
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

/**
 * Implement the initial and final permutation functions. permute_table
 * and target must have exactly len and len * 8 number of entries,
 * respectively, but src can be shorter (expansion function depends on this).
 * NOTE: this assumes that the permutation tables are defined as one-based
 * rather than 0-based arrays, since they're given that way in the
 * specification.
 */
static void permute( unsigned char target[],
           const unsigned char src[],
           const int permute_table[],
           int len )
{
  int i;
  for ( i = 0; i < len * 8; i++ )
  {
    if ( GET_BIT( src, ( permute_table[ i ] - 1 ) ) )
    {
      SET_BIT( target, i );
    }
    else
    {
      CLEAR_BIT( target, i );
    }
  }
}

static const int initial_permute_table[] = {
   58, 50, 42, 34, 26, 18, 10, 2,
   60, 52, 44, 36, 28, 20, 12, 4,
   62, 54, 46, 38, 30, 22, 14, 6,
   64, 56, 48, 40, 32, 24, 16, 8,
   57, 49, 41, 33, 25, 17, 9,  1,
   59, 51, 43, 35, 27, 19, 11, 3,
   61, 53, 45, 37, 29, 21, 13, 5,
   63, 55, 47, 39, 31, 23, 15, 7 };

/**
 * This just inverts ip_table.
 */
static const int final_permute_table[] = { 40, 8, 48, 16, 56, 24, 64, 32,
                                39, 7, 47, 15, 55, 23, 63, 31,
                                38, 6, 46, 14, 54, 22, 62, 30,
                                37, 5, 45, 13, 53, 21, 61, 29,
                                36, 4, 44, 12, 52, 20, 60, 28,
                                35, 3, 43, 11, 51, 19, 59, 27,
                                34, 2, 42, 10, 50, 18, 58, 26,
                                33, 1, 41,  9, 49, 17, 57, 25 };

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
  plaintext[7] = 100;


  permute( enc_text, plaintext, initial_permute_table, block_len );

  print_ascii_data( plaintext, block_len );
  print_hex_data( plaintext, block_len );

  print_hex_data( enc_text, block_len );
  permute( final_text, enc_text, final_permute_table, block_len );
  print_hex_data( final_text, block_len );
  print_ascii_data( final_text, block_len );

  return 0;
}
