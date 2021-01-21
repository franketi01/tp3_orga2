/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __GDT_H__
#define __GDT_H__

#include "defines.h"

#define kernel_code                 8

#define kernel_data                 9

#define user_code                   10

#define user_data                   11

#define pantalla                    12

#define tarea_inicial               13 

#define tarea_idle                  14 

#define tarea_1A                    15

#define tarea_2A                    16

#define tarea_3A                    17

#define tarea_4A                    18

#define tarea_5A                    19

#define tarea_6A                    20

#define tarea_7A                    21

#define tarea_8A                    22

#define tarea_1B                    23

#define tarea_2B                    24

#define tarea_3B                    25

#define tarea_4B                    26

#define tarea_5B                    27

#define tarea_6B                    28

#define tarea_7B                    29

#define tarea_8B                    30


typedef struct str_gdt_descriptor {
    unsigned short  gdt_length;
    unsigned int    gdt_addr;
} __attribute__((__packed__)) gdt_descriptor;

typedef struct str_gdt_entry {
    unsigned short  limit_0_15;
    unsigned short  base_0_15;
    unsigned char   base_23_16;
    unsigned char   type:4;
    unsigned char   s:1;
    unsigned char   dpl:2;
    unsigned char   p:1;
    unsigned char   limit_16_19:4;
    unsigned char   avl:1;
    unsigned char   l:1;
    unsigned char   db:1;
    unsigned char   g:1;
    unsigned char   base_31_24;
} __attribute__((__packed__, aligned (8))) gdt_entry;

/* Tabla GDT */
extern gdt_entry gdt[];
extern gdt_descriptor GDT_DESC;


#endif  /* !__GDT_H__ */
