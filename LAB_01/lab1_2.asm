.model small
.stack 100h

.data

Error_Write_Message db "Write error!",0Dh,0Ah,'$'
Error_Read_Message db "Read error!",0Dh,0Ah,'$'
           
.code

jmp start

Init_COM1 proc
   xor ax,ax
   mov dx,0         ; func num (COM init)
   mov al,10100011b ; port params
   int 14h
   ret            
Init_COM1 endp

Write_COM1 proc
   mov al,'A'  ; symbol
   mov ah,1    ; write func
   mov dx,0    ; port num
   int 14h     ; COM interrupt
   test al,80h
   jnz NoWRite
   ret 
Write_COM1 endp

NoWRite proc
   mov ah,9
   mov dx,offset Error_Write_Message
   add dx,2
   int 21h
   ret 
NoWRite endp

Read_COM2 proc
    mov ah,2    ; func num (read with wait)
    mov dx,1    ; port num (COM2)
    int 14h     ; COM interrupt
    test al,80h ; 1000 0000 if no time-out
    jnz NoRead
    ret
Read_COM2 endp

NoRead proc
   mov ah,9
   mov dx,offset Error_Read_Message
   add dx,2
   int 21h
   ret 
NoRead endp

Output proc
   mov ah,02h  ; print func
   mov dl,al
   int 21h
   ret
Output endp

Exit proc
    mov ax,4C00h
    int 21h
    ret
Exit endp

start:
   call Init_COM1
   call Write_COM1
   call Read_COM2

   call Output
   call Exit

end start