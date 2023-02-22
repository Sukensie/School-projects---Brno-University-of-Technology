%include "rw32-2018.inc"

section .data
    ; zde budou vase data
        arr1 dd 1,2,3

section .text
task23:
    CEXTERN malloc
    push ebp
    mov ebp, esp
    
    cmp ecx, 0
    jle .skip
    
    push edx ;malloc m�n� edx
    push ecx ;p�edej ecx mallocu
    call malloc
    add esp, 4 ;anti segfault
    
    ;v eax je ukazatel na alokovanou pam�

    .skip:
        mov eax, 0
        
 
    pop ecx
    pop edx

    pop ebp
    ret
    
_main:
    
    call _task23       
    
    pop ebp
    ret