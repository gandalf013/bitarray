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
    struct numbers *nums;
    const char *filename = "num.dat";
    const char *ofilename = "num.out";
    FILE *fp;
    FILE *ofp;
    int ret = EXIT_FAILURE;
    size_t i;
    size_t range;
    size_t index;
    bitarray *barray;

    if (argc > 1) {
        filename = argv[1];
    }
    if ((fp = fopen(filename, "r")) == NULL) {
        perror(filename);
        goto fail0;
    }

    if (argc > 2) {
        ofilename = argv[2];
    }
    if ((ofp = fopen(ofilename, "w")) == NULL) {
        perror(ofilename);
        goto fail1;
    }

    if ((nums = read_numbers(fp, MAX)) == NULL) {
        perror("read_numbers");
        goto fail2;
    }

    range = nums->max - nums->min + 1;
    if ((barray = bitarray_init(range)) == NULL) {
        perror("bitarray_init");
        goto fail3;
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

fail4:
    bitarray_free(barray);

fail3:
    free(nums->n);
    free(nums);

fail2:
    fclose(ofp);

fail1:
    fclose(fp);

fail0:
    return ret;
}
