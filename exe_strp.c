
#include <stdio.h>

char buf[16384];

int main()
{
        int size;
        FILE *fp;

        fp = fopen("main_cr.bin", "rb");
        if (!fp) {
                printf("can't open 'main_cr.bin'\n");
                return 0;
        }
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0x40, SEEK_SET);
        fread(buf, 1, size - 0x40, fp);
        fclose(fp);

        fp = fopen("main_cr.raw", "wb");
        fwrite(buf, 1, size - 0x40, fp); 
        fclose(fp);

        return(0);
}