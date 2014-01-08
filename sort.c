/* Simple example program demonstrating the use of the bitarray data structure.
   This program will read a list of numbers and then sort them.  It creates
   a bitarray of size equal to the range of numbers in the input, and sets the
   bits corresponding to each number. */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "bitarray.h"

struct numbers {
    unsigned long long int *n;
    unsigned long long int min;
    unsigned long long int max;
    size_t len;
};

struct numbers *read_numbers(FILE *fp, size_t max)
{
    struct numbers *r;
    size_t i = 0;
    char buf[BUFSIZ];

    if ((r = malloc(sizeof *r)) == NULL) {
        return NULL;
    }
    if ((r->n = malloc(max * sizeof *r->n)) == NULL) {
        free(r);
        return NULL;
    }
    r->min = ULLONG_MAX;
    r->max = 0;
    while (fgets(buf, sizeof buf, fp) != NULL) {
        unsigned long int num = strtoul(buf, NULL, 0);
        r->n[i++] = num;
        if (num > r->max) {
            r->max = num;
        }
        if (num < r->min) {
            r->min = num;
        }
        if (i == max) {
            fprintf(stderr, "%zu numbers read, breaking\n", max);
            break;
        }
    }
    if (i != max) {
        fprintf(stderr, "%zu numbers read\n", i);
    }
    fprintf(stderr, "min: %llu, max: %llu\n", r->min, r->max);
    r->len = i;
    return r;
}

#define MAX 1000001ULL

int main(int argc, char *argv[])
{
    struct numbers *nums = NULL;
    const char *filename = "num.dat";
    const char *ofilename = "num.out";
    FILE *fp = NULL;
    FILE *ofp = NULL;
    int ret = EXIT_FAILURE;
    size_t i;
    size_t range;
    size_t index;
    bitarray *barray = NULL;

    if (argc > 1) {
        filename = argv[1];
    }
    if ((fp = fopen(filename, "r")) == NULL) {
        perror(filename);
        goto done;
    }

    if (argc > 2) {
        ofilename = argv[2];
    }
    if ((ofp = fopen(ofilename, "w")) == NULL) {
        perror(ofilename);
        goto done;
    }

    if ((nums = read_numbers(fp, MAX)) == NULL) {
        perror("read_numbers");
        goto done;
    }

    range = nums->max - nums->min + 1;
    if ((barray = bitarray_init(range)) == NULL) {
        perror("bitarray_init");
        goto done;
    }

    for (i=0; i < nums->len; ++i) {
        bitarray_setbit(barray, nums->n[i] - nums->min);
    }

    index = 0;
    while ((i = bitarray_iter(barray)) != -1) {
        if (i) {
            fprintf(ofp, "%llu\n", nums->min + index);
        }
        ++index;
    }
    ret = EXIT_SUCCESS;

done:
    if (barray) {
        bitarray_free(barray);
    }

    if (nums) {
        free(nums->n);
        free(nums);
    }

    if (ofp) {
        fclose(ofp);
    }

    if (fp) {
        fclose(fp);
    }

    return ret;
}
