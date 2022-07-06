section	.text
  global _start         ;must be declared for using gcc
	
_start:                  ;tell linker entry point
  ; splash
  mov edx, s_len1
  mov ecx, splash1
  mov ebx, 1
  mov eax, 4
  int 0x80

  mov edx, s_len2
  mov ecx, splash2
  mov ebx, 1
  mov eax, 4
  int 0x80

  mov edx, s_len3
  mov ecx, splash3
  mov ebx, 1
  mov eax, 4
  int 0x80

  mov edx, s_len4
  mov ecx, splash4
  mov ebx, 1
  mov eax, 4
  int 0x80

  mov edx, s_len5
  mov ecx, splash5
  mov ebx, 1
  mov eax, 4
  int 0x80

  mov edx, s_len6
  mov ecx, splash6
  mov ebx, 1
  mov eax, 4
  int 0x80
  ; end of splash
  
  ; print intro text
  mov edx, len_intro
  mov ecx, intro
  mov ebx, 1
  mov eax, 4
  int 0x80

bil1:
  ; print step1
  mov edx, len_step1
  mov ecx, step1
  mov ebx, 1
  mov eax, 4
  int 0x80

  ; read and store user input to int_1
  mov edx, 5
  mov ecx, input
  mov ebx, 2
  mov eax, 3
  int 0x80

  mov ecx, 0
  mov ebx, [input]
  mov [int_1], ebx
  mov cl, bl
  cmp cl, 0xa
  je bil2
  sub cl, '0'
  mov esi, ecx
  shr ebx, 8
  mov cl, bl
  cmp cl, 0xa
  je bil2
  sub cl, '0'
  mov eax, esi
  mov dl, 10
  mul dl
  add ax, cx
  shr ebx, 8
  mov cl, bl
  cmp cl, 0xa
  je bil2
  sub cl, '0'
  mov dl, 10
  mul dx
  add eax, ecx
  shr ebx, 8
  cmp bl, 0xa
  je bil2
  sub bl, '0'
  mov dl, 10
  mul dx
  add eax, ecx

bil2:
  push eax
  ; print step2
  mov edx, len_step2
  mov ecx, step2
  mov ebx, 1
  mov eax, 4
  int 0x80

  ; read and store user input to int_1
  mov edx, 5
  mov ecx, input
  mov ebx, 2
  mov eax, 3
  int 0x80

  mov ecx, 0
  mov ebx, [input]
  mov [int_2], ebx
  mov cl, bl
  cmp cl, 0xa
  je result
  sub cl, '0'
  mov esi, ecx
  shr ebx, 8
  mov cl, bl
  cmp cl, 0xa
  je result
  sub cl, '0'
  mov eax, esi
  mov dl, 10
  mul dl
  add ax, cx
  shr ebx, 8
  mov cl, bl
  cmp cl, 0xa
  je result
  sub cl, '0'
  mov dl, 10
  mul dx
  add eax, ecx
  shr ebx, 8
  cmp bl, 0xa
  je result
  sub bl, '0'
  mov dl, 10
  mul dx
  add eax, ecx

result:
  push eax
  ; print hasil
  mov edx, len_hasil
  mov ecx, hasil
  mov ebx, 1
  mov eax, 4
  int 0x80

  pop edx
  pop eax
  cmp eax, edx
  jl swap

gcd:
  cmp edx, 0
  je _exit
  mov ebx, edx
  xor edx, edx
  div ebx
  mov eax, ebx
  jmp gcd

swap:
  push esi
  mov esi, eax
  mov eax, edx
  mov edx, esi
  pop esi
  jmp gcd

error:
  mov edx, len_err_msg
  mov ecx, err_msg
  mov ebx, 1
  mov eax, 4
  int 0x80

printNumber:
    push eax
    push edx
    xor edx,edx
    mov ebx, 10
    div ebx
    test eax,eax
    je .l1
    call printNumber
.l1:
    lea eax,[edx+'0']
    mov [input], eax
    mov edx, 4
    mov ecx, input
    mov ebx, 1
    mov eax, 4
    int 0x80
    pop edx
    pop eax
    ret

_exit:
  call printNumber
  mov eax, 0xa
  mov [input], eax
  mov edx, 4
  mov ecx, input
  mov ebx, 1
  mov eax, 4
  int 0x80
  mov	eax,1             ;system call number (sys_exit)
  int	0x80              ;call kernel

section	.data
splash1 db '    __  ___ __                    __________  ____ ', 0xa
s_len1 equ $-splash1
splash2 db '   / / / (_) /___  ______  ____ _/ ____/ __ \/ __ )', 0xa
s_len2 equ $-splash2
splash3 db '  / /_/ / / __/ / / / __ \/ __ `/ /_  / /_/ / __  |', 0xa
s_len3 equ $-splash3
splash4 db ' / __  / / /_/ /_/ / / / / /_/ / __/ / ____/ /_/ / ', 0xa
s_len4 equ $-splash4
splash5 db '/_/ /_/_/\__/\__,_/_/ /_/\__, /_/   /_/   /_____/  ', 0xa
s_len5 equ $-splash5
splash6 db '                        /____/                     ', 0xa
s_len6 equ $-splash6
                  
intro db 0xa, 'Program untuk menghitung FPB dua integer 4 digit positif.', 0xa, 'Perhatian! Tidak ada error-handling pada masukan! :(', 0xa
len_intro equ $-intro

step1 db 'Masukkan integer positif pertama (1 - 9999): '
len_step1 equ $-step1
step2 db 'Masukkan integer positif kedua (1 - 9999): '
len_step2 equ $-step2

hasil db 'FPB-nya adalah: '
len_hasil equ $-hasil

err_msg db 'Terjadi kesalahan.', 0xa
len_err_msg equ $-err_msg

section .bss
input resb 5
int_1 resb 4
int_2 resb 4