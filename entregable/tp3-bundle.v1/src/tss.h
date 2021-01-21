/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"
#include "screen.h"

typedef struct str_tss {
    unsigned short  ptl;
    unsigned short  unused0;
    unsigned int    esp0;
    unsigned short  ss0;
    unsigned short  unused1;
    unsigned int    esp1;
    unsigned short  ss1;
    unsigned short  unused2;
    unsigned int    esp2;
    unsigned short  ss2;
    unsigned short  unused3;
    unsigned int    cr3;
    unsigned int    eip;
    unsigned int    eflags;
    unsigned int    eax;
    unsigned int    ecx;
    unsigned int    edx;
    unsigned int    ebx;
    unsigned int    esp;
    unsigned int    ebp;
    unsigned int    esi;
    unsigned int    edi;
    unsigned short  es;
    unsigned short  unused4;
    unsigned short  cs;
    unsigned short  unused5;
    unsigned short  ss;
    unsigned short  unused6;
    unsigned short  ds;
    unsigned short  unused7;
    unsigned short  fs;
    unsigned short  unused8;
    unsigned short  gs;
    unsigned short  unused9;
    unsigned short  ldt;
    unsigned short  unused10;
    unsigned short  dtrap;
    unsigned short  iomap;
} __attribute__((__packed__, aligned (8))) tss;

typedef struct info_tss_zombi
{
    unsigned int pos_x;     //pos x en el mapa
    unsigned int pos_y; //pos y en el mapa
    unsigned char tipo_zombie;      //tipo de zombie: M, C, G
    unsigned short indice_gdt;      //posicion indice en la gdt 
    unsigned char active;           // verifica si el zombie es 'A' (esta activo y se puede mover), o 'X' (zombi muerto o slot de zombie no usable)
    int jugador;
    unsigned char clock;    // tick de reloj 0 al 3 se corresponde con los caracteres '- \ | /'
    //revisar si necesite mas cosas
}__attribute__((__packed__, aligned (8))) info_tss;

typedef struct Player_type
{
    unsigned int altura;            //altura en el mapa. tener en cuenta que los posicion en la pantalla no son las mismas a las del juego (pantalla verde)
    unsigned char z_lanzar;         // tipo zombi a lanzar
    unsigned int z_disponibles;     // zombies disponibles. inicia en 20 
    unsigned int z_en_juego;        // zombies en el mapa, inicia en 0. Maximo 8
    unsigned int puntos;            // puntos inicia en 0
    unsigned int zombi_actual;     // inicia en -1 y se mantiene asi si no hay ningun zombie con actividad
    unsigned short ciclos;          //inicia en 1000. Es una cantidad de ticks que decrece en caso de que el jugador tenga 8 zombis activos en el mapa, de no ser asi el valor reinicia
}__attribute__((__packed__, aligned (8))) player;

void tss_inicializar();
void inicializar_idle();

void tss_zombi_lanzar(int jugador, unsigned char tipo, int x, int y);

//jugadores
void players();                         // con i me refiero al jugador A=0 , B=1, inicia los valores de los jugadores
unsigned int altura_player(int i);      // devuelve la altura del jugador i
unsigned char t_zomb(int i);            // retorna el caracter con el tipo de zombi que el jugador i esta seleccionando 
void up_down(int i, int d);            //aumenta o decrece la altura del jugador i segun el movimiento d. Con d le digo arriba(1) o abajo(0) 
void cambiar_z(int i, int s);          // cambia el caracter del tipo de zombie del jugador i que se quiere lanzar
void se_gano_puntos(int i);           //incremento un punto al jugador i, verifica si llego a 10

unsigned char puede_lanzar(int i);      //devuelve 1 si el jugador i esta en condiciones de lanzar, 0 en caso contrario 
void se_lanzo_zombi(int i);             //esta funcion modifica atributos del jugador i cuando este lanzo un zombi

unsigned short prox_Z_tarea(int jugador);   //busca en los slots de zombis del jugador, al proximo zombi disponibles (active = 'A), y retorna el indice que ocupa en la gdt

tss * tss_actual();                         //retorna un puntero a la tss actual, utiliza el valor del registro tr, y verifica a que jugador pertenece y que indice de tarea tss es, si no es ninguno retorna un puntero a la tss idle
info_tss * data_zombi(unsigned short sel);  //pasandole el selector de gdt de una tarea, retorno un puntero a info_tss, verificando con el parametro el jugador i la posicion del zombie correspondiente, no se le puede pasar el selector idle 

unsigned char r_pause();    //retorna la variable de pausa en 1 si lo esta, y 0 si no. Se usa como funcion booleana
void set_pause();   //setea la variable de pause en 0 o 1

void desalojar_tarea(unsigned short sel);   //se le pase el selector de la tarea (info_tss) que se quiere desalojar, o mas bien libera el slot para que vuelva a usarse cuando se lanze un nuevo zombi

unsigned char debug_on();               // retorna el 1 si esta activo el debug, y 0 si no
void press_debug();                 // itercambia el valor de debug en 0 (OFF), y 1 (ON)

unsigned int hay_ganador();         //retorna 1 si alguno de los jugadores llego a 10 puntos o si a alguno se quedo sin ticks de reloj, 0 si no
unsigned int sin_zombis();          //retorna 1 si alguno de los jugadores se quedo sin zombis disponibles y al mismo tiempo si el ultimo zombie en el mapa murio

#endif  /* !__TSS_H__ */
