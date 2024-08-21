#include <stdio.h>

// This does not return a 1 for a 1 bit; it just returns non-zero
#define GET_BIT(byte, bit) byte &(0x80 >> (bit % 8))
#define SET_BIT(byte, bit) byte |= (0x80 >> (bit % 8))
#define CLEAR_BIT(byte, bit) byte &= ~(0x80 >> (bit % 8))

int get_bit(char *byte, int bit)
{
	return *byte & (0x80 >> bit);
}

void set_bit(char *byte, int bit)
{
	*byte = *byte | (0x80 >> bit);
}

void clr_bit(char *byte, int bit)
{
	*byte = *byte & ~(0x80 >> bit);
}

int main()
{
	char a = 0xCF;
	printf("%x\n", a);

	int b = get_bit(&a, 7);
	printf("%x-%d\n", a, b);

	set_bit(&a, 5);
	printf("%x\n", a);

	clr_bit(&a, 5);
	printf("%x\n", a);

	/////////////////// Author's Impl ///////////////
	printf("-------------------------------------------------\n");

	char x = 0xCF;
	printf("%x\n", x);

	int xx = GET_BIT(x, 7);
	printf("%x-%d\n", x, xx);

	SET_BIT(x, 5);
	printf("%x\n", x);

	CLEAR_BIT(x, 5);
	printf("%x\n", x);

	return 0;
}
