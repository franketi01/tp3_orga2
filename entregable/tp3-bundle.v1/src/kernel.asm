; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

; la GDT
extern GDT_DESC

extern print_black_screen
extern print_game_screen

extern idt_inicializar
extern mmu_inicializar_dir_kernel

extern IDT_DESC

extern resetear_pic
extern habilitar_pic

extern inicializar_idle

extern iniciar_sched

extern mapear_pagina

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli
    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    
    

    ; Habilitar A20
    call habilitar_A20
  
    ; Cargar la GDT

    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0

    mov eax, CR0
    or eax, 1
    mov CR0, eax
   
    ; Saltar a modo protegido
    jmp 0x40:modo_protegido

BITS 32

modo_protegido:
    xor eax, eax
    ; Establecer selectores de segmentos 00001001000
    mov ax, 0x48
    mov ss, ax 
   ; jmp $
    mov ds, ax
    mov gs, ax
    mov fs, ax
    mov ax, 0x060
    mov es, ax
;0000 1100 000 
    ; Establecer la base de la pila

    mov esp, 0x27000	
    
    ; Imprimir mensaje de bienvenida
    
   
    ; Inicializar pantalla
    call print_black_screen
    call print_game_screen
    
    ; Inicializar el manejador de memoria
    call mmu_inicializar_dir_kernel

    ; Inicializar el directorio de paginas
    
    
    ; Cargar directorio de paginas
    mov eax, 0x27000
    mov CR3, eax
    

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax


    ; Inicializar tss

    ; Inicializar tss de la tarea Idle
    call inicializar_idle

    ; Inicializar el scheduler

    call iniciar_sched

    ; Inicializar la IDT

    call idt_inicializar
    
    ; Cargar IDT

    lidt[IDT_DESC]
 	 
    ; Configurar controlador de interrupciones
    call resetear_pic
   ; xchg bx, bx
    call habilitar_pic
    sti
    ; Cargar tarea inicial
    mov ax, 0x68
    ltr ax
    jmp 0x70:0

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
