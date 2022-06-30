; 13520003 - Dzaky Fattan Rizqullah
; Program Assembly x86 amat sangat sederhana.

global _start

section .data
  msg db "Halo, ini Maya!", 0xa
  len equ $-msg

section .text
_start:
  mov eax, 4 
  mov ebx, 1
  mov ecx, msg
  mov edx, len
  int 0x80
  mov eax, 1
  mov ebx, 0
  int 0x80

; compile (linux): 
; nasm -f elf32 sederhana.asm -o sederhana.o
; ld -m elf_i386 sederhana.o -o sederhana
; ./ex2