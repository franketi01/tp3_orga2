/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

#define PDE_INDEX(virtual) (virtual >> 22)
#define PTE_INDEX(virtual) ((virtual << 10) >> 22)

unsigned int sgte_pagina_libre = (unsigned int) inicio_paginas_libres;

void mmu_inicializar() {
	
}

unsigned int get_next_free_page(){
	unsigned int res = sgte_pagina_libre;
	sgte_pagina_libre += size_tabla;
	return res;
}

unsigned int codigo_tarea(char tipo, int jug)
{
	unsigned int code;
	int t;
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
	if(jug == 0)
	{
		code = 0x10000;
	}
	else{
		code = 0x13000;
	}
	return code + t*0X1000;
}


void mmu_inicializar_dir_kernel(){	

	dir_entry* pd = (dir_entry*) dir_directory;
//	tab_entry* pt = (tab_entry*) dir_table;

	pd[0].dir_base = dir_table >> 12;
	pd[0].disp = 0;
	pd[0].g = 0;
	pd[0].ps = 0;
	pd[0].cero = 0;
	pd[0].a = 0;
	pd[0].pcd = 0;
	pd[0].pwt = 0;
	pd[0].us = 0;
	pd[0].rw = 1;
	pd[0].p = 1;

	int j;
	//ponemos elmresto de las entradas del page dirctory como NO presente
	for(j = 1; j < 1024; j++)
	{
		pd[j].dir_base = 0;
		pd[j].disp = 0;
		pd[j].g = 0;
		pd[j].ps = 0;
		pd[j].cero = 0;
		pd[j].a = 0;
		pd[j].pcd = 0;
		pd[j].pwt = 0;
		pd[j].us = 0;
		pd[j].rw = 1;
		pd[j].p = 0;
	}
	
	int i = 0;

	unsigned int dir_fisica = 0;  
	for(i = 0; i<1024 ; i++){
		mapear_pagina(dir_fisica, (unsigned int) dir_directory, dir_fisica, 1);
	/*	pt[i].dir_base = i;
		pt[i].disp = 0;
		pt[i].g = 0;
		pt[i].pat = 0;
		pt[i].d = 0;
		pt[i].a = 0;
		pt[i].pcd = 0;
		pt[i].pwt = 0;
		pt[i].us = 0;
		pt[i].rw = 1;
		pt[i].p = 1;*/
		dir_fisica += 0x1000;
	}

	
	tlbflush();
}


unsigned int mmu_inicializar_dir_zombi(int tipo_zombi, int player, int x, int y){
//	unsigned int page_virtual = comienzo_mem_virtual_user;
//	unsigned int cr3_user = get_next_free_page();
//	mapear_pagina(page_virtual, cr3_user )
	unsigned int page = get_next_free_page();
	dir_entry* dir_pag = (dir_entry*) page;
	unsigned int tab = get_next_free_page();
	tab_entry* dir_tab = (tab_entry*) tab;

	dir_pag[0].dir_base = tab >> 12;
	dir_pag[0].disp = 0;
	dir_pag[0].g = 0;
	dir_pag[0].ps = 0;
	dir_pag[0].cero = 0;
	dir_pag[0].a = 0;
	dir_pag[0].pcd = 0;
	dir_pag[0].pwt = 0;
	dir_pag[0].us = 1;
	dir_pag[0].rw = 1;
	dir_pag[0].p = 1;

	unsigned int j;
	//ponemos elmresto de las entradas del page dirctory como NO presente
	for(j = 1; j < 1024; j++)
	{
		dir_pag[j].dir_base = 0;
		dir_pag[j].disp = 0;
		dir_pag[j].g = 0;
		dir_pag[j].ps = 0;
		dir_pag[j].cero = 0;
		dir_pag[j].a = 0;
		dir_pag[j].pcd = 0;
		dir_pag[j].pwt = 0;
		dir_pag[j].us = 1;
		dir_pag[j].rw = 1;
		dir_pag[j].p = 0;
	}

	unsigned int i;
	for (i = 0; i < 1024; i++)
	{
		dir_tab[i].dir_base = i;
		dir_tab[i].disp = 0;
		dir_tab[i].g = 0;
		dir_tab[i].pat = 0;
		dir_tab[i].d = 0;
		dir_tab[i].a = 0;
		dir_tab[i].pcd = 0;
		dir_tab[i].pwt = 0;
		dir_tab[i].us = 1;
		dir_tab[i].rw = 1;
		dir_tab[i].p = 1;
	}
	// breakpoint();
	mapear_pagina_a_mapa(x,y,page,player, tipo_zombi);
	//copiar_codigo_a_mapa(tipo_zombi, player);
	tlbflush();

	return page;
}

void mapear_pagina_a_mapa(int x, int y, unsigned int page, int jugador, int tipo_zombi ){
	//posicion de la pagina 1
	unsigned int pos_mapa1 = 0x400000 + y*0x4E000 + x*0x1000;	//pos1
	mapear_pagina(0x8000000, page, pos_mapa1, 1);


	unsigned int pos_mapa2;
	unsigned int pos_mapa3;
	unsigned int pos_mapa4;
	unsigned int pos_mapa5;
	unsigned int pos_mapa6;
	unsigned int pos_mapa7;
	unsigned int pos_mapa8;
	unsigned int pos_mapa9;
	
	switch(jugador){
		case 0:
				pos_mapa7 = 0x400000 + y*0x4E000 + (x-1)*0x1000;	//pos2
				pos_mapa2 = 0x400000 + y*0x4E000 + (x+1)*0x1000;	//pos7
				if (y==0){
					pos_mapa8 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
					pos_mapa6 = 0x400000 + 43*0x4E000 + (x)*0x1000;
					pos_mapa4 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
				}else{
					pos_mapa8 = 0x400000 + (y-1)*0x4E000 + (x-1)*0x1000;
					pos_mapa6 = 0x400000 + (y-1)*0x4E000 + (x)*0x1000;
					pos_mapa4 = 0x400000 + (y-1)*0x4E000 + (x+1)*0x1000;
				}

				if (y==43){
					pos_mapa9 = 0x400000 + (x-1)*0x1000;
					pos_mapa5 = 0x400000 + (x)*0x1000;
					pos_mapa3 = 0x400000 + (x+1)*0x1000;
				}else{
					pos_mapa9 = 0x400000 + (y+1)*0x4E000 + (x-1)*0x1000;
					pos_mapa5 = 0x400000 + (y+1)*0x4E000 + (x)*0x1000;
					pos_mapa3 = 0x400000 + (y+1)*0x4E000 + (x+1)*0x1000;
				}
				break;
		case 1:	
				pos_mapa2 = 0x400000 + y*0x4E000 + (x-1)*0x1000;	//pos2
				pos_mapa7 = 0x400000 + y*0x4E000 + (x+1)*0x1000;	//pos7
				if (y==0){
					pos_mapa3 = 0x400000 + 43*0x4E000 + (x-1)*0x1000;
					pos_mapa5 = 0x400000 + 43*0x4E000 + (x)*0x1000;
					pos_mapa9 = 0x400000 + 43*0x4E000 + (x+1)*0x1000;
				}else{
					pos_mapa3 = 0x400000 + (y-1)*0x4E000 + (x-1)*0x1000;
					pos_mapa5 = 0x400000 + (y-1)*0x4E000 + (x)*0x1000;
					pos_mapa9 = 0x400000 + (y-1)*0x4E000 + (x+1)*0x1000;
				}

				if (y==43){
					pos_mapa4 = 0x400000 + (x-1)*0x1000;
					pos_mapa6 = 0x400000 + (x)*0x1000;
					pos_mapa8 = 0x400000 + (x+1)*0x1000;
				}else{
					pos_mapa4 = 0x400000 + (y+1)*0x4E000 + (x-1)*0x1000;
					pos_mapa6 = 0x400000 + (y+1)*0x4E000 + (x)*0x1000;
					pos_mapa8 = 0x400000 + (y+1)*0x4E000 + (x+1)*0x1000;
				}
				break;
	}

	mapear_pagina(0x8001000, page, pos_mapa2, 1);
	mapear_pagina(0x8002000, page, pos_mapa3, 1);
	mapear_pagina(0x8003000, page, pos_mapa4, 1);
	mapear_pagina(0x8004000, page, pos_mapa5, 1);
	mapear_pagina(0x8005000, page, pos_mapa6, 1);
	mapear_pagina(0x8006000, page, pos_mapa7, 1);
	mapear_pagina(0x8007000, page, pos_mapa8, 1);
	mapear_pagina(0x8008000, page, pos_mapa9, 1);
	copiar_codigo_a_mapa(tipo_zombi, jugador, pos_mapa1);

//???	unmapear_pagina(0x8000000,page);
	tlbflush();

}

void copiar_codigo_a_mapa(int tipo_zombi, int player, unsigned int fisica){

	unsigned int * codigo_mapa = (unsigned int*) fisica;
	unsigned int * codigo;
	if (player == 0){
		codigo = (unsigned int*) (0x10000 + 0X1000 * tipo_zombi); //tipo_zombi: 0,1, 2 
	}else{
		codigo = (unsigned int*) (0x13000 + 0X1000 * tipo_zombi);
	}
	int i;
	//verificar que este la fisica mapeada

	// breakpoint();
	mapear_pagina(fisica, rcr3(), fisica ,1);
	for(i=0; i<1024 ; i++){
		codigo_mapa[i] = codigo[i];
	}
	// breakpoint();	
	unmapear_pagina(fisica, rcr3() );
	

	//desmapear la fisica
	

}

void mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, char us)
{//parametro n usuario/supervisor

	dir_entry* pd = (dir_entry*) cr3;
	tab_entry* pt;

	unsigned int index_pd =  (unsigned int) PDE_INDEX(virtual); 
	unsigned int index_pt = (unsigned int) PTE_INDEX(virtual);
	//seteamos los atributos:

	if (pd[index_pd].p == 0 ){

		unsigned int dir = (unsigned int) get_next_free_page();	
		pt = (tab_entry*) dir;
		int i;
		for (i = 0; i < 1024; ++i)
		{
			pt[i].dir_base = 0; 
			pt[i].disp = 0;
			pt[i].g = 0;
			pt[i].pat = 0;
			pt[i].d = 0;
			pt[i].a = 0;
			pt[i].pcd = 0;
			pt[i].pwt = 0;
			pt[i].us = 0;
			pt[i].rw = 0;
			pt[i].p = 0;
		}

		pd[index_pd].dir_base = dir >> 12;
		pd[index_pd].us = us;

		pd[index_pd].rw = 1;
		pd[index_pd].p = 1;

	}else{

		pt = (tab_entry*) (pd[index_pd].dir_base << 12);
		
	}
				
	pt[index_pt].dir_base = fisica >> 12; 
	pt[index_pt].disp = 0;
	pt[index_pt].g = 0;
	pt[index_pt].pat = 0;
	pt[index_pt].d = 0;
	pt[index_pt].a = 0;
	pt[index_pt].pcd = 0;
	pt[index_pt].pwt = 0;
	pt[index_pt].us = us;
	pt[index_pt].rw = 1;
	pt[index_pt].p = 1;
				
}

void unmapear_pagina(unsigned int virtual, unsigned int cr3)
{
	dir_entry* p_dir = (dir_entry*) cr3;
	unsigned int pd_index = (unsigned int) PDE_INDEX(virtual);
	unsigned int pt_index = (unsigned int) PTE_INDEX(virtual);

	dir_entry* dir_page = &p_dir[pd_index];

	tab_entry* p_tab = (tab_entry*) (dir_page->dir_base << 12);
	tab_entry* table = (tab_entry*) &p_tab[pt_index];
	table->dir_base = 0; 
	table->disp = 0;
	table->g = 0;
	table->pat = 0;
	table->d = 0;
	table->a = 0;
	table->pcd = 0;
	table->pwt = 0;
	table->us = 0;
	table->rw = 0;
	table->p = 0;
}

