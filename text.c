
void 
print_text(unsigned char *dest, char *text)
{
        int c;

        dest += 8 * 2;
        
        while (c = (int)(*text ++)) {
                unsigned char *addr = font + c * 16;
                int y = 16;
                do {            
                        unsigned char b = *addr ++;
                        int x = 8;
                        do {
                                unsigned char c = - (b & 1);
                                b >>= 1;
                                dest -= 2;
                                dest[0] |= c;
                                dest[1] |= c;
                                dest[320] |= c;
                                dest[321] |= c;
                        } while (--x);
                        dest += 320 * 2 + 2 * 8;
                } while (--y);
                dest += - 320 * 16 * 2 + 2 * 8;
        }
}
