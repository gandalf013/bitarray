#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "bitarray.h"

struct bitarray {
    char *base;
    size_t curr;
    size_t len;
};

bitarray *bitarray_init(size_t len)
{
    bitarray *b = malloc(sizeof *b);
    if (b == NULL) {
        return NULL;
    }
    b->base = malloc(len);
    if (b->base == NULL) {
        free(b);
        return NULL;
    }
    b->curr = 0;
    b->len = len;
    memset(b->base, 0, len);
    return b;
}

void bitarray_free(bitarray *array)
{
    if (array) {
        free(array->base);
    }
    free(array);
}

void bitarray_setbit(bitarray *array, size_t n)
{
    size_t index = n / CHAR_BIT;
    size_t extra = n % CHAR_BIT;
    unsigned char shift = 0x80U >> extra;
    array->base[index] |= shift;
}

void bitarray_clearbit(bitarray *array, size_t n)
{
    size_t index = n / CHAR_BIT;
    size_t extra = n % CHAR_BIT;
    unsigned char shift = (0x80U >> extra) ^ 0xffU;
    array->base[index] &= shift;
}

unsigned char bitarray_get_next(bitarray *array)
{
    size_t n = array->curr;
    size_t index = n / CHAR_BIT;
    size_t extra = n % CHAR_BIT;
    unsigned char shift = 0x80U >> extra;
    array->curr += 1;
    if (array->base[index] & shift) {
        return 1;
    } else {
        return 0;
    }
}

size_t bitarray_iter(bitarray *array)
{
    if (array->curr == array->len) {
        return -1;
    }
    return bitarray_get_next(array);
}

void bitarray_print(FILE *fp, bitarray *array)
{
    int i;
    int j;
    while ((i = bitarray_iter(array)) != -1) {
        fprintf(fp, "%d", i);
        if (++j == 8) {
            fprintf(fp, " ");
            j = 0;
        }
    }
    fprintf(fp, "\n");
}

#ifdef UT
int main(void)
{
    size_t n = 35;
    size_t bits_to_set[] = { 3, 5, 12, 13, 14, 27 };
    size_t nbits = sizeof bits_to_set / sizeof bits_to_set[0];
    size_t i;
    bitarray *a = bitarray_init(n);
    if (a == NULL) {
        perror("bitarray_init");
        return EXIT_FAILURE;
    }
    for (i=0; i < nbits; ++i) {
        bitarray_setbit(a, bits_to_set[i]);
    }
    bitarray_print(stdout, a);
    bitarray_free(a);
    return EXIT_SUCCESS;
}
#endif
