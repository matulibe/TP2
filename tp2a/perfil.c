#include <stdio.h>
#include "perfil.h"

const int LIMITE_MAX_MALDADES = 99;
const int LIMITE_MIN_MALDADES = 0;
const int CANTIDAD_MAX_MASCOTAS = 5;
const int CANTIDAD_MIN_MASCOTAS = 0;
const int MAX_SIGNO = 12;
const int MIN_SIGNO = 1;

const char ELEMENTO_FUEGO = 'F';
const char ELEMENTO_TIERRA = 'T';
const char ELEMENTO_AIRE = 'A';
const char ELEMENTO_AGUA = 'W';
const char GENERO_ACCION = 'A';
const char GENERO_DRAMA = 'D';
const char GENERO_COMEDIA = 'C';
const char GENERO_TERROR = 'T';


//Pre: ingreasar un tipo de dato entero >0 y <=12
//Post: Obtener el elemento del zodiaco
void averiguar_elemento(int* signo_zodiaco, char* elemento){
    printf("Ingresa el numero de tu signo:\n");
    printf("Aries:1\nTauro:2\nGeminis:3\nCancer:4\nLeo:5\nVirgo:6\n");
    printf("Libra:7\nEscorpio:8\nSagitario:9\nCapricornio:10\nAcuario:11\nPiscis:12\n");
    scanf(" %i", signo_zodiaco);
    if(*signo_zodiaco == 1|| *signo_zodiaco == 5|| *signo_zodiaco == 9){
        (*elemento) = ELEMENTO_FUEGO;
        printf("Tu elemento es Fuego\n");
    }else if(*signo_zodiaco == 2|| *signo_zodiaco == 6|| *signo_zodiaco == 10){
        (*elemento) = ELEMENTO_TIERRA;
        printf("Tu elemento es Tierra\n");
    }else if(*signo_zodiaco == 3|| *signo_zodiaco == 7|| *signo_zodiaco == 11){
        (*elemento) = ELEMENTO_AIRE;
        printf("Tu elemento es Aire\n");
    }else if(*signo_zodiaco == 4|| *signo_zodiaco == 8|| *signo_zodiaco == 12){
        (*elemento) = ELEMENTO_AGUA;
        printf("Tu elemento es Agua\n");
    }else{
        while((*signo_zodiaco < MIN_SIGNO) || (*signo_zodiaco > MAX_SIGNO)){
            printf("\nValor ingresado no valido. Por favor ingrese un valor correcto.\n");
            averiguar_elemento( signo_zodiaco, elemento);
        }
        return;
    }
}

//Pre: Valores de numeros enteros positivos
//Post: Saber la cantidad de mascotas para modificar la intensidad
void averiguar_mascotas(int* cantidad_mascotas){
    printf("Cuantas mascotas tenes? Ingrese un numero del 0 a 5\n");
    scanf("%i", cantidad_mascotas);
    if(*cantidad_mascotas > CANTIDAD_MAX_MASCOTAS){
        printf("Valor ingresado no valido. Por favor ingrese un valor correcto.\n");
        averiguar_mascotas( cantidad_mascotas);
    }else if(*cantidad_mascotas < CANTIDAD_MIN_MASCOTAS){
        printf("Valor ingresado no valido. Por favor ingrese un valor correcto.\n");
        averiguar_mascotas( cantidad_mascotas);
    }
}

//Pre: Obtener un valor entero entre 0 y 99
//Post: Saber la cantidad de maldades para obtener un valor inicial de la intensidad
void cantidad_de_maldades(int* maldades_ult_mes){
    printf("Cuantas maldades hiciste en el ultimo mes? Ingresa un numero entre 0 y 99\n");
    printf("NOTA: Si tu elemento es Tierra o Agua ingrese un numero entre 0 y 49. Si su elemento es Fuego o Aire ingrese un numero entre 50 y 99\n");
    scanf("%i", maldades_ult_mes);
    if((*maldades_ult_mes < LIMITE_MIN_MALDADES) || (*maldades_ult_mes > LIMITE_MAX_MALDADES)){
        printf("Valor ingresado no valido. Por favor ingrese un valor correcto.\n");
        cantidad_de_maldades(maldades_ult_mes);
    }

}


void asignar_nivel(int* cantidad_mascotas, int* maldades_ult_mes, int* nivel){

    int intensidad = 0;

    if( *cantidad_mascotas == 1){
       intensidad = intensidad + 1;
    }else if( *cantidad_mascotas == 2){
       intensidad = intensidad + 2;
    }else if( *cantidad_mascotas == 3){
       intensidad = intensidad + 3;
    }else if( *cantidad_mascotas == 4){
       intensidad = intensidad + 4;
    }else if( *cantidad_mascotas == 5){
       intensidad = intensidad + 5;
    }

    if(((*maldades_ult_mes >= 0) && (*maldades_ult_mes <= 9)) || ((*maldades_ult_mes >= 50) && (*maldades_ult_mes <= 59))){
           intensidad = intensidad + 1;
    }else if(((*maldades_ult_mes >= 10) && (*maldades_ult_mes <= 19)) || ((*maldades_ult_mes >= 60) && (*maldades_ult_mes <= 69))){
           intensidad = intensidad + 2;
    }else if(((*maldades_ult_mes >= 20) && (*maldades_ult_mes <= 29)) || ((*maldades_ult_mes >= 70) && (*maldades_ult_mes <= 79))){
           intensidad = intensidad + 3;
    }else if(((*maldades_ult_mes >= 30) && (*maldades_ult_mes <= 39)) || ((*maldades_ult_mes >= 80) && (*maldades_ult_mes <= 89))){
           intensidad = intensidad + 4;
    }else if(((*maldades_ult_mes >= 40) && (*maldades_ult_mes <= 49)) || ((*maldades_ult_mes >= 90) && (*maldades_ult_mes <= 99))){
           intensidad = intensidad + 5;
    }

    *nivel = intensidad;

}


//Pre: Ingresar un caracter en Mayusculas
//Post: Obtener el genero
void averiguar_genero_fav(char *genero_cine_fav){
    printf("Cual es tu genero de cine preferido de las siguientes opciones(Solo ingrese  la letra asignada):\n");
    printf("A.accion\nD.drama\nC.comedia\nT.terror\n");
    scanf(" %c", genero_cine_fav);

    while((*genero_cine_fav != 'A') && (*genero_cine_fav != 'D') && (*genero_cine_fav != 'C') && (*genero_cine_fav != 'T')){
        printf("\nValor ingresado no valido. Por favor ingrese un valor correcto.\n");
        scanf(" %c", genero_cine_fav);
        }
    }

void asignar_bando( int *signo_zodiaco, char *genero_cine_fav, int *maldades_ult_mes, char *elemento, char* bando, int *nivel, int* cantidad_mascotas){


    if(((*genero_cine_fav == GENERO_COMEDIA) || (*genero_cine_fav == GENERO_TERROR)) && (*maldades_ult_mes <= 99) && (*maldades_ult_mes >= 50) &&
        ((*elemento == ELEMENTO_FUEGO) || (*elemento == ELEMENTO_AIRE))){
        *bando = BANDO_OFENSIVO;
         printf("Ha sido asignado al bando ofensivo, nivel %i, luchara de la mano de Saruman. Hagalo sin piedad!\n", *nivel);


    }else if(((*genero_cine_fav == GENERO_ACCION) || (*genero_cine_fav == GENERO_DRAMA)) && (*maldades_ult_mes >= 0) && (*maldades_ult_mes <= 49) &&
        ((*elemento == ELEMENTO_AGUA) || (*elemento == ELEMENTO_TIERRA))){
        *bando = BANDO_DEFENSIVO;
        printf("Ha sido asignado al bando defensivo, nivel %i, luchara de la mano de Aragon. Hagalo con coraje!\n", *nivel);

    }

    while((*bando != BANDO_OFENSIVO) && (*bando != BANDO_DEFENSIVO)){
        printf("No se le ha podido asignar un bando. Por favor vuelva a empezar\n");
        averiguar_elemento( signo_zodiaco,  elemento);
        averiguar_genero_fav( genero_cine_fav);
        cantidad_de_maldades(  maldades_ult_mes);
        asignar_nivel( cantidad_mascotas, maldades_ult_mes, nivel);
        asignar_bando( signo_zodiaco, genero_cine_fav, maldades_ult_mes, elemento, bando, nivel, cantidad_mascotas);
    }
}


void perfil(char* tipo, int* intensidad){
    int signo_zodiaco;
    char genero_cine_fav;
    int maldades_ult_mes;
    int cantidad_mascotas;
    char elemento;
    char bando;
    int nivel;
    averiguar_elemento( &signo_zodiaco, &elemento);
    averiguar_genero_fav(&genero_cine_fav);
    cantidad_de_maldades( &maldades_ult_mes);
    averiguar_mascotas( &cantidad_mascotas);
    asignar_nivel( &cantidad_mascotas, &maldades_ult_mes, &nivel);
    asignar_bando( &signo_zodiaco, &genero_cine_fav, &maldades_ult_mes, &elemento, &bando, &nivel, &cantidad_mascotas);
    *tipo = bando;
    *intensidad = nivel;

}
