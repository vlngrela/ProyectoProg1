#ifndef MUNDO_H
#define MUNDO_H

#include "utilidades.h"

#define MAX_SALIDAS 6
#define MAX_SALAS 20
#define TAM_NOMBRE 64
#define TAM_DESCRIPCION 256
#define ABIERTO 0
#define CERRADO 1

typedef struct{
    const char* descripcion;
    int sala_buscada; //sala de destino
    int bloqueada;  // 0 = abierto, 1 = cerrada
}conexiones_t;

typedef struct{
    int id;
    char nombre[TAM_NOMBRE];
    char descripcion[TAM_DESCRIPCION];
    int piso;
    conexiones_t salidas[MAX_SALIDAS]; //conexion a otras salas
}salas_t;

extern salas_t mundo[MAX_SALAS];
extern int contador_salas;
void inicializar_mundo();

#endif
