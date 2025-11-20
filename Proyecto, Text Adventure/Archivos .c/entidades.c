#include "entidades.h"
#include "objetos.h"
#include "mundo.h"
#include "comandos.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

jugador_t jugador;
npc_t npc[MAX_SALAS];
int contar_npcs = 0;

void inicializar_jugador(){

/* ===[ESTRUCTURA]===
    JUGADOR = (JUGADOR_T) {
        {
            NOMBRE,
            DESCRIPCION,
            TIPO, (jugador, para diferenciarnos de otras entidades)
            ESTADO, (vivo o muerto)
            UBICACION,
            INVENTARIO[i],
            CANTIDAD DE ITEMS
        },
        BENDICION (si usamos rezar con samael, cambia de 0 a 1)
        RAILES (si usamos el brebaje con sebastian, cambia de 0 a 1)
    };
*/
    jugador = (jugador_t){
        {
            "Sacrificio",
            "Pobre alma, traida a uno de los lugares mas oscuros de este mundo.",
            T_JUGADOR,
            E_VIVO,
            0, // ubicacio: primera sala del juego
            {0}, // inventario vacio
            0 // 0 items guardados
        },
        0, // sin bendicion
        0, // railes sin activar
    };
}

void inicializar_npcs(){

/* ===[ESTRUCTURA]===
    NPC[i] = (NPC_T){
        <ESQUELETO ENTIDAD>
        {
            NOMBRE,
            DESCRIPCION,
            TIPO, (npc neutral/amigable o enemigos)
            ESTADO, (vivo o muerto)
            UBICACION,
            INVENTARIO,
            CANTIDAD ITEMS,
        },
        <DISTINTIVO NPC>
        DIALOGOS (-1: no tiene dialogos adicionales, 0: tiene dialogo adicional, 1: dialogo final )
        FRASE CUANDO MUERE
    };

*/

    npc[0] = (npc_t){
        {
            "Viejo Sacrificio",
            "El sacrificio mas longevo, reducido a un monton de \n huesos rotos en su ultimo intento por escapar.",
            T_NPC,
            E_VIVO,
            0,
            {1},
            1,
        },
        -1,
        "Libre.. al fin..."
    };


    npc[1] = (npc_t){
        {
            "Arzobispo del Culto, Xurxess",
            "Maxima autoridad religiosa en la Orden del Culto.\n Autor de cada sacrificio hecho en este altar.",
            T_ENEMIGO,
            E_VIVO,
            1,
            {0},
            0,
        },
        -1,
        "Intenta escapar todo lo quieras.. *arghh* -mientras cae de rodillas- Al final seras borrado por su Santidad! "
    };


    npc[2] = (npc_t){
        {
            "Cardenal del Culto, Julien",
            "Segundo en autoridad. Encargado de doblegar la voluntad de todos\n los sacrificios hasta que solo quedara una cascara.",
            T_ENEMIGO,
            E_VIVO,
            1,
            {0},
            0,
        },
        -1,
        "Moriras! aunque no este aqui para verlo..."
    };

     npc[3] = (npc_t){
        {
            "Diacono del Culto, Oscar",
            "Personaje penoso y humillante, solo sabe adular a Xurxess.",
            T_ENEMIGO,
            E_VIVO,
            1,
            {0},
            0,
        },
        -1,
        "N-No por favor! -suplica- no q-quiero morir..."
    };

     npc[4] = (npc_t){
        {
            "Guardian del Calabozo, Mimir",
            "Ser sadico y horrible. Guiado por el deber y el placer de torturar a los prisioneros.",
            T_ENEMIGO,
            E_VIVO,
            5,
            {1},
            1,
        },
        -1,
        "Maldita! *blughh* -escupe sangre- basura, como te atreves..."
    };

     npc[5] = (npc_t){
        {
            "Arcangel Caido, Samael",
            "Ser amado y divino. Caido en la desesperacion luego de las incontables torturas.",
            T_NPC,
            E_VIVO,
            6,
            {0},
            0,
        },
        -1,
        "Ahora ascendido y liberado. Yo, te bendigo!"
    };

     npc[6] = (npc_t){
         {
            "Soldado Novato, Ramos",
            "Joven iluso que se unio hace poco al Culto. Lo mas bajo de la jerarquia.",
            T_ENEMIGO,
            E_VIVO,
            8,
            {0},
            0,
         },
         -1,
        "P-pero.. solo estuve aca por d-dos dias!"
    };


     npc[7] = (npc_t){
        {
            "Teniente de la Orden, Samuel",
            "Veterano de la Orden del Culto. Entrenado desde joven para ser un soldado devoto.",
            T_ENEMIGO,
            E_VIVO,
            8,
            {2},
            2,
        },
        -1,
        "..."
    };

     npc[8] = (npc_t){
        {
            "Boticario Fernando",
            "Unico boticario de la Orden, se le notan las ojeras por exceso de trabajo.",
            T_NPC,
            E_VIVO,
            10,
            {1},
            1,
        },
        -1,
        "P-por que!?"
    };

     npc[9] = (npc_t){
        {
            "Paladin Hereje, Rudigier",
            "Personaje poderoso de la Orden del Culto. Su arrogancia y narcisismo hicieron que ya no porte\n su armadura. Ahora solo viste telas y sedas finas, como si no le importara a quien se enfreta.",
            T_ENEMIGO,
            E_VIVO,
            11,
            {0},
            0,
        },
        -1,
        "I-Imposible.. como alguien tan bajo? -arde furiosamente- *HRRAAAAGH* ESTE FU-FUEGO.. M-ME CONSUME!"
    };

     npc[10] = (npc_t){
        {
            "Ultimo Minero, Sebastian",
            "Unico superviviente de la masacre en las minas. Se encuentra gravemente herido, es un milagro que siga vivo.",
            T_NPC,
            E_VIVO,
            13,
            {0},
            0,
        },
        0,
        "Gra-gracias.. por todo..."
    };

     npc[11] = (npc_t){
        {
            "Encarnacion de la Luna, Mamakila",
            "Es imposible describirla.",
            T_ENEMIGO,
            E_VIVO,
            19,
            {0},
            0,
        },
        -1,
    };

    contar_npcs = 12;
}

int agregar_al_inventario(int id_objeto){
    jugador.esqueleto_entidad.inventario[jugador.esqueleto_entidad.cantidad_items] = id_objeto;
    jugador.esqueleto_entidad.cantidad_items++;

    return 1;
}

int remover_del_inventario(int id_objeto){
    for(int i = 0; i < jugador.esqueleto_entidad.cantidad_items; i++){
        if(jugador.esqueleto_entidad.inventario[i] == id_objeto){
            for(int j = i; j < jugador.esqueleto_entidad.cantidad_items - 1; j++){
                jugador.esqueleto_entidad.inventario[j] = jugador.esqueleto_entidad.inventario[j + 1];
            }
            jugador.esqueleto_entidad.cantidad_items--;
            jugador.esqueleto_entidad.inventario[jugador.esqueleto_entidad.cantidad_items] = -1;
            return 1;
        }
    }

    return 0;
}

void mostrar_inventario(){
    if(jugador.esqueleto_entidad.cantidad_items == 0){
        printf(" Tu inventario esta vacio..\n");
        return;
    }
    printf(" ---- INVENTARIO ----\n");
    for(int i=0; i < jugador.esqueleto_entidad.cantidad_items; i++){
        for(int j=0; j < contador_objetos; j++){
            if(objetos[j].id == jugador.esqueleto_entidad.inventario[i]){
                printf(" %d- %s\n", i+1, objetos[j].nombre);
                break;
            }
        }
    }
}

void resetear_juego(){
    printf(" Todo el progreso se ha perdido.\n");
    printf(" Reiniciando desde el principio.\n");

    inicializar_mundo();
    inicializar_jugador();
    inicializar_npcs();
    inicializar_objetos();

    printf(" Despiertas nuevamente en el Almacen...\n");
    cmd_mirar();
}
