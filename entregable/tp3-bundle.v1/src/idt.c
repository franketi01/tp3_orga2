/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "screen.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                \
    idt[numero].attr = (unsigned short) 0x8e00;                                                                 \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY_SYS(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                \
    idt[numero].attr = (unsigned short) 0xee00;                                                                 \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    IDT_ENTRY(32);
    IDT_ENTRY(33);
    IDT_ENTRY_SYS(102);
}

void resolver_excepcion(int i,int ebx,int ecx,int edx,int esi,int edi,int ebp,int st5,int st4,int st3,int st2,int st1,int esp, int cr0, int cr2, int cr3, int cr4, int cs, int ds, int es, int fs, int gs, int ss, int eflags, int eip, int eax)
{
    switch(i){
            case 0:
                print("Divide Error",20,0,C_FG_WHITE);
                break;
            case 1:
                print("Reserved", 20,0,C_FG_WHITE);
                break;
            case 2:
                print("NMI Interrupt", 20,0,C_FG_WHITE);
                break;
            case 3:
                print("BreakPoint", 20,0,C_FG_WHITE);
                break;
            case 4:
                print("Overflow", 20,0,C_FG_WHITE);
                break;
            case 5:
                print("BOUND Range Exceeded", 20,0,C_FG_WHITE);
                break;
            case 6:
                print("Invalid Opcode", 20,0,C_FG_WHITE);
                break;
            case 7:
                print("Device Not Available", 20,0,C_FG_WHITE);
                break;
            case 8:
                print("Double Fault", 20,0,C_FG_WHITE);
                break;
            case 9:
                print("Coprocessor Segment Overrun", 20,0,C_FG_WHITE);
                break;
            case 10:
                print("Invalid TSS", 20,0,C_FG_WHITE);
                break;
            case 11:
                print("Segment Not Present", 20,0,C_FG_WHITE);
                break;
            case 12:
                print("Stack-Segment Fault", 20,0,C_FG_WHITE);;
                break;
            case 13:
                print("General protection fault", 20,0,C_FG_WHITE);
                break;
            case 14:
                print("Page fault", 20,0,C_FG_WHITE);
                break;
            case 15:
                print("Intel Reserved", 20,0,C_FG_WHITE);
                break;
            case 16:
                print("x86 FPU Floating-Point Error", 20,0,C_FG_WHITE);
                break;
            case 17:
                print("Alignment Check", 20,0,C_FG_WHITE);
                break;
            case 18:
                print("Machine Check", 20,0,C_FG_WHITE);
                break;
            case 19:
                print("SIMD Floating-Point Exception", 20,0,C_FG_WHITE);
                break;

            default:
                break;
        }

        desalojar_tarea(rtr());
        if(debug_on())
        {
            set_pause();
            
            char c = 0;
            int jug = -1;
            if (rtr() != 0x70)
            {
                info_tss * zombi = data_zombi(rtr());
                c = (*zombi).tipo_zombie;
                jug = (*zombi).jugador;
            }

            print_debug(c,jug,eax,ebx,ecx,edx,st5,st4,st3,st2,st1,esi,edi,ebp,esp,eip,cs,ds,es,fs,gs,ss,eflags,cr3,cr0,cr2,cr4);

        }
        
}

void atencion_teclado(unsigned int t)
{
    unsigned int h;              // altura actual del jugador
    char sb[] = {'x',0};        // para el simbolo tipo zombi a imprimir en pantalla con 0 por el salto de linea
    unsigned char c;            // tipo de zombi seleccionado a lanzar
    switch(t){
    case 0x1e: //A
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(0);
            cambiar_z(0,0);
            c = t_zomb(0);
            sb[0] = c;
            print(sb,0,h+1,C_FG_WHITE);
            break;
    case 0x11: //W
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(0);
            c = t_zomb(0);
            sb[0] = c;
            if(h > 0)
            {
                print(sb,0,h,C_FG_WHITE);
                if(h < 44)
                {
                    print(" ",0,h+1,C_BG_RED);
                }
                up_down(0,0);
            }
            
            break;
    case 0x1f: //s
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(0);
            unsigned int p = h + 1;
            c = t_zomb(0);
            sb[0] = c;
            if(h < 43)
            {
                print(sb,0,p+1,C_FG_WHITE);
                if(p > 0)
                {
                    print(" ",0,p,C_BG_RED);
                }
                up_down(0,1);
            }
            break;
    case 0x20: //d
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(0);
            cambiar_z(0,1);
            c = t_zomb(0);
            sb[0] = c;
            print(sb,0,h+1,C_FG_WHITE);
            break;
    case 0x17: //i
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(1);
            c = t_zomb(1);
            sb[0] = c;
            if(h > 0)
            {
                print(sb,79,h,C_FG_WHITE);
                if(h < 44)
                {
                    print(" ",79,h+1,C_BG_BLUE);
                }
                up_down(1,0);
            }
            break;
    case 0x25: //k
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(1);
            unsigned int q = h + 1;
            c = t_zomb(1);
            sb[0] = c;
            if(h < 43)
            {
                print(sb,79,q+1,C_FG_WHITE);
                if(q > 0)
                {
                    print(sb,79,q,C_BG_BLUE);
                }
                up_down(1,1);
            }
            break;
    case 0x26: //l
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(1);
            cambiar_z(1,1);
            c = t_zomb(1);
            sb[0] = c;
            print(sb,79,h+1,C_FG_WHITE);
            break;
    case 0x24: //j
            print_hex(t,3,75,0,C_FG_WHITE);
            h = altura_player(1);
            cambiar_z(1,0);
            c = t_zomb(1);
            sb[0] = c;
            print(sb,79,h+1,C_FG_WHITE);
            break;
    case 0x2a: //LShift
            if(puede_lanzar(0))
            {
                print_hex(t,3,75,0,C_FG_WHITE);
                h = altura_player(0);                        //en estas lanzo el zombi, por ahora solo imprime el tipo zombie, sin iniciar la tarea en si
                c = t_zomb(0);
                sb[0] = c;
                print(sb,2,h+1,C_FG_WHITE);                                //recordar la posicion en la pantalla no es la misma que toma la tarea, la cual es una dir fisica
            //faltaria iniciar la tarea adecuadamente
                tss_zombi_lanzar(0,c,1,h);
                se_lanzo_zombi(0);
            }
            break;
    case 0x36: //RShift
            if(puede_lanzar(1))
            {    
                print_hex(t,3,75,0,C_FG_WHITE);
                h = altura_player(1); 
                c = t_zomb(1);
                sb[0] = c;  
                print(sb,77,h+1,C_FG_WHITE);        
            //faltaria iniciar la tarea adecuadamente
                tss_zombi_lanzar(1,c,76,h);
                se_lanzo_zombi(1);
            }
            break;
    case 0x15: //y
            print_hex(t,3,75,0,C_FG_WHITE);
            //tss * task = tss_actual();
            press_debug();
            print_intr_line();
            if(r_pause())
            {
                print_back();
                set_pause();
            }

            break;
    default:
            break;
    }
}

void mover_zombi(unsigned int cr3, unsigned int mov)
{
    info_tss* zombi = data_zombi(rtr());

    int j = (*zombi).jugador;
    int x = (*zombi).pos_x;
    int y = (*zombi).pos_y;
    char c = (*zombi).tipo_zombie;
    if(mov == 0x83D)
    {
        mov_adelante(cr3,x,y,j,c);
    }
    else if(mov == 0x441)
    {
        mov_derecha(cr3,x,y,j,c);
    }
    else if(mov == 0xAAA)
    {
        mov_izquierda(cr3,x,y,j,c);
    }
    else if(mov == 0x732)
    {
        mov_atras(cr3,x,y,j,c);
    }
}

void mov_adelante(int cr3, int x, int y, int jug,char tipo)
{
   if((jug == 0 && x+1 == 77) || (jug ==1 && x-1 == 0))
   {
        desalojar_tarea(rtr());
        se_gano_puntos((jug+1)%2);
   }
   else
   {
        info_tss * zomb = data_zombi(rtr());
       // unsigned int fisica_pos = 0x400000 + y*0x4E000 + x*0x1000;
        unsigned int pos_mapa1;
        unsigned int pos_mapa2;
        unsigned int pos_mapa3;
        unsigned int pos_mapa4;
        unsigned int pos_mapa5;
        unsigned int pos_mapa6;
        unsigned int pos_mapa7;
        unsigned int pos_mapa8;
        unsigned int pos_mapa9;

        switch(jug)
        {
            case 0:
                pos_mapa1 = 0x400000 + y*0x4E000 + (x+1)*0x1000;
                pos_mapa2 = 0x400000 + y*0x4E000 + (x+2)*0x1000;
                pos_mapa7 = 0x400000 + y*0x4E000 + x*0x1000;
                if(y == 0)
                {
                    pos_mapa4 = 0x400000 + 43*0x4E000 + (x+2)*0x1000;
                    pos_mapa6 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                    pos_mapa8 = 0x400000 + 43*0x4E000 + x*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (y-1)*0x4E000 + (x+2)*0x1000;
                    pos_mapa6 = 0x400000 + (y-1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa8 = 0x400000 + (y-1)*0x4E000 + x*0x1000;
                }
                if(y == 43)
                {
                    pos_mapa3 = 0x400000 + (x+2)*0x1000;
                    pos_mapa5 = 0x400000 + (x+1)*0x1000;
                    pos_mapa9 = 0x400000 + x*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (y+1)*0x4E000 + (x+2)*0x1000;
                    pos_mapa5 = 0x400000 + (y+1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa9 = 0x400000 + (y+1)*0x4E000 + x*0x1000;
                }
                (*zomb).pos_x += 1;
                
                break;
            case 1:
                pos_mapa1 = 0x400000 + y*0x4E000 + (x-1)*0x1000;
                pos_mapa2 = 0x400000 + y*0x4E000 + (x-2)*0x1000;
                pos_mapa7 = 0x400000 + y*0x4E000 + x*0x1000;
                if(y == 0)
                {
                    pos_mapa3 = 0x400000 + 43*0x4E000 + (x-2)*0x1000;
                    pos_mapa5 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                    pos_mapa9 = 0x400000 + 43*0x4E000 + x*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (y-1)*0x4E000 + (x-2)*0x1000;
                    pos_mapa5 = 0x400000 + (y-1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa9 = 0x400000 + (y-1)*0x4E000 + x*0x1000;
                }
                if(y == 43)
                {
                    pos_mapa4 = 0x400000 + (x-2)*0x1000;
                    pos_mapa6 = 0x400000 + (x-1)*0x1000;
                    pos_mapa8 = 0x400000 + x*0x1000; 
                }
                else
                {
                    pos_mapa4 = 0x400000 + (y+1)*0x4E000 + (x-2)*0x1000;
                    pos_mapa6 = 0x400000 + (y+1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa8 = 0x400000 + (y+1)*0x4E000 + x*0x1000;
                }
                (*zomb).pos_x -= 1;
                break;
            default:
                break;
        }
        mapear_pagina(0x8000000, cr3, pos_mapa1, 1);
        mapear_pagina(0x8001000, cr3, pos_mapa2, 1);
        mapear_pagina(0x8002000, cr3, pos_mapa3, 1);
        mapear_pagina(0x8003000, cr3, pos_mapa4, 1);
        mapear_pagina(0x8004000, cr3, pos_mapa5, 1);
        mapear_pagina(0x8005000, cr3, pos_mapa6, 1);
        mapear_pagina(0x8006000, cr3, pos_mapa7, 1);
        mapear_pagina(0x8007000, cr3, pos_mapa8, 1);
        mapear_pagina(0x8008000, cr3, pos_mapa9, 1);

        map_pagina_copiar_codigo(cr3,x,y,jug,tipo, pos_mapa1);

        print_ade(tipo,x,y,jug);
        
   }
}

void mov_derecha(int cr3, int x, int y, int jug,char tipo)
{
        info_tss * zomb = data_zombi(rtr());
       // unsigned int fisica_pos = 0x400000 + y*0x4E000 + x*0x1000;
        unsigned int pos_mapa1;
        unsigned int pos_mapa2;
        unsigned int pos_mapa3;
        unsigned int pos_mapa4;
        unsigned int pos_mapa5;
        unsigned int pos_mapa6;
        unsigned int pos_mapa7;
        unsigned int pos_mapa8;
        unsigned int pos_mapa9;
        int n_y;
        switch(jug)
        {
            case 0:
                if(y == 43)
                {   
                    n_y = 0;
                }
                else
                {
                    n_y = y+1;
                }
                pos_mapa1 = 0x400000 + n_y*0x4E000 + x*0x1000;
                pos_mapa2 = 0x400000 + n_y*0x4E000 + (x+1)*0x1000;
                pos_mapa7 = 0x400000 + n_y*0x4E000 + (x-1)*0x1000;
                if(n_y == 0)
                {
                    pos_mapa4 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                    pos_mapa6 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (n_y-1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa6 = 0x400000 + (n_y-1)*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (n_y-1)*0x4E000 + (x-1)*0x1000;   
                }
                if(n_y == 43)
                {
                    pos_mapa3 = 0x400000 + (x+1)*0x1000;
                    pos_mapa5 = 0x400000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (x-1)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (n_y+1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa5 = 0x400000 + (n_y+1)*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (n_y+1)*0x4E000 + (x-1)*0x1000;
                }
                break;
            case 1:
                if(y == 0)
                {
                    n_y = 43;
                }
                else
                {
                    n_y = y-1;
                }
                pos_mapa1 = 0x400000 + n_y*0x4E000 + x*0x1000;
                pos_mapa2 = 0x400000 + n_y*0x4E000 + (x-1)*0x1000;
                pos_mapa7 = 0x400000 + n_y*0x4E000 + (x+1)*0x1000;
                if(n_y == 0)
                {
                    pos_mapa3 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                    pos_mapa5 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (n_y-1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa5 = 0x400000 + (n_y-1)*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (n_y-1)*0x4E000 + (x+1)*0x1000;
                }
                if(n_y == 43)
                {
                    pos_mapa4 = 0x400000 + (x-1)*0x1000;
                    pos_mapa6 = 0x400000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (x+1)*0x1000; 
                }
                else
                {
                    pos_mapa4 = 0x400000 + (n_y+1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa6 = 0x400000 + (n_y+1)*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (n_y+1)*0x4E000 + (x+1)*0x1000; 
                }
                break;
            default:
                break;
        }
        (*zomb).pos_y = n_y;
        mapear_pagina(0x8000000, cr3, pos_mapa1, 1);
        mapear_pagina(0x8001000, cr3, pos_mapa2, 1);
        mapear_pagina(0x8002000, cr3, pos_mapa3, 1);
        mapear_pagina(0x8003000, cr3, pos_mapa4, 1);
        mapear_pagina(0x8004000, cr3, pos_mapa5, 1);
        mapear_pagina(0x8005000, cr3, pos_mapa6, 1);
        mapear_pagina(0x8006000, cr3, pos_mapa7, 1);
        mapear_pagina(0x8007000, cr3, pos_mapa8, 1);
        mapear_pagina(0x8008000, cr3, pos_mapa9, 1);

        map_pagina_copiar_codigo(cr3,x,n_y,jug,tipo, pos_mapa1);
        print_der(tipo,x,n_y,jug);
}

void mov_izquierda(int cr3, int x, int y, int jug,char tipo)
{
        info_tss * zomb = data_zombi(rtr());
       // unsigned int fisica_pos = 0x400000 + y*0x4E000 + x*0x1000;
        unsigned int pos_mapa1;
        unsigned int pos_mapa2;
        unsigned int pos_mapa3;
        unsigned int pos_mapa4;
        unsigned int pos_mapa5;
        unsigned int pos_mapa6;
        unsigned int pos_mapa7;
        unsigned int pos_mapa8;
        unsigned int pos_mapa9;
        int n_y;
        switch(jug)
        {
            case 0:
                if(y == 0)
                {
                    n_y = 43;
                }
                else
                {
                    n_y = y-1;
                }
                pos_mapa1 = 0x400000 + n_y*0x4E000 + x*0x1000;
                pos_mapa2 = 0x400000 + n_y*0x4E000 + (x+1)*0x1000;
                pos_mapa7 = 0x400000 + n_y*0x4E000 + (x-1)*0x1000;
                if(n_y == 0)
                {
                    pos_mapa4 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                    pos_mapa6 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (n_y-1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa6 = 0x400000 + (n_y-1)*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (n_y-1)*0x4E000 + (x-1)*0x1000;
                }
                if(n_y == 43)
                {
                    pos_mapa3 = 0x400000 + (x+1)*0x1000;
                    pos_mapa5 = 0x400000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (x-1)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (n_y+1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa5 = 0x400000 + (n_y+1)*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (n_y+1)*0x4E000 + (x-1)*0x1000;
                }
                break;
            case 1:
                if(y == 43)
                {   
                    n_y = 0;
                }
                else
                {
                    n_y = y+1;
                }
                pos_mapa1 = 0x400000 + n_y*0x4E000 + x*0x1000;
                pos_mapa2 = 0x400000 + n_y*0x4E000 + (x-1)*0x1000;
                pos_mapa7 = 0x400000 + n_y*0x4E000 + (x+1)*0x1000;
                if(n_y == 0)
                {
                    pos_mapa3 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                    pos_mapa5 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (n_y-1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa5 = 0x400000 + (n_y-1)*0x4E000 + x*0x1000;
                    pos_mapa9 = 0x400000 + (n_y-1)*0x4E000 + (x+1)*0x1000;
                }
                if(n_y == 43)
                {
                    pos_mapa4 = 0x400000 + (x-1)*0x1000;
                    pos_mapa6 = 0x400000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (x+1)*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (n_y+1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa6 = 0x400000 + (n_y+1)*0x4E000 + x*0x1000;
                    pos_mapa8 = 0x400000 + (n_y+1)*0x4E000 + (x+1)*0x1000;
                }
                break;
            default:
                break;
        }
        (*zomb).pos_y = n_y;
        mapear_pagina(0x8000000, cr3, pos_mapa1, 1);
        mapear_pagina(0x8001000, cr3, pos_mapa2, 1);
        mapear_pagina(0x8002000, cr3, pos_mapa3, 1);
        mapear_pagina(0x8003000, cr3, pos_mapa4, 1);
        mapear_pagina(0x8004000, cr3, pos_mapa5, 1);
        mapear_pagina(0x8005000, cr3, pos_mapa6, 1);
        mapear_pagina(0x8006000, cr3, pos_mapa7, 1);
        mapear_pagina(0x8007000, cr3, pos_mapa8, 1);
        mapear_pagina(0x8008000, cr3, pos_mapa9, 1);

        map_pagina_copiar_codigo(cr3,x,n_y,jug,tipo, pos_mapa1);
        print_der(tipo,x,n_y,(jug+1)%2);
}

void mov_atras(int cr3, int x, int y, int jug,char tipo)
{
    if((jug == 0 && x == 1) || (jug == 1 && x+1 == 76))
    {
        desalojar_tarea(rtr());
        se_gano_puntos((jug+1)%2);
    }
    else
    {
        info_tss * zomb = data_zombi(rtr());
       // unsigned int fisica_pos = 0x400000 + y*0x4E000 + x*0x1000;
        unsigned int pos_mapa1;
        unsigned int pos_mapa2;
        unsigned int pos_mapa3;
        unsigned int pos_mapa4;
        unsigned int pos_mapa5;
        unsigned int pos_mapa6;
        unsigned int pos_mapa7;
        unsigned int pos_mapa8;
        unsigned int pos_mapa9;
        switch(jug)
        {
            case 0:
                pos_mapa1 = 0x400000 + y*0x4E000 + (x-1)*0x1000;
                pos_mapa2 = 0x400000 + y*0x4E000 + x*0x1000;
                pos_mapa7 = 0x400000 + y*0x4E000 + (x-2)*0x1000;
                if(y == 0)
                {
                    pos_mapa4 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa6 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
                    pos_mapa8 = 0x400000 + 43*0x4E000 + (x-2)*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (y-1)*0x4E000 + x*0x1000;
                    pos_mapa6 = 0x400000 + (y-1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa8 = 0x400000 + (y-1)*0x4E000 + (x-2)*0x1000;
                }
                if(y == 43)
                {
                    pos_mapa3 = 0x400000 + x*0x1000;
                    pos_mapa5 = 0x400000 + (x-1)*0x1000;
                    pos_mapa9 = 0x400000 + (x-2)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (y+1)*0x4E000 + x*0x1000;
                    pos_mapa5 = 0x400000 + (y+1)*0x4E000 + (x-1)*0x1000;
                    pos_mapa9 = 0x400000 + (y+1)*0x4E000 + (x-2)*0x1000;
                }
                (*zomb).pos_x -= 1;
                break;
            case 1:
                pos_mapa1 = 0x400000 + y*0x4E000 + (x+1)*0x1000;
                pos_mapa2 = 0x400000 + y*0x4E000 + x*0x1000;
                pos_mapa7 = 0x400000 + y*0x4E000 + (x+2)*0x1000;
                if(y == 0)
                {
                    pos_mapa3 = 0x400000 + 43*0x4E000 + x*0x1000;
                    pos_mapa5 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
                    pos_mapa9 = 0x400000 + 43*0x4E000 + (x+2)*0x1000;
                }
                else
                {
                    pos_mapa3 = 0x400000 + (y-1)*0x4E000 + x*0x1000;
                    pos_mapa5 = 0x400000 + (y-1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa9 = 0x400000 + (y-1)*0x4E000 + (x+2)*0x1000;
                }
                if(y == 43)
                {
                    pos_mapa4 = 0x400000 + x*0x1000;
                    pos_mapa6 = 0x400000 + (x+1)*0x1000;
                    pos_mapa8 = 0x400000 + (x+2)*0x1000;
                }
                else
                {
                    pos_mapa4 = 0x400000 + (y+1)*0x4E000 + x*0x1000;
                    pos_mapa6 = 0x400000 + (y+1)*0x4E000 + (x+1)*0x1000;
                    pos_mapa8 = 0x400000 + (y+1)*0x4E000 + (x+2)*0x1000;
                }
                (*zomb).pos_x += 1;
                break;
            default:
                break;
        }
        mapear_pagina(0x8000000, cr3, pos_mapa1, 1);
        mapear_pagina(0x8001000, cr3, pos_mapa2, 1);
        mapear_pagina(0x8002000, cr3, pos_mapa3, 1);
        mapear_pagina(0x8003000, cr3, pos_mapa4, 1);
        mapear_pagina(0x8004000, cr3, pos_mapa5, 1);
        mapear_pagina(0x8005000, cr3, pos_mapa6, 1);
        mapear_pagina(0x8006000, cr3, pos_mapa7, 1);
        mapear_pagina(0x8007000, cr3, pos_mapa8, 1);
        mapear_pagina(0x8008000, cr3, pos_mapa9, 1);

        map_pagina_copiar_codigo(cr3,x,y,jug,tipo,pos_mapa1);
        print_ade(tipo,x,y,(jug+1)%2);
    }
}


void map_pagina_copiar_codigo(int cr3, int x, int y, int jug,char tipo, unsigned int fisica)
{
    unsigned int* src_code = (unsigned int *) codigo_tarea(tipo,jug);
    unsigned int* dst_code = (unsigned int *) fisica;
    int i;
    mapear_pagina(fisica,cr3,fisica,1);

    for (i = 0; i < 1024; i++)
    {
        dst_code[i] = src_code[i];
    }

    unmapear_pagina(fisica, cr3);
        
    tlbflush();    
}
