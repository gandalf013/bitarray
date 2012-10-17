#ifndef BITARRAY_H_
#define BITARRAY_H_ 1

typedef struct bitarray bitarray;

bitarray *bitarray_init(size_t len);
void bitarray_free(bitarray *array);
void bitarray_setbit(bitarray *array, size_t n);
void bitarray_clearbit(bitarray *array, size_t n);
unsigned char bitarray_get_next(bitarray *array);
size_t bitarray_iter(bitarray *array);

#endif /* BITARRAY_H_ */
