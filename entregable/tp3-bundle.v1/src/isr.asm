; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

;atencion de interrupciones
extern atencion_teclado
extern resolver_excepcion

extern mover_zombi

global _isr32
global _isr33
global _isr102
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    push eax
    mov eax, [esp + 12]  ; eip   o es correcto desplazarme 12 o en verdad es 8
    push eax
    mov eax, [esp + 20] ; eflags
    push eax
    xor eax, eax
    mov ax, ss
    push eax
    mov ax, gs
    push eax
    mov ax, fs
    push eax
    mov ax, es
    push eax
    mov ax, ds
    push eax
    mov ax, cs ; cs
    push eax
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax
    push esp
    mov eax, [ebp]              ;pusheo los valores de la pila
    push eax
    mov eax, [ebp+4]            ;si es que estoy leyendo en el lugar adecuado
    push eax
    mov eax, [ebp+8]
    push eax
    mov eax, [ebp+12]
    push eax
    mov eax, [ebp+16]
    push eax
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    mov eax, %1
    push eax
    call resolver_excepcion
    ;jmp $
    jmp 0x70:0 

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18  
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
;;
_isr32:  
   pushad
   
    ;call fin_intr_pic1

    call proximo_reloj

    call sched_proximo_indice             ;por ahora devuelve 0 para que no devuelva el selector de una tarea que no existe
    cmp ax, 0                           ;recordar si la tarea es la misma deberia devolver 0.  ej: en el juego solo esta el
  
    je .no_jump                         ;de un solo jugador
        ;xchg bx, bx
        mov [sched_tarea_selector], ax
        call fin_intr_pic1
        jmp far [sched_tarea_offset]
        jmp .sched_end
    
    .no_jump:  
    call fin_intr_pic1  
    
    .sched_end:
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
_isr33:
    pushad
    call fin_intr_pic1
    
    xor eax, eax
    in al, 0x60

    push eax
    call atencion_teclado
    add esp,4
    popad
    iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
   _isr102:
    pushad
    cmp eax, ADE
    je .movement
    cmp eax, DER
    je .movement
    cmp eax, IZQ
    je .movement
    cmp eax, ATR
    jne .no_move
.movement:    
    push eax
    mov eax, cr3
    push eax
    call mover_zombi
    add esp, 8
.no_move:
    jmp 0x70:0    ;salto a la idle
    popad
    iret 




;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
        
