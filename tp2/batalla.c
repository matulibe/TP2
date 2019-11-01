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


 void preguntar_usuario(juego_t* juego, personaje_t* personaje, char *ingresar){
  printf("Desea posicionar un personaje? S/N\n");
  scanf(" %c", &(*ingresar));
  if(*ingresar == 'S'){
    *ingresar = 'S';
  }else if(*ingresar == 'N'){
    *ingresar = 'N';
  }else{
      preguntar_usuario(juego, personaje, &(*ingresar));
  }
}


void agregar_rohan(juego_t* juego, personaje_t* personaje){
  printf("Que personaje queres agregar?\n");
  scanf(" %c", &(personaje->codigo));
  if((personaje->codigo) == HOMBRES){
    printf("Donde lo deseas posicionar?\n");
    printf("Columna: ");
    scanf("%i", &(personaje->columna));
    personaje->fila = FILA_HOMBRES;
    if(personaje->columna <= 9 && personaje->columna >= 0){
      juego->terreno[personaje->fila][personaje->columna] = HOMBRES;
      juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
      juego->cantidad_rohan++;
    }else{
      while(personaje->columna < 0 || personaje->columna > 9) {
        printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
        printf("Donde lo deseas posicionar?\n");
        printf("Columna: ");
        scanf("%i", &(personaje->columna));
        personaje->fila = FILA_HOMBRES;
      }
      juego->terreno[personaje->fila][personaje->columna] = HOMBRES;
      juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
      juego->cantidad_rohan++;
    }
  }else if((personaje->codigo) == ELFOS){
    printf("Donde lo deseas posicionar?\n");
    printf("Columna: ");
    scanf("%i", &(personaje->columna));
    printf("Fila: ");
    scanf("%i", &(personaje->fila));
    while(!esta_vacio(juego->terreno[personaje->fila][personaje->columna]) || ((personaje->fila) < 5) || ((personaje->fila) > 8) || (personaje->columna < 0 || personaje->columna > 9)){
      printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
      printf("Donde lo deseas posicionar?\n");
      printf("Columna: ");
      scanf("%i", &(personaje->columna));
      printf("Fila: ");
      scanf("%i", &(personaje->fila));
    }
    juego->terreno[personaje->fila][personaje->columna] = ELFOS;
    juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
    juego->cantidad_rohan++;
    if(((personaje->fila) < 9) && ((personaje->fila) > 4) && (personaje->columna <= 9 && personaje->columna >= 0)){
      juego->terreno[personaje->fila][personaje->columna] = ELFOS;
      juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
      juego->cantidad_rohan++;
    }
  }else{
    printf("Valor no valido. Reingrese otro.\n");
    agregar_rohan(juego, personaje);
  }
}


void agregar_isengard(juego_t* juego, personaje_t* personaje){
  printf("Que personaje queres agregar?\n");
  scanf(" %c", &(personaje->codigo));
  if((personaje->codigo) == ORCOS){
    printf("Donde lo deseas posicionar?\n");
    printf("Columna: ");
    scanf("%i", &(personaje->columna));
    personaje->fila = 0;
    if(personaje->columna <= 9 && personaje->columna >= 0){
      juego->terreno[personaje->fila][personaje->columna] = ORCOS;
      juego->isengard  [juego->cantidad_isengard ] = personaje_movil_nuevo(ORCOS, juego->plus_isengard  , personaje->columna, personaje->fila);
      juego->cantidad_isengard++;
    }else{
      while(personaje->columna < 0 || personaje->columna > 9) {
        printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
        printf("Donde lo deseas posicionar?\n");
        printf("Columna: ");
        scanf("%i", &(personaje->columna));
        personaje->fila = ORCOS;
      }
      juego->terreno[personaje->fila][personaje->columna] = ORCOS;
      juego->isengard  [juego->cantidad_isengard ] = personaje_movil_nuevo(ORCOS, juego->plus_isengard  , personaje->columna, personaje->fila);
      juego->cantidad_isengard++;
    }
  }else if((personaje->codigo) == URUKHAI){
    printf("Donde lo deseas posicionar?\n");
    printf("Columna: ");
    scanf("%i", &(personaje->columna));
    printf("Fila: ");
    scanf("%i", &(personaje->fila));
    while(!esta_vacio(juego->terreno[personaje->fila][personaje->columna]) || ((personaje->fila) < 1) || ((personaje->fila) > 4) || (personaje->columna < 0 || personaje->columna > 9)){
      printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
      printf("Donde lo deseas posicionar?\n");
      printf("Columna: ");
      scanf("%i", &(personaje->columna));
      printf("Fila: ");
      scanf("%i", &(personaje->fila));
    }
    juego->terreno[personaje->fila][personaje->columna] = URUKHAI;
    juego->isengard  [juego->cantidad_isengard ] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard  , personaje->columna, personaje->fila);
    juego->cantidad_isengard++;
    if(((personaje->fila) < 9) && ((personaje->fila) > 4) && (personaje->columna <= 9 && personaje->columna >= 0)){
      juego->terreno[personaje->fila][personaje->columna] = URUKHAI;
      juego->isengard  [juego->cantidad_isengard ] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard  , personaje->columna, personaje->fila);
      juego->cantidad_isengard++;
    }
  }else{
    printf("Valor no valido. Reingrese otro.\n");
    agregar_isengard(juego, personaje);
  }
}


void jugador_maquina(juego_t* juego, personaje_t personaje){
  int numero = rand () % 10;
  if(juego->jugador2.tipo == BANDO_DEFENSIVO){
    personaje->fila = FILA_HOMBRES;
    personaje->columna = numero;
    juego->terreno[personaje->fila][personaje->columna] = HOMBRES;
    juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
    juego->cantidad_rohan++;
  }else{
    personaje->fila = 0;
    personaje->columna = numero;
    juego->terreno[personaje->fila][personaje->columna] = HOMBRES;
    juego->isengard[juego->cantidad_isengard] = personaje_movil_nuevo(HOMBRES, juego->plus_isengard, personaje->columna, personaje->fila);
    juego->cantidad_isengard++;
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


void rango_inmovil(personaje_t personaje, personaje_t* enemigo, juego_t juego){
  int distancia = (abs(personaje.fila - enemigo->fila) + abs(personaje.columna - enemigo->columna));
  if(personaje.codigo == ELFOS){
    for(int i = 0; i < juego.cantidad_isengard; i++){
      if(distancia <= 3){
        enemigo->vida - (ATAQUE_INMOVILES + plus_rohan);
      }
    }
  }else if(personaje.codigo == URUKHAI){
    for(int i = 0; i < juego.cantidad_rohan; i++){
      if(distancia <= 3){
        enemigo->vida - (ATAQUE_INMOVILES + plus_isengard);
      }
}

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
  char ingresar;
  preguntar_usuario(juego, personaje, &ingresar);
  if(ingresar == 'S'){
    if(juego->jugador1.tipo == BANDO_DEFENSIVO){
      agregar_rohan(juego, personaje);
    }else{
      agregar_isengard(juego, personaje);
    }
  }else if(ingresar == 'N'){
    return;
  }else{
    printf("Respuesta no valida. Por favor ingrese un valor correcto\n");
    pedir_datos(juego, personaje);
  }
  imprimir_terreno(juego);
}



/*void posicionar_personaje(juego_t* juego, personaje_t personaje){
  imprimir_terreno( juego);
  }
*/




//gcc  batalla.c  -o batalla - std = c99 - Wall - Wconversion - Werror - lm
