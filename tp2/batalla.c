#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "perfil.h"
#include "batalla.h"



const int VIDA_INMOVILES = 200;
const int VIDA_MOVILES = 100;
const int ATAQUE_INMOVILES = 10;
const int ATAQUE_MOVILES = 50;
const int ENERGIA_MAX = 10;
const int ENERGIA_INICIAL = 5;
const int ENERGIA_MOVILES = 3;
const int ENERGIA_INMOVILES = 8;
const int CANTIDAD_MAX_JUGADORES = 2;
const int COSTO_INMOVILES = 8;
const int COSTO_MOVILES = 3;
const int CANTIDAD_DE_LLEGADAS = 5;
const int INTENSIDAD_MAX = 10;
const int FILA_HOMBRES = 9;

const char HOMBRES = 'H';
const char ELFOS = 'E';
const char URUKHAI = 'U';
const char ORCOS = 'O';

//Inicializa el struct de personaje.
void limpiar_personajes(personaje_t *personaje){
  personaje->vida = 0;
  personaje->ataque = 0;
  personaje->fila = 0;
  personaje->columna = 0;
  personaje->esta_vivo = false;
}

//Inicializa el struct de juego.
void limpiar_juego(juego_t *juego){
   juego->cantidad_rohan = 0;
	 juego->llegadas_rohan = 0;
	 juego->plus_rohan = 0;
   juego->cantidad_isengard = 0;
 	 juego->llegadas_isengard = 0;
 	 juego->plus_isengard = 0;
   juego->jugador1.energia = 5;
   juego->jugador2.energia = 5;
}

/*
*Tomara la intensidad de perfil y le multiplicara un numero entre 0 y 5
*para modificar la vida y el ataque
*/
void definir_plus_rohan(juego_t *juego, int intensidad, char tipo){
    int plus;
    int numero = rand () % 5;
    plus = intensidad * numero;
    juego->plus_rohan = plus;

}

/*
*Tomara la intensidad de perfil y le multiplicara un numero entre 0 y 5
*para modificar la vida y el ataque.
*/
void definir_plus_isengard(juego_t *juego, int intensidad, char tipo){
    int plus;
    int numero = rand () % 5;
    plus = intensidad * numero;
        juego->plus_isengard = plus;
}

//Forma la matriz del terreno del juego.
void limpiar_terreno(juego_t* juego){
  for(int i = 0; i < MAX_TERRENO_FIL; i++){
      for(int j = 0; j < MAX_TERRENO_COL; j++){
          juego->terreno[i][j] = ' ';
      }
  }
}

//Imprime la matriz y sus bordes.
void imprimir_terreno(juego_t* juego){

    printf("+ ");
    for(int i = 1; i<MAX_TERRENO_COL+1; i++){
        printf("= ");
    }
    printf("+ \n");


    for(int i = 0; i < MAX_TERRENO_FIL; i++){
        printf("| ");
        for(int j = 0; j < MAX_TERRENO_COL; j++){
            printf(" %c", juego->terreno[i][j] );
        }

        printf("| \n");
    }

    printf("+ ");
    for(int i = 1; i<MAX_TERRENO_COL+1; i++){
        printf("= ");
    }
    printf("+ \n");
}

//Guarda los datos de los personajes inmoviles para cargarlos a su vector correspondiente.
personaje_t personaje_inmovil_nuevo(char codigo, int plus, int x, int y){
  personaje_t personaje;
  personaje.vida = VIDA_INMOVILES - plus;
  personaje.ataque = ATAQUE_INMOVILES + plus;
  personaje.codigo = codigo;
  personaje.fila = y;
  personaje.columna = x;
  personaje.esta_vivo = true;
  return personaje;

}

//Guarda los datos de los personajes moviles para cargarlos a su vector correspondiente.
personaje_t personaje_movil_nuevo(char codigo, int plus, int x, int y){
  personaje_t personaje;
  personaje.vida = VIDA_MOVILES - plus;
  personaje.ataque = ATAQUE_MOVILES + plus;
  personaje.codigo = codigo;
  personaje.fila = y;
  personaje.columna = x;
  personaje.esta_vivo = true;
  return personaje;

}

//Chequea que la poscion del terreno este vacia.
bool esta_vacio(char codigo){
  return (codigo == ' ');
}

//Asigna una posicion del terreno a los primeros 3 elfos del juego, para madarlo al vector de rohan.
void agregar_elfos(juego_t* juego){
  int x;
  int y;
  for(int i=0; i < 3; i++){
    do{
      x = rand () % 10;
      y = rand () % 4 + 5;
    } while(!esta_vacio(juego->terreno[y][x]));
    (*juego).terreno[y][x] = ELFOS;
    juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, x, y);
    juego->cantidad_rohan++;
  }
}

//Asigna una posicion del terreno a los primeros 3 urukhai del juego y lo al vector de isengard.
void agregar_urukhai(juego_t* juego){
    int x;
    int y;
    for(int i=0; i < 3; i++){
      do{
        x = rand () % 10;
        y = rand () % 4 + 1;
      } while(!esta_vacio(juego->terreno[y][x]));
      juego->terreno[y][x] = URUKHAI;
      juego->isengard[juego->cantidad_isengard] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard, x, y);
      juego->cantidad_isengard++;
    }
}

//Pide al jugador defensivo que personaje quiere agregar y en que posicion, para madarlo al vector de rohan.
void preguntar_rohan(juego_t* juego, personaje_t* personaje){
  char ingresar;
  printf("Desea posicionar un personaje? S/N\n");
  scanf(" %c", &ingresar);
  if(ingresar == 'S'){
    printf("Que personaje queres posicionar?\n");
    scanf(" %c", &(personaje->codigo));
    if((personaje->codigo == ELFOS) && ((juego->jugador1.energia >= ENERGIA_MOVILES) || (juego->jugador2.energia >= ENERGIA_MOVILES))){
      printf("Donde lo queres posicionar?\n");
      printf("Columna:\n");
      scanf(" %i", &(personaje->columna));
      printf("Fila:\n");
      scanf(" %i", &(personaje->fila));
      while(!esta_vacio(juego->terreno[personaje->fila][personaje->columna]) || ((personaje->fila) < 5) || ((personaje->fila) > 8)){
        printf("Posicion no valida. Por favor ingrese otra.\n");
        printf("Columna:\n");
        scanf(" %i", &(personaje->columna));
        printf("Fila:\n");
        scanf(" %i", &(personaje->fila));
      }
        juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
        juego->cantidad_rohan++;

      if(((personaje->fila > 4) && (personaje->fila < 9)) && ((personaje->columna >= 0) && (personaje->fila <= 9))){
        juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
        juego->cantidad_rohan++;
      }
      if(juego->jugador1.tipo == BANDO_DEFENSIVO){
        juego->jugador1.energia = juego->jugador1.energia - ENERGIA_INMOVILES;
      }else if(juego->jugador2.tipo == BANDO_DEFENSIVO){
        juego->jugador2.energia = juego->jugador2.energia - ENERGIA_INMOVILES;
      }

    }else if((personaje->codigo == HOMBRES) && ((juego->jugador1.energia >= ENERGIA_MOVILES) || (juego->jugador2.energia >= ENERGIA_MOVILES))){
      personaje->fila = FILA_HOMBRES;
      printf("Donde lo queres posicionar?\n");
      printf("Columna:\n");
      scanf(" %i", &(personaje->columna));
      juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
      juego->cantidad_rohan++;
      if(juego->jugador1.tipo == BANDO_DEFENSIVO){
        juego->jugador1.energia = juego->jugador1.energia - ENERGIA_MOVILES;
      }else if(juego->jugador2.tipo == BANDO_DEFENSIVO){
        juego->jugador2.energia = juego->jugador2.energia - ENERGIA_MOVILES;
      }
    }else{
      printf("No se puede ingresar ese personaje. Por favor ingrese otro.\n");
      preguntar_rohan( juego, personaje);
    }
  }else{
    return;
  }
}

//Pide al jugador defensivo que personaje quiere agregar y en que posicion y lo manda al vector de isengard.
void preguntar_isengard(juego_t* juego, personaje_t* personaje){
  char ingresar;
  printf("Desea posicionar un personaje? S/N\n");
  scanf(" %c", &ingresar);
  if(ingresar == 'S'){
    printf("Que personaje queres posicionar?\n");
    scanf(" %c", &(personaje->codigo));
    if((personaje->codigo == URUKHAI) && ((juego->jugador1.energia >= ENERGIA_INMOVILES) || (juego->jugador2.energia >= ENERGIA_INMOVILES))){
      printf("Donde lo queres posicionar?\n");
      printf("Columna:\n");
      scanf(" %i", &(personaje->columna));
      printf("Fila:\n");
      scanf(" %i", &(personaje->fila));
      while(!esta_vacio(juego->terreno[personaje->fila][personaje->columna]) || ((personaje->fila) < 1) || ((personaje->fila) > 4)){
        printf("Posicion no valida. Por favor ingrese otra.\n");
        printf("Columna:\n");
        scanf(" %i", &(personaje->columna));
        printf("Fila:\n");
        scanf(" %i", &(personaje->fila));
      }
        juego->rohan[juego->cantidad_isengard] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard, personaje->columna, personaje->fila);
        juego->cantidad_isengard++;

      if(((personaje->fila > 0) && (personaje->fila < 5)) && ((personaje->columna >= 1) && (personaje->fila <= 4))){
        juego->isengard[juego->cantidad_isengard] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard, personaje->columna, personaje->fila);
        juego->cantidad_isengard++;
      }
      if(juego->jugador1.tipo == BANDO_OFENSIVO){
        juego->jugador1.energia = juego->jugador1.energia - ENERGIA_INMOVILES;
      }else if(juego->jugador2.tipo == BANDO_OFENSIVO){
        juego->jugador2.energia = juego->jugador2.energia - ENERGIA_INMOVILES;
      }

    }else if((personaje->codigo == ORCOS) && ((juego->jugador1.energia >= ENERGIA_MOVILES) || (juego->jugador2.energia >= ENERGIA_MOVILES))){
      personaje->fila = 0;
      printf("Donde lo queres posicionar?\n");
      printf("Columna:\n");
      scanf(" %i", &(personaje->columna));
      juego->isengard[juego->cantidad_isengard] = personaje_movil_nuevo(ORCOS, juego->plus_isengard, personaje->columna, personaje->fila);
      juego->cantidad_isengard++;
      if(juego->jugador1.tipo == BANDO_OFENSIVO){
        juego->jugador1.energia = juego->jugador1.energia - ENERGIA_MOVILES;
      }else if(juego->jugador2.tipo == BANDO_OFENSIVO){
        juego->jugador2.energia = juego->jugador2.energia - ENERGIA_MOVILES;
      }
    }else{
      printf("No se puede ingresar ese personaje. Por favor ingrese otro.\n");
      preguntar_isengard( juego, personaje);
    }
  }else{
    return;
  }
}

//Le asigna el bando opuesto del jugador 1 al jugador 2.
char tipo_opuesto(char tipo){
  if (tipo == BANDO_OFENSIVO)
    return BANDO_DEFENSIVO;
  return BANDO_OFENSIVO;
}

//Aumenta en 1 la energia de jugador 1.
void energia_j1(juego_t* juego){
  if(juego->jugador1.energia < ENERGIA_MAX){
    juego->jugador1.energia++;
  }else{
    juego->jugador1.energia = juego->jugador1.energia;
  }

}

//Aumenta en 1 la energia de jugador 2.
void energia_j2(juego_t* juego){
  if(juego->jugador2.energia < ENERGIA_MAX){
    juego->jugador2.energia++;
  }else{
    juego->jugador2.energia = juego->jugador2.energia;
  }

}


/*bool rango_elfo(juego_t* juego, personaje_t personaje){
  int x;
  int y;
  int i;
  int j;
  if((juego->terreno[x][y] = ELFOS && juego->terreno[i][j] = URUKHAI) || (juego->terreno[x][y] = ELFOS && juego->terreno[i][j] = ORCOS)){
    while(((x - i) + (y - j)) == 2 || ((x - i) + (y - j)) == -2){
      return true;
    }
  }else{
    return false;
  }
}


bool rango_urukhai(juego_t* juego, personaje_t personaje){
  int x;
  int y;
  int i;
  int j;
  if((juego->terreno[x][y] = URUKHAI && juego->terreno[i][j] = ELFOS) || (juego->terreno[x][y] = URUKHAI && juego->terreno[i][j] = HOMBRES)){
    while(((x - i) + (y - j)) == 2 || ((x - i) + (y - j)) == -2){
      return true;
    }
  }else{
    return false;
  }
}


void ataque_inmoviles(juego_t* juego, personaje_t personaje){
  int x;
  int y;
  if((juego->terreno[x][y] = ELFOS) || (juego->terreno[x][y] = URUKHAI)){
    while()
  }
}
*/

void inicializar_juego(juego_t* juego){
    int intensidad;
    char tipo;
    srand (( unsigned ) time ( NULL ));
    perfil(&tipo, &intensidad);
    (juego->jugador1.tipo) = tipo;
    (juego->jugador2.tipo) = tipo_opuesto(tipo);
    limpiar_juego( juego);
    limpiar_terreno(juego);
    definir_plus_rohan( juego, intensidad, tipo);
    definir_plus_isengard( juego, intensidad, tipo);
    agregar_elfos( juego);
    agregar_urukhai( juego);
    imprimir_terreno( juego);
}


void pedir_datos(juego_t* juego, personaje_t* personaje){
  if(juego->jugador1.tipo == BANDO_DEFENSIVO){
    preguntar_rohan(juego, personaje);
  }else if(juego->jugador2.tipo == BANDO_OFENSIVO){
    preguntar_isengard(juego, personaje);
  }
  if(juego->jugador2.tipo == BANDO_OFENSIVO){
    preguntar_isengard(juego, personaje);
  }else if(juego->jugador2.tipo == BANDO_DEFENSIVO){
    preguntar_rohan(juego, personaje);
  }
}


void posicionar_personaje(juego_t* juego, personaje_t personaje){
  imprimir_terreno( juego);
  }





//gcc  batalla.c  -o batalla - std = c99 - Wall - Wconversion - Werror - lm
