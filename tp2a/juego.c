#include <stdio.h>
#include "batalla.h"


int main(){
	juego_t juego;
	personaje_t personaje;
	inicializar_juego( &juego);
	pedir_datos( &juego, &(personaje));
 	return 0;
}










//gcc juego.c batalla.c perfil.c -o juego -std=c99 -Wall -Wconversion -Werror -lm
