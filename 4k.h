
unsigned char video_buffer[64000];

int pal[0x300];

unsigned int pal_bright;

#ifdef DPMI

unsigned char *VideoMem;

void set_video_mode(int);
#pragma aux set_video_mode = " int 10h " parm[ax];

void m_getch();
#pragma aux m_getch = " xor ah,ah "\
                      " int 16h   " modify[ah];

int m_kbhit();
#pragma aux m_kbhit = " mov ah,1 "\
                     " xor ebx,ebx "\
                     " int 16h  "\
                     " jz @nopr "\
                     " inc ebx  "\
                     " @nopr:    " modify[eax ebx] value[ebx];

void update_video();
#pragma aux update_video = " mov dx,03dah                "\
                          "@e0: in al,dx                "\
                          "  test al,8                  "\
                          " jnz @e0                     "\
                          "@e1: in al,dx                "\
                          "  test al,8                  "\
                          " jz @e1                      "\
                          " mov edx,3c8h                "\
                          " xor al,al                   "\
                          " out dx,al                   "\
                          " mov esi,offset pal          "\
                          " mov ebx,dword ptr ds:[pal_bright] "\
                          " mov ecx,768                 "\
                          "@e3:                         "\
                          " lodsd                       "\
                          " mul ebx                     "\
                          " mov al,ah                   "\
                          " mov edx,03c9h               "\
                          " out dx,al                   "\
                          " loop @e3                    "\
                          " mov esi,offset video_buffer + 320 " \
                          " mov edi,dword ptr VideoMem  " \
                          " mov ecx,16000 - 80            " \
                          " rep movsd                   " modify[esi edi ecx dx al];

void memset(void *, int, int);
#pragma aux memset = " rep stosb " parm[edi][eax][ecx] modify[edi ecx];

void memset4(void *, int, int);
#pragma aux memset4 = " rep stosd " parm[edi][eax][ecx] modify[edi ecx];

void memcpy(void *, void *, int);
#pragma aux memcpy = " rep movsb " parm[edi][esi][ecx] modify[edi esi ecx];

void memcpy4(void *, void *, int);
#pragma aux memcpy4 = " rep movsd " parm[edi][esi][ecx] modify[edi esi ecx];

void outp(int, int);
#pragma aux outp = " out dx,al " parm nomemory [edx][eax] modify exact nomemory[];

#endif /* DPMI */


int rand_r1 = 0x327E283;
int rand_r2 = 0x926C2A3;
int rand_r3 = 0x43F4B29;

int mrand()
{
        rand_r1 += rand_r2;
        rand_r2 += rand_r3;
        rand_r3 += rand_r1;
        return(rand_r3);
}
