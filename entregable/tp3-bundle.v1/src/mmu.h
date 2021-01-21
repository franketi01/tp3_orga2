/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

#define dir_directory 0x27000
#define dir_table 0x28000
#define inicio_paginas_libres 0x100000
#define size_tabla 0x1000
#define comienzo_mem_virtual_user 0x08000000
#define tipoZombiG 0
#define tipoZombiM 1
#define tipoZombiC 2


typedef struct directory_entry {
    unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   cero:1;
    unsigned char   ps:1;
    unsigned char   g:1;
    unsigned char   disp:3;
    unsigned int  dir_base:20;	
} __attribute__((__packed__, aligned (4))) dir_entry;

typedef struct table_entry {
    unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   d:1;
    unsigned char   pat:1;
    unsigned char   g:1;
    unsigned char   disp:3;
    unsigned int  dir_base:20;	
} __attribute__((__packed__, aligned (4))) tab_entry;

unsigned int get_next_free_page();

unsigned int codigo_tarea(char tipo, int jug);

void mmu_inicializar();
void mmu_inicializar_dir_kernel();
void inicializar_mmu();
unsigned int mmu_inicializar_dir_zombi(int dir_zombi, int player, int x, int y);
void mapear_pagina_a_mapa(int x, int y, unsigned int page, int jugador, int tipo_zombi);
void mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, char us);
void unmapear_pagina(unsigned int virtual, unsigned int cr3);
void copiar_codigo_a_mapa(int tipo_zombi,int player, unsigned int fisica);

#endif	/* !__MMU_H__ */




