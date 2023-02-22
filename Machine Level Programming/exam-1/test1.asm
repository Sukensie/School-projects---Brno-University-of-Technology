%include "rw32-2018.inc"

section .data
    ; zde budou vase data

    a db 8
    b dd 20
    c dw 16
    d dw 3   
    e dw 5
    
    
    
section .text
_main:
    push ebp
    mov ebp, esp
    
    
    
    
    
    
    
    ; zde bude vas kod
    ;TASK 2 --- 
    
    ;q = (a + b*c + 25)/(10*d + e + 115)
    ;(a*b+c+35)/(d*10+e+125)
    ;UNSIGNED
    
    xor eax,eax
    mov eax, [b]
    
    xor ebx,ebx
    mov bx, [c]
    mul ebx;eax = b*c
    mov ecx, eax ;z�loha EAX
    
    xor eax,eax
    mov al, [a]
    add eax, ecx
    
    add eax, 25 ;v eax je v�sledek prvn� z�vorky
    
    mov ebx, eax ;EBX...z�loha prvn� z�vorky
    
    xor eax, eax
    mov ax, [d]
    
    xor ecx,ecx
    mov cl, 10

    mul ecx ;eax = 10*d (40)
    
    xor ecx,ecx
    mov cx, [e]

    add eax, ecx ;eax = 10*d + e (45)
    
    xor ecx,ecx
    mov cl, 115

    add eax, ecx ; EAX... druh� z�vorka (160)
    
    ;prohozen� ebx za eax
    xor ecx, ecx
    mov ecx, eax
    mov eax, ebx
    
    ;mov edx, 0
    div ecx
    
    ;ulo�en� v�sledk�
    mov [q], eax
    mov [r], edx
  
    
    
    
    
    
    
    mov ax, [c]
    cwde 
    mov ecx, eax
    sub ecx, 38;v�sledek c-38
    
    add ebx, ecx ;v�sledek 1. z�vorky v ebx
    
    
    xor eax, eax
    mov ax, [d]
    cwde
    mov ecx, 13
    mul ecx; eax = 13*d
    
    xor ecx, ecx
    mov ecx,eax;ecx = 13*d
    
    ;nahr�t do cx a ud�lat p�esun na ecx (pomoc� eax)
    xor eax,eax
    mov ax, [e]
    cwde
    add eax, ecx;eax = 13*d + e
    
    sub eax, 108;v eax cel� 2. z�vorka
    
    ;ebx...1 zavorka | eax...2 zavorka
    xor ecx, ecx
    mov ecx, eax
    mov eax, ebx
    
   ; mov eax, -100
   ; mov ecx, 20
    
    ;eax... 1 zavorka | ecx...2 zavorka
    
    div ecx
    
    
    
    
    
    
    ; r =(a*b + c -38) % (13*d + e - 108)
    
    xor eax,eax
    mov al, [a]
    cbw
    cwde
    mov ebx, [b]
    mul ebx
    
    mov ebx, eax ;z�loha v�sledku a*b
    
    xor eax, eax
    mov ax, [c]
    cwde 
    mov ecx, eax
    sub ecx, 38;v�sledek c-38
    
    add ebx, ecx ;v�sledek 1. z�vorky v ebx
    
    
    xor eax, eax
    mov ax, [d]
    cwde
    mov ecx, 13
    mul ecx; eax = 13*d
    
    xor ecx, ecx
    mov ecx,eax;ecx = 13*d
    
    ;nahr�t do cx a ud�lat p�esun na ecx (pomoc� eax)
    xor eax,eax
    mov ax, [e]
    cwde
    add eax, ecx;eax = 13*d + e
    
    sub eax, 108;v eax cel� 2. z�vorka
    
    ;ebx...1 zavorka | eax...2 zavorka
    xor ecx, ecx
    mov ecx, eax
    mov eax, ebx
    
   ; mov eax, -100
   ; mov ecx, 20
    
    ;eax... 1 zavorka | ecx...2 zavorka
    
    div ecx
    
    call WriteInt32
    
   
    
    
    
    
    
    ;TASK 1
    mov eax, 0xDDCCBBAA
    xchg ah, al
    ror eax, 8
    xchg ah, al
    xor eax,eax

    pop ebp
    ret