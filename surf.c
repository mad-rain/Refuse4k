#define SURF_GSIZE_LOG 2
#define SURF_GSIZE (1 << SURF_GSIZE_LOG)

#define SURF_SIZE_Y ((200 >> SURF_GSIZE_LOG) + 1)
#define SURF_SIZE_X ((320 >> SURF_GSIZE_LOG) + 1)

int SURF_grid[SURF_SIZE_Y][SURF_SIZE_X][2];
int *surf_dest = (int *)SURF_grid;

void 
compute_surf(void (*sproc)(float, float), unsigned char *texture)
{

#define SURF_STEP_Y (float)(1.0 / 200.0 * 240.0 * SURF_GSIZE / 8.0f)
#define SURF_STEP_X (SURF_GSIZE / 8.0f)

        unsigned char *dest = video_buffer;
        float y = -SURF_STEP_Y * (SURF_SIZE_Y / 2);
        int i = SURF_SIZE_Y;
        surf_dest = (int *)SURF_grid;
        do {
                float x = -(SURF_SIZE_X / 2) * SURF_STEP_X;
                int j = SURF_SIZE_X;
                do {
                        sproc(y, x);
                        surf_dest += 2;
                        x += SURF_STEP_X;
                } while (--j);
                y += SURF_STEP_Y;
        } while (--i);

 {
        int y = SURF_SIZE_Y - 1;
        surf_dest = (int *)SURF_grid;
        do {
                int x = SURF_SIZE_X - 1;
                do {
                        int txstepy0, tystepy0;
                        int txstepy1, tystepy1;
                        int tx0, ty0;
                        int tx1, ty1;
                        int f = SURF_GSIZE;

                        tx0 = surf_dest[0];
                        ty0 = surf_dest[1];
                        tx1 = surf_dest[2];
                        ty1 = surf_dest[3];
        
                        txstepy0 = (surf_dest[SURF_SIZE_X * 2    ] - tx0) >> SURF_GSIZE_LOG;
                        tystepy0 = (surf_dest[SURF_SIZE_X * 2 + 1] - ty0) >> SURF_GSIZE_LOG;
                        txstepy1 = (surf_dest[SURF_SIZE_X * 2 + 2] - tx1) >> SURF_GSIZE_LOG;
                        tystepy1 = (surf_dest[SURF_SIZE_X * 2 + 3] - ty1) >> SURF_GSIZE_LOG;

                        do {
                                int tx,ty;
                                int txstepx,tystepx;
                                int g = SURF_GSIZE;

                                tx = tx0, ty = ty0;

                                tystepx = ty1-ty0;
                                if (tystepx >= 0x800000) tystepx -= 0x1000000;
                                tystepx >>= SURF_GSIZE_LOG;
                                txstepx = (tx1 - tx0) >> SURF_GSIZE_LOG;

                                tx0 += txstepy0;
                                ty0 += tystepy0;
                                tx1 += txstepy1;
                                ty1 += tystepy1;

                            do {
                                *dest ++ = texture[((ty >> 8)&0xFF00) 
                                               + ((tx >> 16)&0xFF)];
                                tx += txstepx;
                                ty += tystepx;
                            } while (--g);

                                dest += 320 - SURF_GSIZE;
                        } while(--f);

                        dest -= 320 * SURF_GSIZE - SURF_GSIZE;
                        surf_dest += 2;
                } while (--x);

                dest += 320*(SURF_GSIZE-1);
                surf_dest += 2;
        } while (--y);
 }
}

