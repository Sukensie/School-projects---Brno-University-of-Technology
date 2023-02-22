%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
_task31:
    push ebp
    mov ebp, esp
    
    ;cos(y+(pi*x))
    fld dword [ebp+12] ; x |
    fldpi ; pi | x
    
    fmulp ; pi * x |
    
    fld dword [ebp+8] ; y | pi * x 
    
    faddp ; y + (pi * x)
    
    fcos ; cos(y+(pi*x))
    
    
    fld dword [ebp+8] ; y | cos(y+(pi*x))
    
    push __float32__ (7.750)
    fld dword [esp] ; 3.150 | y | cos(y+(pi*x))
    add esp, 4 ;uklizení push
    
    faddp ; 3.150 + y | cos(y+(pi*x))
    
    fsqrt ; sqrt(3.150 +y) | cos(y+(pi*x))
    
    faddp ; sqrt(3.150 +y) + cos(y+(pi*x))
    
    
    ; abs(y+(x/8))
    fld dword [ebp+12] ; x | sqrt(3.150 +y) + cos(y+(pi*x))
    
    push 2
    fild dword [esp] ; 8 | x | sqrt(3.150 +y) + cos(y+(pi*x))
    add esp, 4 ;uklizení push
    
    fdivp ; x / 8 | sqrt(3.150 +y) + cos(y+(pi*x))
    
    
   fld dword [ebp+8] ; y | x / 8 | sqrt(3.150 +y) + cos(y+(pi*x)))
   faddp ; y +(x / 8) | sqrt(3.150 +y) + cos(y+(pi*x)))
    
   fabs; abs(y +(x / 8)) | sqrt(3.150 +y) + cos(y+(pi*x)))
   
   ftst
   je fail
   
    fdivp
    
    jmp konec
    
    fail: 
    push 0xffffffff
    fld dword [esp]
    add esp, 4
    
    konec:
    pop ebp
    ret
       
     
    
    
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    push __float32__ (2.0) ; poslání hodnoty x 
    push __float32__ (3.0) ; poslání hodnoty y 
    
    
    call _task31
    add esp, 8

    pop ebp
    ret