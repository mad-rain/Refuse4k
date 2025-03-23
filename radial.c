
#define RADIAL_INT 8

int RADIAL_T1[160][RADIAL_INT];
int RADIAL_T2[160][RADIAL_INT];

void 
radial_do()
{
        int *tb2 = &RADIAL_T2[0][0];
        unsigned char *src = video_buffer + 32160;
        unsigned char *src0;
        unsigned char *src1;
        unsigned char *src2;
        unsigned char *src3;
        int f = 100;

        src0 = src1 = src2 = src3 = src;

        do {
                int g = 160;
                int *tb1 = &RADIAL_T1[0][0];
    
                do {
                        int t = RADIAL_INT;
                        int c0, c1, c2, c3;
                        c0 = c1 = c2 = c3 = 0;
                                
                        do {
                                c0 += src[  *tb1 + *tb2];
                                c1 += src[- *tb1 + *tb2];
                                c2 += src[- *tb1 - *tb2];
                                c3 += src[  *tb1 - *tb2];
                                tb1 ++, tb2 ++;
                        } while (--t);

                        *src0 ++ = (c0 / RADIAL_INT);
                        *src1    = (c1 / RADIAL_INT);
                         src1 --;
                        *src2    = (c2 / RADIAL_INT);
                         src2 --;
                        *src3 ++ = (c3 / RADIAL_INT);
                
                        tb2 -= RADIAL_INT;
                } while (--g);
                *src1 = 0;
                *src2 = 0;
                src0 += 160;
                src1 += 160+320;
                src2 -= 160;
                src3 -= 160+320;
                tb2 += RADIAL_INT;
        } while(--f);
}

void 
radial_init(float inten)
{
        int f;
        int *dest1 = &RADIAL_T1[0][0];
        int *dest2 = &RADIAL_T2[0][0];

        for (f=0;f<160;f++) {
                int t = RADIAL_INT;
                float fq = 1.0f;
                do {
                        int q = (fq*f);
                        fq *= inten;
                        *dest1 ++ = q;
                        *dest2 ++ = q * 320;
                } while (--t);
        }
}
