#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "mundo.h"

#define TAM_NOMBRE_ENTIDADES 50
#define TAM_DESCRIPCION_ENTIDADES 256
#define TAM_INVENTARIO 9
#define TAM_SALUD 1

typedef enum{
    E_VIVO,
    E_MUERTO
}estado_t;

typedef enum{
    T_JUGADOR,
    T_ENEMIGO,
    T_NPC
}tipo_entidad_t;

typedef struct{
    char nombre[TAM_NOMBRE_ENTIDADES];
    char descripcion[TAM_DESCRIPCION_ENTIDADES];
    tipo_entidad_t tipo;
    estado_t estado;
    int ubicacion;
    int inventario[TAM_INVENTARIO];
    int cantidad_items;
}entidades_t;


typedef struct{
    entidades_t esqueleto_entidad;
    int bendicion; //si le rezamos al arcangel y lo liberamos nos bendice, esto es una condicion para derrotar al jefe final
    int railes_activos; //esto determina si nos podemos mover por algunas salas del area minera
}jugador_t;

typedef struct{
    entidades_t esqueleto_entidad;
    int dialogo;
    char frase_muerte[TAM_DESCRIPCION_ENTIDADES];
}npc_t;


extern jugador_t jugador;
extern npc_t npc[MAX_SALAS];
extern int contar_npcs;


//funciones
void inicializar_jugador();
void inicializar_npcs();
int agregar_al_inventario(int id_objeto);
int remover_del_inventario(int id_objeto);
void mostrar_inventario();
void resetear_juego();

#endif // ENTIDADES_H
