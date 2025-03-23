
char fnm_str[3] = {'M', 'N', 'F'};      /* crypted :) */

unsigned char texture[0x100][0x100];
unsigned char texture2[0x100][0x100];
unsigned char texture3[0x100][0x100];
unsigned char texture4[0x100][0x100];

void 
plasma_interp(int c1, int c2, int d, int x, int y)
{
        int c;
        unsigned char *q = &texture[y][x];
        
        if (*q) return;
        c = ((c1 + c2) >> 1) + ((((mrand() & 0xff) - 0x80) * d) >> 5);
        if (c < 1) c = 1; else if (c > 0xff) c = 0xff;
        *q = c;
}

void 
plasma_subdiv(int x1, int y1, int x2, int y2)
{
        int xc, yc, c1, c2, c3, c4, y2n, x2n;
        int dx, dy;

        if (x2 - x1 < 2) return;

        y2n = y2 & 0xFF;
        x2n = x2 & 0xFF;

        xc = x2 + x1;
        xc >>= 1;
        yc = y2 + y1;
        yc >>= 1;

        c1 = texture[y1][x1];
        c2 = texture[y1][x2n];
        c4 = texture[y2n][x2n];
        c3 = texture[y2n][x1];

        dx = x2 - x1;

        plasma_interp(c1, c2, dx, xc, y1);
        plasma_interp(c3, c4, dx, xc, y2n);

        dy = y2 - y1;

        plasma_interp(c1, c3, dy, x1, yc);
        plasma_interp(c2, c4, dy, x2n, yc);

        texture[yc][xc] = (c1 + c2 + c3 + c4) / 4;

        plasma_subdiv(x1, y1, xc, yc);
        plasma_subdiv(xc, yc, x2, y2);
        plasma_subdiv(x1, yc, xc, y2);
        plasma_subdiv(xc, y1, x2, yc);
}
