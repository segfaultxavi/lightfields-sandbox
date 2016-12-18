#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OFFS(c,x,y,ax,ay) ((ay) * (nangx * nsizey * (nsizex * ncom + 1)) + (ax) * (nsizey * (nsizex * ncom + 1)) + (y) * (nsizex * ncom + 1) + (x) * (ncom) + (c))

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "rb");
    int ncom = atoi(argv[2]);
    int nsizex = atoi(argv[3]);
    int nsizey = atoi(argv[4]);
    int nangx = atoi(argv[5]);
    int nangy = atoi(argv[6]);
    int size = ncom*nsizex*nsizey*nangx*nangy;
    unsigned char *mem = malloc(size);
    int r = fread(mem, 1, size, f);
    printf ("Read %d bytes, needed %d\n", r, size);
    fclose(f);

    int angx, angy;
    double error = 0;
    int nsamples = 0;
    for (angy=0; angy<nangy; angy++) {
        for (angx=0; angx<nangx; angx++) {
            if (((angx ^ angy) & 1) == 0) continue;
            int c, x, y;
            char fname[256];
            sprintf (fname, "out%x%x.raw", angx, angy);
            FILE *f = fopen (fname, "wb");
            for (y=0; y<nsizey; y++) {
                for (x=0; x<nsizex; x++) {
                    for (c=0; c<ncom; c++) {
                        int pred = 0;
                        int np = 0;
                        if (angx > 0) { pred += mem[OFFS(c,x,y,angx-1,angy)]; np++; }
                        if (angx < nangx-1) { pred += mem[OFFS(c,x,y,angx+1,angy)]; np++; }
                        if (angy > 0) { pred += mem[OFFS(c,x,y,angx,angy-1)]; np++; }
                        if (angy < nangy-1) { pred += mem[OFFS(c,x,y,angx,angy+1)]; np++; }
                        pred /= np;
                        int val = mem[OFFS(c,x,y,angx,angy)];
                        error += (val - pred) * (val - pred);
                        nsamples++;
                        char pred_b = pred;
                        fwrite (&pred_b, 1, 1, f);
                    }
                }
            }
            fclose (f);
        }
    }
    error /= nsamples;
    printf ("PREDICTION PSNR = %g\n", 10 * logf (255 * 255 / error));
}
