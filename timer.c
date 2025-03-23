
#ifdef DPMI

int current_time;
extern int dseg;

void timer_proc();

void setup_timer_proc();
#pragma aux setup_timer_proc = "mov word ptr [dseg],ds     "\
                               "mov cx,cs                  "\
                               "mov edx,offset timer_proc  "\
                               "mov bl,8                   "\
                               "mov ax,205h                "\
                               "int 31h                    " modify[eax ebx ecx];

void set_timer_speed(int);
#pragma aux set_timer_speed parm[ebx] modify[eax]

#endif /* DPMI */
