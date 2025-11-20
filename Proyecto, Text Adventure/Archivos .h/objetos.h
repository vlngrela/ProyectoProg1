#ifndef OBJETOS_H
#define OBJETOS_H

#define TAM_OBJETOS 50
#define TAM_NOMBRE_OBJETOS 32
#define TAM_ARCHIVO 128

typedef enum{
    T_NORMAL,
    T_CONSUMIBLE,
    T_LLAVE,
    T_ARMA,
    T_INGREDIENTE
}tipo_objeto_t;

typedef struct{
    int id;
    char nombre[TAM_NOMBRE_OBJETOS];
    char ascii_art[TAM_ARCHIVO];
    tipo_objeto_t tipo;
    int usable;
    int consumible;
    int ubicacion;
}objetos_t;

extern objetos_t objetos[TAM_OBJETOS];
extern int contador_objetos;

void inicializar_objetos();
int posesion_objeto(int id_objeto);
int fabricar_molotov();
void mostrar_ascii_art(const char* archivo);

#endif // OBJETOS_H
