#include <stdio.h>
#include <stdlib.h>

#define GET_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] & ( 0x80 >> ( bit % 8 ) ) )
#define SET_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] |= ( 0x80 >> ( bit % 8 ) ) )
#define CLEAR_BIT( array, bit ) \
    ( array[ ( int ) ( bit / 8 ) ] &= ~( 0x80 >> ( bit % 8 ) ) )

#define DES_BLOCK_SIZE         8 // 64-bits
#define DES_KEY_SIZE           8 // 56-bit key is used, 8 are ignored
#define EXPANSION_BLOCK_SIZE   6 // 32-bit half is expanded to 48-bits
#define PC1_KEY_SIZE           7 // Initial key permutation (56-bits)
#define SUBKEY_SIZE            6 // 48-bit subkeys

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

static const int expansion_table[] = {
  32,  1,  2,  3,  4,  5,
  4,   5,  6,  7,  8,  9,
  8,   9, 10, 11, 12, 13,
  12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21,
  20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29,
  28, 29, 30, 31, 31,  1,
};

static const int sbox[8][64] = {

  { 14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1,
    3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8,
    4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7,
    15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13 },
  { 15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14,
    9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5,
    0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2,
    5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9 },
  { 10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10,
    1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1,
    13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7,
    11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12 },
  { 7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3,
    1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9,
    10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8,
    15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14 },
  { 2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1,
    8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6,
    4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13,
    15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3 },
  { 12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5,
    0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8,
    9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10,
    7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13 },
  { 4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10,
    3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6,
    1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7,
    10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12 },
  { 13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4,
    10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2,
    7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13,
    0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11 }
};

// Rotates the 56-bit key into two 28-bit parts
static void rol( unsigned char *target) {

  int left_carry, right_carry;

  left_carry = right_carry = 0;

  // Rotate the first 28-bits

  left_carry = (target[0] & 0x80) >> 3;

  target[0] = (target[0] << 1) | ((target[1] & 0x80) >> 7);
  target[1] = (target[1] << 1) | ((target[2] & 0x80) >> 7);
  target[2] = (target[2] << 1) | ((target[3] & 0x80) >> 7);

  right_carry = (target[3] & 0x08) >> 3;

  target[3] = (((target[3] << 1) | ((target[4] & 0x80) >> 7)) & ~0x10) | left_carry;

  target[4] = (target[4] << 1) | ((target[5] & 0x80) >> 7);
  target[5] = (target[5] << 1) | ((target[6] & 0x80) >> 7);
  target[6] = (target[6] << 1) | right_carry;
}

static const int p_table[] =
  { 16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25 };


static void des_block_operate( const unsigned char plaintext[ DES_BLOCK_SIZE ],
                               unsigned char ciphertext[ DES_BLOCK_SIZE ],
                               const unsigned char key[ DES_KEY_SIZE ]
                               ) {


  unsigned char ip_block[ DES_BLOCK_SIZE ];              // Holds the plaintext that is being encrypted.
  unsigned char pc1_key[ PC1_KEY_SIZE ];                 // Initial permuted key (56-bits)
  unsigned char expanded_block[ EXPANSION_BLOCK_SIZE ];  // Holds the expanded 48-bits
  unsigned char subkey[ SUBKEY_SIZE ];

  // Initial permutation of the plaintext
  permute( ip_block, plaintext, initial_permute_table, DES_BLOCK_SIZE );

  // Initial Key permutation
  permute( pc1_key, key, key_permutation_1, PC1_KEY_SIZE );

  for ( int round = 1; round =< 16; round++ ) {

    // Expand right-half that is bits 32-64 of plaintext to 48-bits
    permute( expanded_block, ip_table + 4, expansion_table, EXPANSION_BLOCK_SIZE );

    // Key Mixing
    // Rotate both halves once using the specialized rol function
    // which rotates the two 28-bits independently.
    rol( pc1_key );

    // Round numbers are 1-indexed
    // For Rounds 1, 2, 9 and 16 there is only 1 rotation
    // For all other rounds there are 2 rotations

    if ( (round != 1) || (round != 2) || (round != 9) || (round != 16) ) {
      rol( pc1_key );
    }

    // Permute the pc1_key to create subkey for this round
    permute( subkey, pc1_key, key_permutation_2, SUBKEY_SIZE );

    // XOR the expanded block and subkey for this round.
    // expanded_block is right half of ip_table expanded to 48-bits
    // subkey is the 48-bit round key
    xor(expanded_block, subkey, SUBKEY_SIZE);


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


int main() {

  int block_len = 8;
  unsigned char *plaintext  = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );
  unsigned char *enc_text   = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );
  unsigned char *final_text = (unsigned char *)malloc( block_len * sizeof( unsigned char ) );

  plaintext[0] = 0x7A;
  plaintext[1] = 0xBC;
  plaintext[2] = 0xDE;
  plaintext[3] = 0xF8;
  plaintext[4] = 0xFF;
  plaintext[5] = 0xFF;
  plaintext[6] = 0xFF;
  //plaintext[7] = 0xFF;

  print_hex_data(plaintext, 7);
  rol(plaintext, 2);
  print_hex_data(plaintext, 7);

  // rol_1(plaintext, 1);


  /* permute( enc_text, plaintext, initial_permute_table, block_len ); */

  /* print_ascii_data( plaintext, block_len ); */
  /* print_hex_data( plaintext, block_len ); */

  /* print_hex_data( enc_text, block_len ); */
  /* permute( final_text, enc_text, final_permute_table, block_len ); */
  /* print_hex_data( final_text, block_len ); */
  /* print_ascii_data( final_text, block_len ); */

  return 0;
}
