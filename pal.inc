{
        int *dest = pal;
        int i = 0;
        int z = 0x80;

        do {
                dest[0] = r & i;
                dest[1] = g & i;
                dest[2] = b & i;
                i ++;
                dest += 3;
        } while (--z);

        z = 0x180;
        do {
                if (dest[-3] + 1 < 0x7f) dest[0] = dest[-3] + 1;
                                    else dest[0] = 0x7f;
                dest ++;
        } while (--z);
}
