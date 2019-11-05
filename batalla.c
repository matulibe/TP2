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
const char RESPUESTA_POSITIVA = 'S';
const char RESPUESTA_NEGATIVA = 'N';


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
   juego->cantidad_jugadores = 0;
   juego->jugador1.energia = ENERGIA_INICIAL;
   juego->jugador2.energia = ENERGIA_INICIAL;
}

/*
  **Tomara la intensidad de perfil y le multiplicara un numero entre 0 y 5
*para modificar la vida y el ataque
 */
void definir_plus_rohan(juego_t *juego, int intensidad, char tipo){
    int plus;
    int numero = rand () % 5;
    plus = intensidad * numero;
    juego->plus_rohan = plus;

}

/*
  **Tomara la intensidad de perfil y le multiplicara un numero entre 0 y 5
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
void elfos_iniciales(juego_t* juego){
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
void urukhai_iniciales(juego_t* juego){
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

/*
 * Le preguntara al usuario cuantos jugadores van a jugar
 */
void averiguar_cant_jugadores(int* cantidad){
  printf("Cuantos jugadores son? (1 o 2)\n");
  scanf("%i", cantidad);
  if(*cantidad > CANTIDAD_MAX_JUGADORES || *cantidad < 1){
    printf("Valorno valido. Ingrese 1 o 2.\n");
    averiguar_cant_jugadores( cantidad);
  }
}


//Se fiajra que judaor es defensivo y le asignara la enregia del jugador a la variable
int energia_defensivo(juego_t* juego){
  int energia;
  if(juego->jugador1.tipo == BANDO_DEFENSIVO){
    energia = juego->jugador1.energia;
  }else{
    energia = juego->jugador2.energia;
  }
  return energia;
}


//Se fiajra que judaor es ofensivo y le asignara la enregia del jugador a la variable
int energia_ofensivo(juego_t* juego){
  int energia;
  if(juego->jugador1.tipo == BANDO_OFENSIVO){
    energia = juego->jugador1.energia;
  }else{
    energia = juego->jugador2.energia;
  }
  return energia;
}


/*
 * Se fijara que jugador es ofensivo y que personaje ingreso.
 * Y depende el personaje le quitara la energia correspondiente
 */
void sacar_energia_ofensivo(juego_t* juego, char codigo){
  if(codigo == ORCOS){
    if(juego->jugador1.tipo == BANDO_OFENSIVO){
      juego->jugador1.energia -= ENERGIA_MOVILES;
    }else{
      juego->jugador2.energia -= ENERGIA_MOVILES;
    }
  }else{
    if(juego->jugador1.tipo == BANDO_OFENSIVO){
      juego->jugador1.energia -= ENERGIA_INMOVILES;
    }else{
      juego->jugador2.energia -= ENERGIA_INMOVILES;
    }
  }
}


/*
 * Se fijara que jugador es defensivo y que personaje ingreso.
 * Y depende el personaje le quitara la energia correspondiente
 */
void sacar_energia_defensivo(juego_t* juego, char codigo){
  if(codigo == HOMBRES){
    if(juego->jugador1.tipo == BANDO_DEFENSIVO){
      juego->jugador1.energia -= ENERGIA_MOVILES;
    }else{
      juego->jugador2.energia -= ENERGIA_MOVILES;
    }
  }else{
    if(juego->jugador1.tipo == BANDO_DEFENSIVO){
      juego->jugador1.energia -= ENERGIA_INMOVILES;
    }else{
      juego->jugador2.energia -= ENERGIA_INMOVILES;
    }
  }
}

/*
 * Le preguntara al usurio si quiere ingresar un personaje
 */
 void preguntar_usuario(char* respuesta){
  printf("Desea posicionar un personaje? S/N\n");
  scanf(" %c", &(*respuesta));
  if(*respuesta != RESPUESTA_POSITIVA && *respuesta != RESPUESTA_NEGATIVA){
    printf("Respuesta invalida\n");
    preguntar_usuario( respuesta);
  }
}

/*
 * Le pregunta al usuario que personaje quiere ingresar.
 */
void preguntar_personaje(char* soldado, char soldado_inmovil, char soldado_movil){
  printf("Que personaje desea agregar?\n");
  scanf(" %c", &(*soldado));
  if(*soldado != soldado_inmovil && *soldado != soldado_movil){
    printf("Respuesta invalida\n");
    preguntar_personaje( soldado, soldado_inmovil, soldado_movil);
  }
}


/*
 * En el caso que el usurio quiera ingresar un hombre le pedira los datos,
 * y enviara los datos al vector de rohan.
 */
void agregar_hombres(juego_t* juego, personaje_t* personaje){
  printf("Donde lo deseas posicionar?\n");
  printf("Columna: ");
  scanf("%i", &(personaje->columna));
  personaje->fila = FILA_HOMBRES;
  if(personaje->columna <= 9 && personaje->columna >= 0){
    juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
    juego->cantidad_rohan++;
    sacar_energia_defensivo( juego, HOMBRES);
  }else{
    while(personaje->columna < 0 || personaje->columna > 9) {
      printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
      printf("Donde lo deseas posicionar?\n");
      printf("Columna: ");
      scanf("%i", &(personaje->columna));
      personaje->fila = FILA_HOMBRES;
    }
    juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
    juego->cantidad_rohan++;
    sacar_energia_defensivo( juego, HOMBRES);
  }
}


/*
 * En el caso que el usurio quiera ingresar un elfo le pedira los datos,
 * y enviara los datos al vector de rohan.
 */
void agregar_elfos(juego_t* juego, personaje_t* personaje){
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
  juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
  juego->cantidad_rohan++;
  sacar_energia_defensivo( juego, ELFOS);
  if(((personaje->fila) < 9) && ((personaje->fila) > 4) && (personaje->columna <= 9 && personaje->columna >= 0)){
    juego->rohan[juego->cantidad_rohan] = personaje_inmovil_nuevo(ELFOS, juego->plus_rohan, personaje->columna, personaje->fila);
    juego->cantidad_rohan++;
    sacar_energia_defensivo( juego, ELFOS);
  }
}


/*
 * En el caso que el usurio quiera ingresar un orco le pedira los datos,
 * y enviara los datos al vector de isengard.
 */
void agregar_orcos(juego_t* juego, personaje_t* personaje){
  printf("Donde lo deseas posicionar?\n");
  printf("Columna: ");
  scanf("%i", &(personaje->columna));
  personaje->fila = 0;
  if(personaje->columna <= 9 && personaje->columna >= 0){
    juego->isengard  [juego->cantidad_isengard ] = personaje_movil_nuevo(ORCOS, juego->plus_isengard  , personaje->columna, personaje->fila);
    juego->cantidad_isengard++;
    sacar_energia_ofensivo( juego, ORCOS);
  }else{
    while(personaje->columna < 0 || personaje->columna > 9) {
      printf("No se puede posicionar en ese lugar. Por favor ingrese otro valor.\n");
      printf("Donde lo deseas posicionar?\n");
      printf("Columna: ");
      scanf("%i", &(personaje->columna));
    }
    personaje->fila = ORCOS;
    juego->isengard  [juego->cantidad_isengard ] = personaje_movil_nuevo(ORCOS, juego->plus_isengard  , personaje->columna, personaje->fila);
    juego->cantidad_isengard++;
    sacar_energia_ofensivo( juego, ORCOS);
  }
}


/*
 * En el caso que el usurio quiera ingresar un urukhai le pedira los datos,
 * y enviara los datos al vector de isengard.
 */
void agregar_urukhai(juego_t* juego, personaje_t* personaje){
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
  juego->isengard  [juego->cantidad_isengard ] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard  , personaje->columna, personaje->fila);
  juego->cantidad_isengard++;
  sacar_energia_ofensivo( juego, URUKHAI);
  if(((personaje->fila) < 9) && ((personaje->fila) > 4) && (personaje->columna <= 9 && personaje->columna >= 0)){
    juego->isengard  [juego->cantidad_isengard ] = personaje_inmovil_nuevo(URUKHAI, juego->plus_isengard  , personaje->columna, personaje->fila);
    juego->cantidad_isengard++;
    sacar_energia_ofensivo( juego, URUKHAI);
  }
}


/*
 * Le preguntara al usurio si quiere ingresar un personaje y en el caso que quiera,
 * le preguntara que personaje quiere ingresar.
 */
void agregar_personajes(juego_t* juego, personaje_t* personaje, char bando){
  char respuesta;
  int energia = energia_defensivo(juego);
  if(bando == 'R'){
    preguntar_usuario(&respuesta);
    if(respuesta == RESPUESTA_POSITIVA){
      char soldado;
      preguntar_personaje( &soldado, ELFOS, HOMBRES);
      if(soldado == ELFOS){
        if((energia >= ENERGIA_INMOVILES)){
          agregar_elfos(juego, personaje);
        }else{
          if((energia <= ENERGIA_INMOVILES)){
            printf("Energia insuficiente.\n");
            agregar_personajes(juego, personaje, bando);
          }
        }
      }else if(soldado == HOMBRES ){
        if((energia >= ENERGIA_MOVILES)){
          agregar_hombres(juego, personaje);
        }else{
          if((energia <= ENERGIA_MOVILES)){
            printf("Energia insuficiente.\n");
            agregar_personajes(juego, personaje, bando);
          }
        }
      }
    }
  }else{
    preguntar_usuario(&respuesta);
    if(respuesta == RESPUESTA_POSITIVA){
      char soldado;
      preguntar_personaje( &soldado, URUKHAI, ORCOS);
      if(soldado == URUKHAI){
        if((energia >= ENERGIA_INMOVILES)){
          agregar_urukhai(juego, personaje);
        }else{
          if((energia <= ENERGIA_INMOVILES)){
            printf("Energia insuficiente.\n");
            agregar_personajes(juego, personaje, bando);
          }
        }
      }else if(soldado == ORCOS){
        if((energia >= ENERGIA_MOVILES)){
          agregar_orcos(juego, personaje);
        }else{
          if((energia <= ENERGIA_MOVILES)){
            printf("Energia insuficiente.\n");
            agregar_personajes(juego, personaje, bando);
          }
        }
      }
    }
  }
}


/*
 * En el caso de que haya un solo jugador y su bando sea ofensivo, posicionara hombres y
 * cargara los datos al vector de rohan
 */
void jugador_maquina_defensivo(juego_t* juego, personaje_t* personaje){
  int numero = rand () % 10;
  if(juego->jugador2.energia >= ENERGIA_MOVILES){
      personaje->fila = FILA_HOMBRES;
      personaje->columna = numero;
      juego->terreno[personaje->fila][personaje->columna] = HOMBRES;
      juego->rohan[juego->cantidad_rohan] = personaje_movil_nuevo(HOMBRES, juego->plus_rohan, personaje->columna, personaje->fila);
      juego->cantidad_rohan++;
      juego->jugador2.energia -= ENERGIA_MOVILES;
  }else{
    return;
  }
}

/*
 * En el caso de que haya un solo jugador y su bando sea defensivo, posicionara orcos y
 * cargara los datos al vector de isengard
 */
void jugador_maquina_ofensivo(juego_t* juego, personaje_t* personaje){
  int numero = rand () % 10;
  if(juego->jugador2.energia >= ENERGIA_MOVILES){
      personaje->fila = 0;
      personaje->columna = numero;
      juego->terreno[personaje->fila][personaje->columna] = ORCOS;
      juego->isengard[juego->cantidad_isengard] = personaje_movil_nuevo(ORCOS, juego->plus_isengard, personaje->columna, personaje->fila);
      juego->cantidad_isengard++;
      juego->jugador2.energia -= ENERGIA_MOVILES;
  }else{
    return;
  }
}

//Revisa la vida de los personjes y define si sigue vivo o no.
void sigue_vivo(personaje_t *personaje){
  if(personaje->vida <= 0){
    personaje->esta_vivo = false;
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
  }
}

//Aumenta en 1 la energia de jugador 2.
void energia_j2(juego_t* juego){
  if(juego->jugador2.energia < ENERGIA_MAX){
    juego->jugador2.energia++;
  }
}


/*
 *
 */
void rango_inmovil(personaje_t personaje, personaje_t* enemigo, juego_t juego){
  int distancia = (abs(personaje.fila - enemigo->fila) + abs(personaje.columna - enemigo->columna));
  if(personaje.codigo == ELFOS){
    for(int i = 0; i < juego.cantidad_isengard; i++){
      if(distancia <= 3){
        enemigo->vida -= (ATAQUE_INMOVILES + juego.plus_rohan);
      }
    }
  }else if(personaje.codigo == URUKHAI){
    for(int i = 0; i < juego.cantidad_rohan; i++){
      if(distancia <= 3){
        enemigo->vida -= (ATAQUE_INMOVILES + juego.plus_isengard);
      }
    }
  }
}


/*
 * Revisara los vectores de cada bando y revisara si algun personaje no sigue vivo,
 * y en tal caso lo eliminara del vector y de la matriz
*/
void eliminar_personajes(juego_t* juego, personaje_t* personaje){
  for(int i = 0; i < juego->cantidad_rohan; i++){
    sigue_vivo(personaje);
    if(personaje->esta_vivo == false){
      juego->rohan[i] = juego->rohan[juego->cantidad_rohan - 1];
      juego->cantidad_rohan--;
      juego->terreno[personaje->fila][personaje->columna] = ' ';
    }
  }
  for(int i = 0; i < juego->cantidad_isengard; i++){
    sigue_vivo(personaje);
    if(personaje->esta_vivo == false){
      juego->isengard[i] = juego->isengard[juego->cantidad_isengard - 1];
      juego->cantidad_isengard--;
      juego->terreno[personaje->fila][personaje->columna] = ' ';
    }
  }
}

/*
 *Se fijara la poscion de los personajes inmoviles, y revisara el vector del bando
 * contrario y en el caso que este en rango atacara al personaje.
 */
void rango_inmoviles(juego_t* juego, personaje_t* personaje, personaje_t *enemigo){
  int distancia = (abs(personaje->fila - enemigo->fila) + abs(personaje->columna - enemigo->columna));
    if(personaje->codigo == HOMBRES && distancia != 1){
      personaje->fila--;
    }else{
      for(int i = 0; i < juego->cantidad_isengard; i++){
        if(distancia == 1){
          enemigo->vida = (ATAQUE_MOVILES + juego->plus_rohan);
        }
      }
    }
    if(personaje->codigo == ORCOS && distancia != 1){
      personaje->columna++;
    }else{
      for(int i = 0; i < juego->cantidad_rohan; i++){
        if(distancia == 1){
          enemigo->vida = (ATAQUE_MOVILES + juego->plus_isengard);
      }
    }
  }
}


/*
 *Revisara la posicion de los personjes moviles de cada, y en el caso de que esta en la posicion
 * esperada le sumara 1 a la cantidad de llegadas de cada jugador.
 */
void puntos(juego_t* juego, personaje_t personaje){
  if(personaje.codigo == HOMBRES && personaje.fila == 0){
    juego->llegadas_rohan++;
  }else if(personaje.codigo == ORCOS && personaje.fila == FILA_HOMBRES){
    juego->llegadas_isengard++;
  }
}

/*
 *Resvisara la cantidad de llegadas de cada jugador
 * y en el caso de que alguno llegue a la cantidad devolvera un true.
 */
bool ganador(juego_t* juego){
  bool gano = false;
  if(juego->llegadas_rohan == CANTIDAD_DE_LLEGADAS){
    gano = true;
  }else if(juego->llegadas_isengard == CANTIDAD_DE_LLEGADAS){
    gano = true;
  }
  return gano;
}


void inicializar_juego(juego_t* juego){
    int intensidad;
    int cantidad;
    char tipo;
    srand (( unsigned ) time ( NULL ));
    perfil(&tipo, &intensidad);
    (juego->jugador1.tipo) = tipo;
    (juego->jugador2.tipo) = tipo_opuesto(tipo);
    limpiar_juego( juego);
    limpiar_terreno(juego);
    averiguar_cant_jugadores( &cantidad);
    juego->cantidad_jugadores = cantidad;
    definir_plus_rohan( juego, intensidad, tipo);
    definir_plus_isengard( juego, intensidad, tipo);
    elfos_iniciales( juego);
    urukhai_iniciales( juego);
    imprimir_terreno( juego);
}


void pedir_datos(juego_t* juego, personaje_t* personaje){
  if(juego->cantidad_jugadores == 1){
    if(juego->jugador1.tipo == BANDO_DEFENSIVO){
      char bando = 'R';
      agregar_personajes(juego, personaje, bando);
      jugador_maquina_ofensivo(juego, personaje);
    }else{
      char bando = 'I';
      jugador_maquina_defensivo(juego, personaje);
      agregar_personajes(juego, personaje, bando);
    }
  }else{
    char bando_def = 'R';
    char bando_ofe = 'I';
    printf("Jugador Rohan:\n");
    agregar_personajes(juego, personaje, bando_def);
    printf("Jugador Isengard:\n");
    agregar_personajes(juego, personaje, bando_ofe);
  }
}


void posicionar_personaje(juego_t* juego, personaje_t personaje){
  for(int i = 0; i < juego->cantidad_rohan; i++){
    if(personaje.codigo == ELFOS) {
      juego->terreno[personaje.fila][personaje.columna] = ELFOS;
    }else{
      juego->terreno[personaje.fila][personaje.columna] = HOMBRES;
    }
  }
  for(int i = 0; i < juego->cantidad_isengard; i++){
    if(personaje.codigo == URUKHAI) {
      juego->terreno[personaje.fila][personaje.columna] = URUKHAI;
    }else{
      juego->terreno[personaje.fila][personaje.columna] = ORCOS;
    }
  }
  imprimir_terreno( juego);
}




//gcc  batalla.c  -o batalla -std=c99 -Wall -Wconversion -Werror -lm
