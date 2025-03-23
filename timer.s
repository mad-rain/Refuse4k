
        BITS 32

        SECTION .code use32
        
        GLOBAL timer_proc_
        GLOBAL set_timer_speed_

        EXTERN _current_time
        GLOBAL _dseg

set_timer_speed_:
        mov al,34h
        out 43h,al
        mov al,bl
        out 40h,al
        mov al,bh
        out 40h,al
        ret

timer_proc_:

        push eax
        push ds
        mov ax,1234h
_dseg   equ $-2
        mov ds,ax

        inc dword [_current_time]

        pop ds
        mov al,20h
        out 20h,al
        pop eax
        iretd
