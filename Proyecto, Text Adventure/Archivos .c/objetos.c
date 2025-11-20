#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "objetos.h"
#include "entidades.h"
#include "utilidades.h"
#include "comandos.h"

objetos_t objetos[TAM_OBJETOS];
int contador_objetos = 0;

void inicializar_objetos(){

/* ===[ESTRUCTURA]===

    OBJETOS[i] = (OBJETOS_T){
        ID,
        NOMBRE,
        NOMBRE_ARCHIVO_ASCII,
        TIPO,
        USO, (tiene un uso especifico)
        DESAPARECE, (=consumible)
        UBICACION
    };

*/

    //Espada Desgastada
    objetos[0] = (objetos_t){
        0, //id objeto
        "Espada Desgastada", //nombre, descripcion, descripcion al examinar
        "Espada_desgastada.txt",
        T_ARMA, //tipo de objeto
        0, // no se "usa" como tal
        0, //no es consumible
        -1, //ubicacion: sala 0  (si es -1: esta en el inventario, si es -2: esta oculto)
    };

    // Llave del Calabozo
     objetos[1] = (objetos_t){
        1, //id objeto
        "Llaves de las Celdas", //nombre, descripcion, descripcion al examinar
        "Llave.txt",
        T_LLAVE, //tipo de objeto
        1, // se puede usar
        0, //no es consumible
        -2, // drop del Guardian
    };

    // Botella de Whisky, parte del crafteo de la molotov
     objetos[2] = (objetos_t){
        2, //id objeto
        "Botella de Whisky", //nombre, descripcion, descripcion al examinar
        "Whisky.txt",
        T_INGREDIENTE, //tipo de objeto
        0, // no se puede usar
        0, // mucho menos tomar en la situacion en la que esta el jugador
        -2, //la dropea l
    };

    //Encendedor
     objetos[3] = (objetos_t){
        3, //id objeto
        "Encendedor", //nombre, descripcion, descripcion al examinar
        "Encendedor.txt",
        T_INGREDIENTE, // polemico, como tal no forma parte del crafteo pero es parte necesaria para tener/usar el molotov
        0, // no se puede usar solo (o vas a querer mirar el fuego? jaja)
        0, //no es consumible
        -2, //lo dropea el teniente, sala 8
    };

    //Paquete de cigarrillos
     objetos[4] = (objetos_t){
        4, //id objeto
        "Paquete de cigarrillos", //nombre, descripcion, descripcion al examinar
        "Cigarrillos.txt",
        T_NORMAL, //tipo de objeto
        0, // no se pueden usar
        0, //el personaje no es fumador (conveniencia mia para no tener que hacerlo)
        -2, //drop del teniente junto con el encendedor
    };

    //Vendas
     objetos[5] = (objetos_t){
        5, //id objeto
        "Vendas", //nombre, descripcion, descripcion al examinar
        "Vendas.txt",
        T_INGREDIENTE, //tipo de objeto
        0, // no se usa, es parte del crafteo para el molotov
        0, //no es consumible
        -2, //lo da el boticario, sala 10
    };

    //Molotov
     objetos[6] = (objetos_t){
        6, //id objeto
        "Molotov", //nombre, descripcion, descripcion al examinar
        "Molotov.txt",
        T_ARMA, //tipo de objeto
        1, // la usamos para calcinar a rudigier
        1, // solo hay una, si se usa se consume
        -2, // la fabrica el boticario, sala 10
    };

    //Brebaje Medicinal (una pocion basicamente)
     objetos[7] = (objetos_t){
        7, //id objeto
        "Brebaje Medicinal", //nombre, descripcion, descripcion al examinar
        "Brebaje_medicinal.txt",
        T_CONSUMIBLE, //tipo de objeto
        1, // lo usamos con el minero herido
        1, // solo hay una, se consume
        -2, // la da fernando cuando hablamos
    };

    //Espada Bendita - "reemplaza" la espada vieja que teniamos, "recupera" su verdadera forma (basicamente la cambiamos asi es mas facil)
    objetos[8] = (objetos_t){
        8, //id objeto
        "Espada Aclamada por el Sol", //nombre, descripcion, descripcion al examinar
        "Espada_bendita.txt",
        T_ARMA, //tipo de objeto
        0, // no se "usa" como tal
        0, // no se consume
        -2, // Samael bendice la espada desgastada y obtemos esta
    };


    contador_objetos = 9;
}
int posesion_objeto(int id_objeto){
    for (int i=0; i < jugador.esqueleto_entidad.cantidad_items; i++){

        if(jugador.esqueleto_entidad.inventario[i] == id_objeto) return 1;
    }

    return 0;
}

void mostrar_ascii_art(const char* archivo){

    FILE* arch = fopen(archivo, "r");

    if(arch == NULL){
        printf(" ===[NO SE PUDO CARGAR EL OBJETO]===\n");
        return;
    }
    printf("\n");
    char linea[TAM_LINEA];
    while(fgets(linea, sizeof(linea), arch) != NULL) printf("%s", linea);
    printf("\n");

    pausa();
    fclose(arch);
    limpiar_pantalla();
}

int fabricar_molotov(){

    int whisky = posesion_objeto(BOTELLA);
    int encendedor = posesion_objeto(ENCENDEDOR);
    int vendas = posesion_objeto(VENDAS);

    if(whisky && encendedor && vendas){
        remover_del_inventario(BOTELLA);
        remover_del_inventario(VENDAS);
        agregar_al_inventario(MOLOTOV);

        printf("\n");
        printf(" El Boticario agarra la botella y envuelve el cuello con las vendas.\n");
        printf(" [%s]: ya esta lista, usa el encendedor cuando quieras lanzarla.\n Ten el cuidado de no tirartela a ti mismo\n\n", npc[8].esqueleto_entidad.nombre);
        printf(" ===[HAS RECIBIDO: MOLOTOV]===\n");

        return 1;
    }

    printf(" El Boticario revisa tus objetos...\n\n");
    printf(" [%s]: Para fabricar un molotov necesitas: \n", npc[8].esqueleto_entidad.nombre);
    if(!whisky) printf("  -Una botella con alcohol dentro\n");
    if(!encendedor) printf("  -Un encendedor para prender el molotov\n");
    if(!vendas) printf("  -Tela para hacer de mecha\n");

    if(!vendas && whisky && encendedor){
        printf(" [%s]: Veo que tienes casi todo.. Dejame ayudarte con la mecha\n\n", npc[8].esqueleto_entidad.nombre);
        printf(" ===[HAS RECIBIDO: VENDAS]===\n");
        agregar_al_inventario(VENDAS);
        objetos[5].ubicacion = -1;
    }

    return 1;
}
