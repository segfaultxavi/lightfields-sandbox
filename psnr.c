// gcc psnr.c -o psnr -lm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_FILE_SIZE 512*1024*1024

int main(int argc, char *argv[])
{
    FILE *f1 = fopen(argv[1], "rb");
    FILE *f2 = fopen(argv[2], "rb");
    char *mem1 = malloc(MAX_FILE_SIZE);
    char *mem2 = malloc(MAX_FILE_SIZE);
    int r1 = fread(mem1, 1, MAX_FILE_SIZE, f1);
    int r2 = fread(mem2, 1, MAX_FILE_SIZE, f2);
    if (r1 != r2) {
      printf ("File sizes do not match %d != %d\n", r1, r2);
      return -1;
    }
    fclose(f1);
    fclose(f2);

    double error = 0;
    for (r1=0; r1<r2; r1++) {
      error += (mem1[r1] - mem2[r1]) * (mem1[r1] - mem2[r1]);
    }
    error /= r1;
    printf ("PSNR = %g\n", 10 * logf (255 * 255 / error));
}
