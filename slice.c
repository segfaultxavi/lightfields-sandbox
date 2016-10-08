#include <stdio.h>
#include <stdlib.h>

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

    int angx, angy;
    for (angy=0; angy<nangy; angy++) {
        for (angx=0; angx<nangx; angx++) {
            char fname[256];
            sprintf(fname, argv[7], angx+nangx*angy);
            f = fopen (fname, "wb");
            int c, x, y;
            for (y=0; y<nsizey; y++) {
                for (x=0; x<nsizex; x++) {
                    for (c=0; c<3; c++) {
                        unsigned char b = DATA(c,x,y,angx,angy) >> 8;
                        fwrite(&b, 1, 1, f);
                    }
                }
                // Hack for GStreamer's ROUNDUP_2
                fwrite(&c, 1, 1, f);
            }
            fclose(f);
        }
    }
}
