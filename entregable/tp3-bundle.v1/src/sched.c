/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


int j_actual;		// turno jugador (0/1)


void iniciar_sched()
{

	players();
	
	unsigned int a1 = altura_player(0); 
	unsigned int a2 = altura_player(1);
	print("M",0,a1+1,C_FG_WHITE);
	print("M",79,a2+1,C_FG_WHITE);

	print("debug:off",22,49,C_FG_WHITE);
	j_actual = 0;

 //completar si falta mas codigo
}

unsigned short sched_proximo_indice() {
 	
 	unsigned short next = 0x00;
 	if(!r_pause() && !hay_ganador() && !sin_zombis())
 	{
 		if(j_actual == 0)
 		{
 			next = prox_Z_tarea(1) + 0x00;
 			j_actual = 1;
 		}
 		else if(j_actual == 1)
 		{
 			next = prox_Z_tarea(0) + 0x00;
 			j_actual = 0;
 		}
 	}
 	return next;
}




