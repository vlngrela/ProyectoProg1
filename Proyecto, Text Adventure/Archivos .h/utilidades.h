#ifndef UTILIDADES_H
#define UTILIDADES_H

#define TAM_LINEA 256

//Direcciones
typedef enum{
    DIR_NORTE,
    DIR_SUR,
    DIR_ESTE,
    DIR_OESTE,
    DIR_ARRIBA,
    DIR_ABAJO,
    DIR_NINGUNO
}direcciones_t;

//Opciones del menu en main
typedef enum{
    SALIR,
    JUGAR,
    CREDITOS
}menu_t;

//Indice = id de los objetos. para no tener numero sueltos
typedef enum{
    ESPADA_DESGASTADA,
    LLAVES,
    BOTELLA,
    ENCENDEDOR,
    CIGARRILLOS,
    VENDAS,
    MOLOTOV,
    BREBAJE,
    ESPADA_BENDITA
}e_objetos_t;

const char*  direccion_a_string(direcciones_t dir); //Convierte la direccion a strings
void mostrar_escena_muerte();
void final_malo();
void final_bueno();
void limpiar_buffer();
void limpiar_pantalla();
void pausa();

#endif
