#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DATA(c,x,y,ax,ay) (*(unsigned short*)&mem[c * (nsizex * nsizey * nangx * nangy * 2) + x * (nsizey * nangx * nangy * 2) + y * (nangx * nangy * 2) + ax * (nangy * 2) + ay * 2])

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "rb");
    int ncom = atoi(argv[2]);
    int nsizex = atoi(argv[3]);
    int nsizey = atoi(argv[4]);
    int nangx = atoi(argv[5]);
    int nangy = atoi(argv[6]);
    int size = ncom*nsizex*nsizey*nangx*nangy*2;
    char *mem = malloc(size);
    int r = fread(mem, 1, size, f);
    printf ("Read %d bytes, needed %d\n", r, size);
    fclose(f);

    f = fopen (argv[7], "wb");
    int c, x, y;
    for (y=0; y<nsizey; y++) {
        for (x=0; x<nsizex; x++) {
            for (c=0; c<3; c++) {
                unsigned char b = DATA(c,x,y,7,7) >> 8;
                fwrite(&b, 1, 1, f);
            }
        }
    }
    fclose(f);
}
