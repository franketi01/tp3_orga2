/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

ca screen_debug[35][30];

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}


void print_black_screen()
{
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i,j;

    for (i = 0; i < VIDEO_FILS; i++)
    {
        for (j = 0; j < VIDEO_COLS; j++)
        {
            ca* px = &p[i][j];
            px->c = C_BG_BLACK | C_FG_BLACK;
            px->a = C_BG_BLACK | C_FG_BLACK;
        }
    }
}

void print_game_screen()
{   ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
  print("franloscarmircoko",0,0,C_FG_WHITE);  
  int i, j;
    for (i = 1; i < VIDEO_FILS; ++i)
    {
        for (j = 0; j < VIDEO_COLS; ++j)
        {
        	ca* px = &p[i][j];
            if(i > 44)
            {
            	if(j >= 35 && j <= 44)
            	{
            		if(j <= 39)
            		{
            		//	ca* px = &p[i][j];
                		px->c = C_FG_RED | C_BG_RED;
                		px->a = C_BG_RED | C_FG_RED;
            		}
            		else
            		{
            		//	ca* px = &p[i][j];
                		px->c = C_FG_BLUE | C_BG_BLUE;
                		px->a = C_BG_BLUE | C_FG_BLUE;
            		}
            	}
            	else
            	{
            	//	ca* px = &p[i][j];
                	px->c = C_FG_BLACK | C_BG_BLACK;
                	px->a = C_BG_BLACK | C_FG_BLACK;
            	}
                
            }
            else
            {
                if(j > 0 && j < 79)
                {
               //     ca* px = &p[i][j];
                    px->c = C_BG_GREEN | C_FG_GREEN;
                    px->a = C_BG_GREEN | C_FG_GREEN;
                }
                else
                {
                    if(j ==0)
                    {
                //        ca* px = &p[i][j];
                        px->c = C_BG_RED | C_FG_RED;
                        px->a = C_BG_RED | C_FG_RED;
                    }
                    else
                    {
                 //       ca* px = &p[i][j];
                        px->c = C_BG_BLUE | C_FG_BLUE;
                        px->a = C_BG_BLUE | C_FG_BLUE;
                    }
                    
                }
            }        
        }
    }

    for (i = 0; i < 8; i++)
    {
        print_int(i+1,4+2*i,46,C_FG_WHITE);
        print("-",4+2*i,48,C_FG_WHITE);
        print_int(i+1,60+2*i,46,C_FG_WHITE);
        print("-",60+2*i,48,C_FG_WHITE);
    }
}

void print_debug(char c, int jug, int eax, int ebx, int ecx, int edx,int st5,int st4,int st3,int st2,int st1,int esi, int edi, int ebp, int esp, int eip, short cs, short ds, short es, short fs, short gs, short ss, int flags, int cr3, int cr0, int cr2, int cr4)
{
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i,j;

    for (i = 0; i < 35; i++)
    {
        for (j = 0; j < 30; j++)
        {
            ca* px = &p[i+7][j+25];
            screen_debug[i][j].c = px->c;
            screen_debug[i][j].a = px->a;
        }
    }

    for (i = 7; i < 42; i++)
    {
        for (j = 25; j < 55; j++)
        {
            ca* px = &p[i][j];
            
            if(i == 7 || i == 41 || j == 54 || j == 25)
            {
                px->c = C_FG_BLACK | C_BG_BLACK;
                px->a = C_FG_BLACK | C_BG_BLACK;
            }
            else if(i == 8)
            {
                px->c = C_FG_BLUE | C_BG_BLUE;
                px->a = C_FG_BLUE | C_BG_BLUE;
            }
            else
            {
                px->c = C_BG_LIGHT_GREY | C_FG_LIGHT_GREY;
                px->a = C_BG_LIGHT_GREY | C_FG_LIGHT_GREY;
            }
        }
    }
    if(jug == -1 && c == 0)
    {
        print("Tarea idle",26,8,C_BG_BLUE | C_FG_WHITE);
    }
    else
    {
        print("Zombi",26,8,C_BG_BLUE | C_FG_WHITE);
        if(jug == 0)
        {
            print("A",32,8,C_BG_BLUE | C_FG_WHITE);
        }
        else if(jug == 1)
        {
            print("B",32,8,C_BG_BLUE | C_FG_WHITE);
        }
        if(c == 'M')
        {
            print("Mago",34,8,C_BG_BLUE | C_FG_WHITE);
        }
        else if(c == 'C')
        {
            print("Clerigo",34,8,C_BG_BLUE | C_FG_WHITE);
        }
        else if(c == 'G')
        {
            print("Guerrero",34,8,C_BG_BLUE | C_FG_WHITE);
        }

    }

    print("eax",27,10,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(eax,8,31,10,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("ebx",27,12,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(ebx,8,31,12,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("ecx",27,14,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(ecx,8,31,14,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("edx",27,16,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(edx,8,31,16,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("esi",27,18,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(esi,8,31,18,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("edi",27,20,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(edi,8,31,20,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("ebp",27,22,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(ebp,8,31,22,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("esp",27,24,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(esp,8,31,24,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("eip",27,26,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(eip,8,31,26,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("cs",28,28,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(cs,4,31,28,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("ds",28,30,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(ds,4,31,30,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("es",28,32,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(es,4,31,32,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("fs",28,34,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(fs,4,31,34,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("gs",28,36,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(gs,4,31,36,C_FG_WHITE | C_BG_LIGHT_GREY); 
    print("ss",28,38,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(ss,4,31,38,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("eeflags",28,40,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(flags,8,36,40,C_FG_WHITE | C_BG_LIGHT_GREY);

    print("cr0",41,10,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(cr0,8,45,10,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("cr2",41,12,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(cr2,8,45,12,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("cr3",41,14,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(cr3,8,45,14,C_FG_WHITE | C_BG_LIGHT_GREY);
    print("cr4",41,16,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(cr4,8,45,16,C_FG_WHITE | C_BG_LIGHT_GREY);

 //   unsigned int* dir_stack = (unsigned int*) ebp; 
    print("stack",41,27,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(st1,8,41,30,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(st2,8,41,31,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(st3,8,41,32,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(st4,8,41,33,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(st5,8,41,34,C_FG_WHITE | C_BG_LIGHT_GREY);
}

void print_back()
{
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i,j;

    for (i = 0; i < 35; i++)
    {
        for (j = 0; j < 30; j++)
        {
            ca* px = &p[i+7][j+25];
            px->c = screen_debug[i][j].c;
            px->a = screen_debug[i][j].a;
        }
    }   
}

void print_ade(char tipo, int x, int y, int jug)
{
    char str[] ={tipo,0};
    if(jug == 0)
    {
        print(str,x+2,y+1,C_FG_WHITE);
        print("x",x+1,y+1,C_FG_WHITE | C_BG_GREEN);
    }
    else
    {
        print(str,x,y+1,C_FG_WHITE);
        print("x",x+1,y+1,C_FG_WHITE | C_BG_GREEN);
    }

}

void print_der(char tipo, int x, int y, int jug)
{   
    char str[] = {tipo,0};
    if(jug == 0)
    {
        print(str,x+1,y+1,C_FG_WHITE);
        if(y==0)
        {
            print("x",x+1,44,C_FG_WHITE | C_BG_GREEN);
        }
        else
        {
            print("x",x+1,y,C_FG_WHITE | C_BG_GREEN);
        }
    }
    else
    {
        print(str,x+1,y+1,C_FG_WHITE);
        if(y == 43)
        {
            print("x",x+1,1,C_FG_WHITE | C_BG_GREEN);
        }
        else
        {
            print("x",x+1,y+2,C_FG_WHITE | C_BG_GREEN);
        }
    }

}


void print_reloj(int i_tarea, char tick, int jug)
{
    int x;
    unsigned short atr;
    if(jug == 0)
    {
        x = 4+2*i_tarea;
        atr = C_FG_RED;
    }
    else
    {
        x = 60+2*i_tarea;
        atr = C_FG_LIGHT_BLUE;
    }

    switch(tick)
    {
        case 0:
            print("-",x,48,C_FG_WHITE);
            break;
        case 1:
            print("\\",x,48,C_FG_WHITE);
            break;
        case 2:
            print("|",x,48,C_FG_WHITE);
            break;
        case 3:
            print("/",x,48,C_FG_WHITE);
            break;
        default:
            print("x",x,48,atr);
            break;
    }
}

void print_ganador(int puntos_jA, int puntos_jB)
{   
    int i,j;
    for(i = 2; i < 6; i++)
    {
        for(j = 29; j < 50; j++)
        {
            print(" ",j,i,C_BG_MAGENTA | C_FG_MAGENTA);
        }
    }
    if(puntos_jA == puntos_jB)
    {
        print("Es un empate",31,3,C_BG_MAGENTA);;
    }
    else if(puntos_jA > puntos_jB)
    {
        print("Ganador: Jugador A",31,3,C_BG_MAGENTA);
    }
    else
    {
        print("Ganador: Jugador B",31,3,C_BG_MAGENTA);
    }
}

void print_intr_line()
{
    int i;
    for (i = 20; i < 44; ++i)
    {
        print(" ",i,0,C_BG_BLACK);    
    }
}

