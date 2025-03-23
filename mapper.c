
#define SCR_RESX 320
#define SCR_RESY 200

void
affine_mapper(int x1, int y1, int tx1, int ty1,
              int x2, int y2, int tx2, int ty2,
              int x3, int y3, int tx3, int ty3)
{
        register int temp;
        int real_y;
        int dx1, dy1, dtx1, dty1;
        int dx2, dy2, dtx2, dty2;
        int dx3, dy3, dtx3, dty3;
        register unsigned char *dest2;
        register int stx, sty;
        register int t_tx1, t_ty1;
        int t_x1;
        int t_x2, t_tx2, t_ty2;
        int count;

        if (y1 > y2) {
                temp =  y1,  y1 =  y2,  y2 = temp;
                temp =  x1,  x1 =  x2,  x2 = temp;
                temp = tx1, tx1 = tx2, tx2 = temp;
                temp = ty1, ty1 = ty2, ty2 = temp;
        }

        if (y1 > y3) {
                temp =  y1,  y1 =  y3,  y3 = temp;
                temp =  x1,  x1 =  x3,  x3 = temp;
                temp = tx1, tx1 = tx3, tx3 = temp;
                temp = ty1, ty1 = ty3, ty3 = temp;
        }

        if (y2 > y3) {
                temp =  y2,  y2 =  y3,  y3 = temp;
                temp =  x2,  x2 =  x3,  x3 = temp;
                temp = tx2, tx2 = tx3, tx3 = temp;
                temp = ty2, ty2 = ty3, ty3 = temp;
        }

        if (y3 < 0 || y1 >= SCR_RESY || y1 == y3) return;

        if (y1 != y2) {
                temp = y2 - y1;
                dx1 = ( x2 -  x1) / temp;
                dy1 = ( y2 -  y1) / temp;
                dtx1 = (tx2 - tx1) / temp;
                dty1 = (ty2 - ty1) / temp;
        }

        temp = y3 - y1;
        dx2 = ( x3 -  x1) / temp;
        dy2 = ( y3 -  y1) / temp;
        dtx2 = (tx3 - tx1) / temp;
        dty2 = (ty3 - ty1) / temp;

        if (y2 != y3) {
                temp = y3 - y2;
                 dx3 = ( x3 -  x2) / temp;
                 dy3 = ( y3 -  y2) / temp;
                dtx3 = (tx3 - tx2) / temp;
                dty3 = (ty3 - ty2) / temp;
        } else {
                dx3 = dy3 = dtx3 = dty3 = 0;
        }

        real_y = y1;
        if (real_y < 0) real_y = 0;

        do {
                if (real_y < y2) {
                        temp = real_y - y1;
                        t_x1 = (dx1 * temp + x1) >> 16;
                        t_tx1 = dtx1 * temp + tx1;
                        t_ty1 = dty1 * temp + ty1;
                }  else {
                        temp = real_y - y2;
                        t_x1 = (dx3 * temp+x2 ) >> 16;
                        t_tx1 = dtx3 * temp + tx2;
                        t_ty1 = dty3 * temp + ty2;
                }

                temp = real_y - y1;

                t_x2 = (dx2 * temp + x1) >> 16;
                t_tx2 = dtx2 * temp + tx1;
                t_ty2 = dty2 * temp + ty1;

                if (t_x2 < t_x1) {
                        temp =  t_x1,  t_x1 =  t_x2,  t_x2 = temp;
                        temp = t_tx1, t_tx1 = t_tx2, t_tx2 = temp;
                        temp = t_ty1, t_ty1 = t_ty2, t_ty2 = temp;
                }

                temp = t_x2 - t_x1;

                if (t_x2 >= 0 && t_x1 < SCR_RESX && temp) {
                        temp = t_x2 - t_x1;

                        stx = (t_tx2 - t_tx1) / temp;
                        sty = (t_ty2 - t_ty1) / temp;

                        dest2 = video_buffer + real_y * SCR_RESX;

                        if (t_x1 >= 0) dest2 += t_x1; else
                                do {
                                        t_tx1 += stx;
                                        t_ty1 += sty;
                                } while (++t_x1);

                        temp = t_x2;
                        if (temp >= SCR_RESX) temp = SCR_RESX - 1;
                        count = temp - t_x1;
                        if (count)
                                do {
                                        *dest2 ++ = texture[(t_tx1 >> 16) & 0xFF]
                                                           [(t_ty1 >> 16) & 0xFF];
                                        t_tx1 += stx;
                                        t_ty1 += sty;
                                } while (--count);
                }

                real_y ++;
        } while(real_y <= y3 && real_y < SCR_RESY);
}


