#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Bush.raw.reorder.nv12
#define YSIZE (nsizex * nsizey)
#define FRAMESIZE (YSIZE * 3 / 2)

int nsizex;
int nsizey;
int nangx;
int nangy;

int offs(int c, int x, int y, int ax, int ay)
{
  int offs = (ay * nangx + ax) * FRAMESIZE;
  if (c==0) return offs + y * nsizex + x;
  if (c==1) return offs + YSIZE + y * nsizex / 2 + x;
  if (c==2) return offs + YSIZE + YSIZE / 4 + y * nsizex / 2 + x;
}

// Bush.raw.reorder.nv12 624 432 15 15 Bush.raw.reorder.nv12.skip
int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "rb");
    nsizex = atoi(argv[2]);
    nsizey = atoi(argv[3]);
    nangx = atoi(argv[4]);
    nangy = atoi(argv[5]);
    int size = FRAMESIZE * nangx * nangy;
    unsigned char *mem = malloc(size);
    int r = fread(mem, 1, size, f);
    printf ("Read %d bytes, needed %d\n", r, size);
    fclose(f);

    f = fopen (argv[6], "wb");

    int angx, angy;
    for (angy=0; angy<nangy; angy++) {
        for (angx=0; angx<nangx; angx++) {
            if (((angx ^ angy) & 1) == 1) continue;
            fwrite (mem + offs(0, 0, 0, angx, angy), FRAMESIZE, 1, f);
        }
    }

    fclose(f);
}
