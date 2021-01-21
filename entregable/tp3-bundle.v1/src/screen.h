/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void print_black_screen();

void print_game_screen();

void print_debug(char c, int jug ,int eax, int ebx, int ecx, int edx,int st5,int st4,int st3,int st2,int st1,int esi, int edi, int ebp, int esp, int eip, short cs, short ds, short es, short fs, short gs, short ss, int flags, int cr3, int cr0, int cr2, int cr4);

void print_back();

void print_ade(char tipo, int x, int y, int jug);

void print_der(char tipo, int x, int y, int jug);

void print_reloj(int i_tarea, char tick, int jug);
//void print_izq(char tipo, int x, int y, int jug);

void print_ganador(int puntos_jA, int puntos_jB);

void print_intr_line();

#endif  /* !__SCREEN_H__ */
