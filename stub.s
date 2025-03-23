
MSIZE   equ 1024*1024

        SECTION .text use16

        org 100h

        mov ax,1130h
        mov bh,6
        xor ebp,ebp
        int 10h

        mov ax,es
        shl eax,4
        add eax,ebp
        mov [font-4],eax

        mov ax,1687h
        mov cx,ax
        int 2Fh
        sub ax,cx
        jz noDPMI
        push es
        push di
        mov ax,cs
        add ah,10h
        mov es,ax
        mov ax,1
        ; call dword [esp]
        
        db 67h, 0ffh, 1ch, 24h


        pop ebx
        jb perror
        mov ax,501h
        mov bx,MSIZE/65536
        xor cx,cx
        int 31h
        jb perror
                        ;bx:cx
        mov [cbase-2],bx
        mov [cbase-4],cx

        mov al,0CFh
        mov ah,bh
        push ax
        mov ah,0FAh
        mov al,bl
        push ax
        push cx
        mov cx,-1
        push cx
        neg cx
        xor ax,ax
        int 31h
        mov bx,ax
        mov ax,0Ch
        mov edi,esp
        int 31h
        push bx
        mov al,0Ah
        int 31h
        mov es,ax
        xor edi,edi
        push di
        mov si,pmode_data
        mov cx,4000h
        rep movsb
        mov ds,ax
        mov esi,0BEBEBEBEh
cbase
        mov ebx,0BBBBBBBBh
font
        retf

noDPMI
        mov ah,9
        mov dx,message_noDPMI
        int 21h
perror
        mov ax,4CFFh
        int 21h

message_noDPMI          db 'noDPMI',13,10,'$'

pmode_data:


