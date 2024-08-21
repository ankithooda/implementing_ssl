#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *base64 =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int unbase64[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54,
	55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0,  -1, -1, -1, 0,  1,	2,
	3,  4,	5,  6,	7,  8,	9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, -1, -1, -1, -1, -1, -1

};

// Assume enough memory in dest
void base64_encode(char *src, int n, char *dest)
{
	// Read in chunks of three chars

	// i tracks the index in the src.
	// j tracks the index in the dest.
	int j = 0;
	/* int i = 0; */

	for (int i = 0; i < n; i = i + 3) {
		// Three bytes in src encodes to 4 bytes in dest.

		// First byte of dest
		dest[j++] = base64[(src[i] & 0XFC) >> 2];

		// Second byte of dest
		if (i + 1 < n) {
			dest[j++] = base64[(src[i] & 0x3) << 4 |
					   (src[i + 1] & 0xF0) >> 4];
		} else {
			dest[j++] = base64[(src[i] & 0x3) << 4];
			dest[j++] = '=';
			dest[j++] = '=';
			break;
		}

		// Third byte of dest
		if (i + 2 < n) {
			dest[j++] = base64[(src[i + 1] & 0x0F) << 2 |
					   (src[i + 2] & 0xC0) >> 6];
		} else {
			dest[j++] = base64[(src[i + 1] & 0x0F) << 2];
			dest[j++] = '=';
			break;
		}

		// Fourth byte of dest
		dest[j++] = base64[src[i + 2] & 0x3F];
	}
}

int base64_decode(char *src, int n, char *dest)
{
	// j tracks the position in dest.
	// i tracks the position in src.

	if ((n % 4) != 0) {
		return -1;
	}
	// 4 bytes of src decodes to 3 bytes of dest.
	int j = 0;
	for (int i = 0; i < n; i = i + 4) {
		char a, b;

		// Return if invalid base64 code.
		if ((unbase64[src[i + 0]] == -1) ||
		    (unbase64[src[i + 1]] == -1) ||
		    (unbase64[src[i + 2]] == -1) ||
		    (unbase64[src[i + 3]] == -1)) {
			return -1;
		}

		// 1st byte of dest
		a = (char)unbase64[src[i]];
		b = (char)unbase64[src[i + 1]];
		dest[j++] = ((a & 0x3F) << 2) | ((b & 0x30) >> 4);

		// Check for incorrect padding, before the 2nd byte of dest
		if (src[i + 2] == '=' && src[i + 3] != '=') {
			return -1;
		}

		// Check for double padding
		if (src[i + 2] == '=' && src[i + 3] == '=') {
			return 0;
		}

		// 2nd byte of dest
		a = (char)unbase64[src[i + 1]];
		b = (char)unbase64[src[i + 2]];
		dest[j++] = ((a & 0x0F) << 4) | ((b & 0x3C) >> 2);

		// Check for single pad
		if (src[i + 3] == '=') {
			return 0;
		}

		// 3d byte of dest
		a = (char)unbase64[src[i + 2]];
		b = (char)unbase64[src[i + 3]];
		dest[j++] = ((a & 0x03) << 6) | b;
	}
	return 0;
}
