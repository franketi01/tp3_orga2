/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

#define base23(direccion) (((unsigned int) (direccion)) >> 16)
#define base31(direccion) (((unsigned int) (direccion)) >> 24)

#define GDT_ZOMBI_A(numero, indice)																\
  gdt[numero].base_0_15  = (unsigned short) ((unsigned int) (&tss_zombisA[indice])); 			\
  gdt[numero].base_23_16 = (unsigned char)  ((unsigned int) base23((&tss_zombisA[indice])));		\
  gdt[numero].base_31_24 = (unsigned char)  ((unsigned int) base31((&tss_zombisA[indice])));	

#define GDT_ZOMBI_B(numero, indice)																\
  gdt[numero].base_0_15  = (unsigned short) ((unsigned int) (&tss_zombisB[indice])); 			\
  gdt[numero].base_23_16 = (unsigned char)  ((unsigned int) base23((&tss_zombisB[indice])));		\
  gdt[numero].base_31_24 = (unsigned char)  ((unsigned int) base31((&tss_zombisB[indice])));	

unsigned char pause = 0;
unsigned char debug_y = 0;

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];


info_tss info_zA[CANT_ZOMBIS];	// estructuras para guardar info de zombies, pos actual, tipo
info_tss info_zB[CANT_ZOMBIS];	//si falta algo agregar

player jugadores[2];

void tss_inicializar() { 
}

void inicializar_idle(){

	tss_idle.eip = 0x16000;
	tss_idle.ebp = 0x27000;
	tss_idle.esp = 0x27000;
	tss_idle.es = 0x060;
	tss_idle.cs = 0x40;
	tss_idle.ss = 0x48;
	tss_idle.ds	= 0x48;
	tss_idle.fs = 0x48;
	tss_idle.gs = 0x48;
	tss_idle.cr3 = 0x27000;
	tss_idle.eflags = 0x202;  //con interrupciones habilitadas
	//llenamos la direccion base del descriptor tss de tarea inicial
   	gdt[tarea_inicial].base_0_15 = (unsigned short) ((unsigned int) &tss_inicial);
   	gdt[tarea_inicial].base_23_16 = (unsigned char) ((unsigned int) &tss_inicial)>>16;
   	gdt[tarea_inicial].base_31_24 = (unsigned char) ((unsigned int) &tss_inicial)>>24;
	//llenamos la direccion base del descriptor tss de tarea idle
   	gdt[tarea_idle].base_0_15 = (unsigned short) ((unsigned int) &tss_idle);
   	gdt[tarea_idle].base_23_16 = (unsigned char) ((unsigned int) &tss_idle)>>16;
   	gdt[tarea_idle].base_31_24 = (unsigned char) ((unsigned int) &tss_idle)>>24;


}

void tss_zombi_lanzar(int jugador, unsigned char tipo, int x, int y)
{	
	int t;
	int j_g;
	tss * tarea_tss;
	info_tss* tarea_zombi;
	if(tipo == 'G')
	{
		t = 0;
	}
	else if(tipo == 'M')
	{
		t = 1;
	}
	else if(tipo == 'C')
	{
		t = 2;
	}

	if(jugador == 0)
	{
		tarea_zombi = info_zA;
		tarea_tss = tss_zombisA;
	}
	else
	{
		tarea_zombi = info_zB;
		tarea_tss = tss_zombisB;
	}
	int i = 0;
	while(i < 8 && tarea_zombi[i].active != 'X')
	{
		i++;
	}
	j_g = tarea_zombi[i].indice_gdt;
	tarea_zombi[i].active = 'A';
	tarea_zombi[i].pos_x = x;
	tarea_zombi[i].pos_y = y;
	tarea_zombi[i].tipo_zombie = tipo;

	tarea_tss[i].eip = 0x8000000;
	tarea_tss[i].ebp = 0x8001000;	
	tarea_tss[i].esp = 0x8001000;
	tarea_tss[i].es = (((unsigned int) user_data) << 3) + 0x3; //descriptores de segmento de datos lvl 3
	tarea_tss[i].cs = (((unsigned int) user_code) << 3) + 0x3; //descriptores de segmento de codigo lvl 3
	tarea_tss[i].ss = (((unsigned int) user_data) << 3) + 0x3;
	tarea_tss[i].ds = (((unsigned int) user_data) << 3) + 0x3;
	tarea_tss[i].fs = (((unsigned int) user_data) << 3) + 0x3;
	tarea_tss[i].gs = (((unsigned int) user_data) << 3) + 0x3; //
	tarea_tss[i].cr3 = mmu_inicializar_dir_zombi(t,jugador,x,y);
	tarea_tss[i].eflags = 0x202;
	tarea_tss[i].esp0 = get_next_free_page() + 0x1000;      // no se si esto es necesario, o  si se deberia ser la actual pagina libre
	tarea_tss[i].ss0 = (unsigned int) (kernel_data << 3);

	if(jugador == 0)
	{
		GDT_ZOMBI_A(j_g,i);
	}
	else
	{
		GDT_ZOMBI_B(j_g,i);
	}

}


void players()
{
	jugadores[0].altura = 22;
	jugadores[0].z_lanzar = 'M';
	jugadores[0].z_disponibles = 20;
	jugadores[0].z_en_juego = 0;
	jugadores[0].puntos = 0;
	jugadores[0].zombi_actual = -1;
	jugadores[0].ciclos = 1000;
	print_int(jugadores[0].puntos,37,47,C_FG_WHITE);
	print_int(jugadores[0].z_disponibles,32,47,C_FG_WHITE);


	jugadores[1].altura = 22;
	jugadores[1].z_lanzar = 'M';
	jugadores[1].z_disponibles = 20;
	jugadores[1].z_en_juego = 0;
	jugadores[1].puntos = 0;
	jugadores[1].zombi_actual = -1;
	jugadores[1].ciclos = 1000;
	print_int(jugadores[1].puntos,42,47,C_FG_WHITE);
	print_int(jugadores[1].z_disponibles,47,47,C_FG_WHITE);

	int i;
	for (i = 0; i < 8; ++i)
	{
		info_zA[i].jugador = 0;
		info_zB[i].jugador = 1;

		info_zA[i].active = 'X';
		info_zB[i].active = 'X';

		info_zA[i].indice_gdt = 15 + i;
		info_zB[i].indice_gdt = 23 + i;

		info_zA[i].clock = 0;
		info_zB[i].clock = 0;	
	}	
}

unsigned int altura_player(int i)
{
	return jugadores[i].altura;
}

unsigned char t_zomb(int i)
{
	return jugadores[i].z_lanzar;
}

void up_down(int i, int d)
{
	if(d == 0)
	{
		jugadores[i].altura -= 1;
	}
	else
	{
		jugadores[i].altura += 1;
	}
}

void cambiar_z(int i, int s)
{
	if(s == 0)
	{
		if(jugadores[i].z_lanzar == 'M')
		{
			jugadores[i].z_lanzar = 'G';
		}
		else if(jugadores[i].z_lanzar == 'G')
		{
			jugadores[i].z_lanzar = 'C';
		}
		else if(jugadores[i].z_lanzar == 'C')
		{
			jugadores[i].z_lanzar = 'M'; 
		}
	}
	else
	{
		if(jugadores[i].z_lanzar == 'M')
		{
			jugadores[i].z_lanzar = 'C';
		}
		else if(jugadores[i].z_lanzar == 'C')
		{
			jugadores[i].z_lanzar = 'G';
		}
		else if(jugadores[i].z_lanzar == 'G')
		{
			jugadores[i].z_lanzar = 'M'; 
		}
	}
}

void se_gano_puntos(int i)
{
	int j = (i+1)%2;
	jugadores[i].puntos += 1;


	if(i == 0)
	{
		print_int(jugadores[i].puntos,37,47,C_FG_WHITE);
		print_int(jugadores[j].z_disponibles,47,47,C_FG_WHITE);
	} 
	else
	{
		print_int(jugadores[i].puntos,42,47,C_FG_WHITE);
		print_int(jugadores[j].z_disponibles,32,47,C_FG_WHITE);
	}

	if(hay_ganador())
	{
		print_ganador(jugadores[0].puntos,jugadores[1].puntos);
	}
}           


unsigned char puede_lanzar(int i)
{
	if(!hay_ganador())
	{
		if(jugadores[i].z_disponibles > 0)
		{
			return jugadores[i].z_en_juego < 8;
		}
	}
	return 0;
}

void se_lanzo_zombi(int i)
{
	jugadores[i].z_disponibles -= 1;
	jugadores[i].z_en_juego += 1;
	if(i == 0)
	{
		print(" ",31,47,C_FG_BLACK);
		print_int(jugadores[i].z_disponibles,32,47,C_FG_WHITE);
	}
	else
	{
		print(" ",46,47,C_BG_BLACK);
		print_int(jugadores[i].z_disponibles,47,47,C_FG_WHITE);
	}	
}

unsigned short prox_Z_tarea(int jugador)
{
	unsigned short sel = 0x00;
	int i = (jugadores[jugador].zombi_actual+1) % 8;
	info_tss * selec_prox;
	if(jugador == 0)
	{
		selec_prox = info_zA;
	}
	else
	{
		selec_prox = info_zB;
	}
	while(i < 8 && selec_prox[i].active == 'X')
	{
		i++;
	}
	if(i < 8)
	{
		sel = (unsigned short) (selec_prox[i].indice_gdt << 3);	
		jugadores[jugador].zombi_actual = i;
		selec_prox[i].clock = (selec_prox[i].clock+1)%4;
		print_reloj(i,selec_prox[i].clock,jugador);
	}
	else
	{
		jugadores[jugador].zombi_actual = -1;
	}
	if(jugadores[jugador].z_en_juego == 8)
	{
		jugadores[jugador].ciclos -= 1;
		if(jugadores[jugador].ciclos == 0)
		{
			print_ganador(jugadores[0].puntos, jugadores[1].puntos);
		}
	}
	else
	{
		jugadores[jugador].ciclos = 1000;
	}

	return sel;
}


tss * tss_actual()
{
	unsigned int tr = rtr() >> 3;
	int i;
	if(tr >= 15 && tr <= 22)
	{
		i = tr % 15;
		return (tss*) &tss_zombisA[i];
	}
	else if(tr >= 23 && tr <= 30)
	{
		i = tr % 23;
		return (tss*) &tss_zombisB[i];	
	}

	return (tss*) &tss_idle;
}

info_tss * data_zombi(unsigned short sel)
{
	int i = sel >> 3;
	info_tss* ret;
	if(i >= 15 && i <= 22)
	{	
		i = i % 15;
		ret = (info_tss*) &info_zA[i];
	}
	else
	{
		i = i % 23;
		ret = (info_tss*) &info_zB[i];
	}

    return ret;
}


unsigned char r_pause()
{
	return pause == 1;
}
void set_pause()
{
	if(pause == 1)
	{
		pause = 0;
	}
	else
	{
		pause = 1;
	}
}

void desalojar_tarea(unsigned short sel)
{
	info_tss * task = data_zombi(sel);
	(*task).active = 'X';
	int i;
	int x = (*task).pos_x;
	int y = (*task).pos_y;
	jugadores[(*task).jugador].z_en_juego -= 1;
	unsigned short atr;
	if((*task).jugador == 0)
	{
		i = (*task).indice_gdt % 15;
		atr = C_FG_RED;
	}
	else
	{
		i = (*task).indice_gdt % 23;
		atr = C_FG_BLUE;
	}
	print("X",x+1,y+1,atr | C_BG_GREEN);
	print_reloj(i,4,(*task).jugador);

	if(sin_zombis())
	{
		print_ganador(jugadores[0].puntos,jugadores[1].puntos);
	}
}

unsigned char debug_on()
{
	return debug_y == 1;
}

void press_debug()
{
	debug_y = (debug_y+1)%2;
	if(debug_y == 1)
	{
		print("on ",28,49,C_FG_WHITE);
	}
	else
	{
		print("off",28,49,C_FG_WHITE);
	}
}

unsigned int hay_ganador()
{
	return (jugadores[0].puntos == 10 || jugadores[1].puntos == 10) || (jugadores[0].ciclos == 0 || jugadores[1].ciclos == 0);
}

unsigned int sin_zombis()
{
	return (jugadores[0].z_disponibles == 0 && jugadores[0].z_en_juego == 0) || (jugadores[1].z_disponibles == 0 && jugadores[1].z_en_juego == 0);
}
