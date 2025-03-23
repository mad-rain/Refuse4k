start:
        cld
        mov ax,ds
        mov ss,ax
        mov esp,offset _stack + 0x1000 * 4

        fninit
        mov eax,0a0000h
        sub eax,esi
        sub ebx,esi
        mov dword ptr FLAT:_font,ebx
        mov dword ptr FLAT:_VideoMem,eax

        mov word ptr [esp],1f7fh
        fldcw [esp]     

        include main_cr.inc

        mov ax,4c00h
        int 21h

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

        inc dword ptr FLAT:_current_time

        pop ds
        mov al,20h
        out 20h,al
        pop eax
        iretd
