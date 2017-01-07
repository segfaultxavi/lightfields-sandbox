#include <stdio.h>
#include <stdlib.h>

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

// out/Bush.skip.jpg-30.dec 624 432 15 15 out/Bush.skip.jpg-30.dec.interp
int main(int argc, char *argv[])
{
    FILE *fin = fopen(argv[1], "rb");
    nsizex = atoi(argv[2]);
    nsizey = atoi(argv[3]);
    nangx = atoi(argv[4]);
    nangy = atoi(argv[5]);
    int size = FRAMESIZE * nangx * nangy;
    unsigned char *mem = malloc(size);
    FILE *fout = fopen(argv[6], "wb");

    int angx, angy;
    double error = 0;
    int nsamples = 0;
    printf ("Reading...\n");
    for (angy=0; angy<nangy; angy++) {
        for (angx=0; angx<nangx; angx++) {
            if (((angx ^ angy) & 1) == 1) continue;
            int r = fread(mem + offs(0, 0, 0, angx, angy), 1, FRAMESIZE, fin);
            if (r != FRAMESIZE) printf("Could not read %d bytes (only read %d)\n", FRAMESIZE, r);
        }
    }
    fclose (fin);
    printf ("Interpolating...\n");
    for (angy=0; angy<nangy; angy++) {
        for (angx=0; angx<nangx; angx++) {
            if (((angx ^ angy) & 1) == 0) continue;
            int i;
            for (i=0; i<FRAMESIZE; i++) {
              int pred = 0;
              int np = 0;
              if (angx > 0) { pred += mem[offs(0,0,0,angx-1,angy) + i]; np++; }
              if (angx < nangx-1) { pred += mem[offs(0,0,0,angx+1,angy) + i]; np++; }
              if (angy > 0) { pred += mem[offs(0,0,0,angx,angy-1) + i]; np++; }
              if (angy < nangy-1) { pred += mem[offs(0,0,0,angx,angy+1) + i]; np++; }
              pred /= np;
              mem[offs(0,0,0,angx,angy) + i] = pred;
            }
        }
    }
    printf ("Writing...\n");
    fwrite (mem, nangx * nangy, FRAMESIZE, fout);
    fclose (fout);
}
