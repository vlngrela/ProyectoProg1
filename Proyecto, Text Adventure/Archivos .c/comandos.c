#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "comandos.h"
#include "mundo.h"
#include "entidades.h"
#include "objetos.h"
#include "utilidades.h"

extern jugador_t jugador;
extern npc_t npc[];
extern int contar_npcs;
extern objetos_t objetos[];
extern int contador_objetos;

int oportunidad_guardian = 0;

int convert_a_dir(const char *dir){
    if(!dir) return DIR_NINGUNO;
    if(strcmp(dir, "norte") == 0) return DIR_NORTE;
    if(strcmp(dir, "sur") == 0) return DIR_SUR;
    if(strcmp(dir, "oeste") == 0) return DIR_OESTE;
    if(strcmp(dir, "este") == 0) return DIR_ESTE;
    if(strcmp(dir, "arriba") == 0) return DIR_ARRIBA;
    if(strcmp(dir, "abajo") == 0) return DIR_ABAJO;

    return DIR_NINGUNO;
}


//strstr es estricto con mayusculas-minusculas, esto facilita la comparacion y la busqueda
static int comparar_nombres_entidades(const char *nombre_npc, const char *npc_buscado){
    char comp_nombre[TAM_NOMBRE_ENTIDADES];
    char comp_npc_buscado[TAM_NOMBRE_ENTIDADES];
    int i;

    for (i = 0; nombre_npc[i] && i < TAM_NOMBRE_ENTIDADES -1; i++){
        comp_nombre[i] = tolower((unsigned char) nombre_npc[i]);
    }
    comp_nombre[i] = '\0';

    for(i = 0; npc_buscado[i] && i < TAM_NOMBRE_ENTIDADES -1; i++){
        comp_npc_buscado[i] = tolower((unsigned char) npc_buscado[i]);
    }
     comp_npc_buscado[i] = '\0';

    return strstr(comp_nombre, comp_npc_buscado) != NULL;
}

static int comparar_nombres_objetos (const char *nombre_objeto, const char *objeto_buscado){
    char comp_nombre[TAM_NOMBRE_OBJETOS];
    char comp_objeto_buscado[TAM_NOMBRE_OBJETOS];
    int i;

    for (i = 0; nombre_objeto[i] && i < TAM_NOMBRE_OBJETOS -1; i++){
        comp_nombre[i] = tolower((unsigned char) nombre_objeto[i]);
    }
    comp_nombre[i] = '\0';

    for(i = 0; objeto_buscado[i] && i < TAM_NOMBRE_OBJETOS -1; i++){
        comp_objeto_buscado[i] = tolower((unsigned char) objeto_buscado[i]);
    }
     comp_objeto_buscado[i] = '\0';

    return strstr(comp_nombre, comp_objeto_buscado) != NULL;
}

//comando ir

int cmd_ir(const char *par){
    int dir = convert_a_dir(par);

    if (!par){
        printf(" A donde quieres ir?\n");

        return 1;
    }

    if(dir == DIR_NINGUNO){
        printf(" <%s> no es un lugar valido..\n", par);

        return 1;
    }

    salas_t *sala_actual = &mundo[jugador.esqueleto_entidad.ubicacion];
    conexiones_t salida = sala_actual->salidas[dir];

    if(salida.sala_buscada == -1 || salida.descripcion == NULL){
        printf(" No puedes ir en esa direccion..\n");

        return 1;
    }

    if(salida.bloqueada == CERRADO){
        printf(" <%s> se encuentrada bloqueado/cerrado.\n", salida.descripcion);

        return 1;
    }

    int vagonetas = 0;

    if((jugador.esqueleto_entidad.ubicacion == 13 && salida.sala_buscada == 15) || (jugador.esqueleto_entidad.ubicacion == 15 && salida.sala_buscada == 13)){
        vagonetas = 1;
    }
    if((jugador.esqueleto_entidad.ubicacion == 16 && salida.sala_buscada == 17) || (jugador.esqueleto_entidad.ubicacion == 17 && salida.sala_buscada == 16)){
        vagonetas = 1;
    }
    if((jugador.esqueleto_entidad.ubicacion == 17 && salida.sala_buscada == 18) || (jugador.esqueleto_entidad.ubicacion == 18 && salida.sala_buscada == 17)){
        vagonetas = 1;
    }

    if (vagonetas && !jugador.railes_activos){
        printf(" El sistema de transporte de vagonetas por railes se encuentra desactivado.\n");
        printf(" Necesitas a un maquinista para operarlas..\n");

        return 1;
    }

    jugador.esqueleto_entidad.ubicacion = salida.sala_buscada;

    if(vagonetas){
         printf(" Te subes a una vagoneta y viajas por los railes..\n");
         pausa();
         limpiar_pantalla();
    }

    else{
        printf(" Te mueves por <%s>, hasta llegar a..\n", salida.descripcion);
        pausa();
        limpiar_pantalla();
    }

    cmd_mirar();

    return 1;
}

//comando mirar

int cmd_mirar(){

    limpiar_pantalla();
    salas_t *sala = &mundo[jugador.esqueleto_entidad.ubicacion];

    printf("\n --- %s ---\n\n", sala->nombre);
    //mostrar conexiones entre salas
    printf(" [CONEXIONES]: \n\n");
    for(int i=0; i < MAX_SALIDAS; i++){

        if(sala->salidas[i].descripcion != NULL && sala->salidas[i].sala_buscada != -1){
            const char *dir_str = direccion_a_string(i);
            printf(" [%s]: %s", dir_str, sala->salidas[i].descripcion);

            if(sala->salidas[i].bloqueada == CERRADO) printf(" [CERRADO]");
            printf("\n");
        }
    }
    printf("\n");
    printf(" [PERSONAJES]: \n\n");
    int npcs_encontrados = 0;

    for (int i=0; i < contar_npcs; i++){

        if(npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO){
            printf(" - %s\n", npc[i].esqueleto_entidad.nombre);
            npcs_encontrados++;
        }
    }

    if(npcs_encontrados == 0) printf(" No hay nadie en esta sala..\n");
    printf("\n");

    printf(" [OBJETOS]: \n\n");
    int objetos_encontrados = 0;

    for(int i=0; i < contador_objetos; i++){
        if(objetos[i].ubicacion == jugador.esqueleto_entidad.ubicacion){
            printf(" - %s\n", objetos[i].nombre);
            objetos_encontrados++;
        }
    }

    if(objetos_encontrados == 0) printf(" No hay objetos en esta sala..\n");
    printf("\n");

    return 1;
}

// comando examinar

int cmd_examinar(const char *par){

    if (!par || strlen(par) == 0){
        printf(" Que quieres examinar?\n");

        return 1;
    }

    if (strstr(par,"sala")){

        for (int i = 0; i < contador_salas; i++){

            if (mundo[i].id == jugador.esqueleto_entidad.ubicacion){
                printf(" Examinas <%s>:\n", mundo[i].nombre);
                printf(" %s\n", mundo[i].descripcion);

                if (jugador.esqueleto_entidad.ubicacion == 4){
                    printf("\n\n");
                    printf(" [PISTA]: se ve movimiento en la <puerta> del calabozo..\n");
                }


                if(jugador.esqueleto_entidad.ubicacion == 5 ){

                    if(npc[4].esqueleto_entidad.estado == E_MUERTO){
                        pausa();
                        printf("\n");
                        printf(" Mientras caminas por el calabozo, te encuentras con el puesto del Guardian.\n");
                        printf(" Revuelves entre sus cosas y te encuentras con..\n");

                        pausa();

                        if(objetos[2].ubicacion == -2){
                            printf(" Una botella de whisky!\n");
                            printf(" Parece que el Guardian la pasaba muy bien en este lugar...\n");
                            printf(" Parece ser que aun queda algo de whisky..\n\n");
                            objetos[2].ubicacion = 5;
                            printf(" ===[YA PUEDES AGARRAR LA BOTELLA DE WHISKY]===\n");

                            pausa();
                            cmd_mirar();
                        }
                        else if(objetos[2].ubicacion == -1) printf(" Ya tienes guardada en el inventario la botella de whisky.\n");

                        else printf(" La botella que encontraste antes.\n");
                    }
                    else printf(" El Guardian te impide examinar la sala con detalle.\n");
                }

                return 1;
            }
        }
    }

     if(jugador.esqueleto_entidad.ubicacion == 4 && (strstr(par, "puerta") || strstr(par, "calabozo"))){
        printf("\n");
        printf(" Examinas la puerta del calabozo...\n\n");
        printf(" Casi que espiando, ves al <%s> patrullando.\n", npc[4].esqueleto_entidad.nombre);
        printf(" Por alguna razon se encuentra distraido y esta de espaldas.\n");
        printf(" Podrias atacarlo desde aqui!\n");
        oportunidad_guardian = 1;

        return 1;
    }

    for(int i=0; i < contar_npcs; i++){

        if(npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && comparar_nombres_entidades(npc[i].esqueleto_entidad.nombre, par)){
            printf("\n");
            printf(" Examinas a <%s>:\n",npc[i].esqueleto_entidad.nombre);
            printf(" %s\n", npc[i].esqueleto_entidad.descripcion);

            if(npc[i].esqueleto_entidad.estado == E_MUERTO) printf(" Esta muerto...\n");

            if(i == 4 && npc[i].esqueleto_entidad.estado == E_VIVO) printf(" [PISTA]: Es demasiado fuerte para luchar de frente. Quizas desde otro lugar.\n");

            if (i == 5 && npc[i].esqueleto_entidad.estado == E_VIVO) printf(" Miras con pena a este ser caido. Sientes la necesidad de dar una humilde plegaria...\n");

            if(i == 9 && npc[i].esqueleto_entidad.estado == E_VIVO) printf(" [PISTA]: Esas sedas y telas raras parecen inflamables.\n");

            if(i == 11 && npc[i].esqueleto_entidad.estado == E_VIVO) printf(" [PISTA]: La fuerza mortal y humana no puede lidiar con esta clase de poder.. Se necesita un.. milagro.\n");

            return 1;
        }
    }


    for (int i = 0; i < contador_objetos; i++){

        if (posesion_objeto(i) && comparar_nombres_objetos(objetos[i].nombre, par)){

            if (strlen(objetos[i].ascii_art) > 0){

                mostrar_ascii_art(objetos[i].ascii_art);

                cmd_mirar();
            }

            return 1;
        }

    }
    printf(" No ves nada especial sobre <%s>\n", par);

    return 1;
}

//comando agarrar

int cmd_agarrar(const char *par){

        if (!par || strlen(par) == 0){
            printf(" Que quieres agarrar?\n");

            return 1;
        }

        if (comparar_nombres_objetos("llaves", par) && jugador.esqueleto_entidad.ubicacion == 5){
           agregar_al_inventario(LLAVES);
           objetos[1].ubicacion = -1;

           printf(" Agarras la <%s> y la guardas en el inventario.\n", objetos[1].nombre);
           pausa();
           cmd_mirar();

           return 1;
        }

        if ((comparar_nombres_objetos("botella", par) || comparar_nombres_objetos("whisky", par)) && jugador.esqueleto_entidad.ubicacion == 5){
            agregar_al_inventario(BOTELLA);
            objetos[2].ubicacion = -1;

            printf(" Agarras la <%s> y la guardas en el inventario.\n", objetos[2].nombre);
            pausa();
            cmd_mirar();

            return 1;
        }


        if (comparar_nombres_objetos("encendedor", par) && jugador.esqueleto_entidad.ubicacion == 8){
            agregar_al_inventario(ENCENDEDOR);
            objetos[3].ubicacion = -1;

            printf(" Agarras el <%s> y lo guardas en el inventario.\n", objetos[3].nombre);
            pausa();
            cmd_mirar();

            return 1;
        }

        if ((comparar_nombres_objetos("paquete", par) || comparar_nombres_objetos("cigarrillos", par)) && jugador.esqueleto_entidad.ubicacion == 8){
            agregar_al_inventario(CIGARRILLOS);
            objetos[4].ubicacion = -1;

            printf(" Agarras el <%s> y lo guardas en el inventario.\n", objetos[4].nombre);
            pausa();
            cmd_mirar();

            return 1;
        }
    printf(" No hay ningun <%s> en este lugar\n", par);

    return 1;
}

//comando abrir

int cmd_abrir(const char *par){

    if (!par || strlen(par) == 0){
        printf(" Que quieres abrir?\n");

        return 1;
    }

    salas_t *sala = &mundo[jugador.esqueleto_entidad.ubicacion];

    if (!posesion_objeto(LLAVES)){
        printf(" ===[PUERTA DE LAS CELDAS: CERRADA]===\n");

        return 1;
    }

    for (int i = 0; i < MAX_SALIDAS; i++){

        if (sala->salidas[i].sala_buscada != -1 && sala->salidas[i].bloqueada == CERRADO){

            int destino = sala->salidas[i].sala_buscada;
            sala->salidas[i].bloqueada = ABIERTO;

            for (int j = 0; j < MAX_SALIDAS; j++){

                if (mundo[destino].salidas[i].sala_buscada == jugador.esqueleto_entidad.ubicacion){
                    mundo[destino].salidas[j].bloqueada = ABIERTO;

                    break;
                }
            }
            printf(" ===[PUERTA DE LAS CELDAS: ABIERTA]===\n");

            pausa();
            cmd_mirar();

            return 1;
        }
    }
    printf(" No hay ninguna puerta que puedas abrir en esta sala.\n");

    return 1;
}

// comando atacar

int cmd_atacar(const char *par){

    //si no hay parametro, detecta al que esta en la sala
    if (!par || strlen(par) == 0){
        int enemigos_encontrados = 0;
        int enemigos = -1;

        for (int i = 0; i < contar_npcs; i++){

            if(npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO && npc[i].esqueleto_entidad.tipo == T_ENEMIGO){
                enemigos_encontrados++;
                enemigos = i;
            }
        }
        if (enemigos_encontrados == 0){
            printf(" No hay enemigos en las cercanias al cual atacar.\n");

            return 1;
        }
        if (enemigos_encontrados > 1){
            printf(" Hay enemigo/s en esta sala. Elije a quien atacar.\n");
            printf(" [ENEMIGOS PRESENTES]: \n");

            for (int i = 0; i < contar_npcs; i++){

                if (npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO && npc[i].esqueleto_entidad.tipo == T_ENEMIGO){
                    printf(" -%s\n", npc[i].esqueleto_entidad.nombre);
                }
            }
            printf("(usa el comando [atacar] seguido de [nombre del enemigo])\n");

            return 1;
        }

        printf(" Atacas al unico enemigo presente en la sala: <%s>\n", npc[enemigos].esqueleto_entidad.nombre);
        int i = enemigos;

        if (i == 1 || i == 2 || i == 3){
            printf(" Atacas a <%s>!\n", npc[i].esqueleto_entidad.nombre);
            printf(" Consigues derrotarlo con facilidad.\n");
            npc[i].esqueleto_entidad.estado = E_MUERTO;
            printf(" [%s]: %s\n\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);

            if (npc[1].esqueleto_entidad.estado == E_MUERTO && npc[2].esqueleto_entidad.estado == E_MUERTO && npc[3].esqueleto_entidad.estado == E_MUERTO){
                    mundo[1].salidas[DIR_OESTE].bloqueada = ABIERTO;
                    mundo[2].salidas[DIR_SUR].bloqueada = ABIERTO;
                    pausa();
                    limpiar_pantalla();
                    cmd_mirar();
                }

            return 1;
        }


        if (i == 4){
            printf(" Te lanzas al ataque en un intento desesperado!\n");

            pausa();

            printf(" Lamentableme el Guadian Mimir es demasiado fuerte y te atrapa con una mano...\n");
            printf(" Sientes como lentamente te comprime la cabeza con su agarre hasta que..\n");

            pausa();

            printf(" Te estalla la cabeza!\n");
            printf(" [%s]: Lo que faltaba! mas mugre a esta horrible sala!\n", npc[i].esqueleto_entidad.nombre);

            pausa();

            mostrar_escena_muerte();

            resetear_juego();

            return 1;
        }

        if (i == 6){

            if (jugador.esqueleto_entidad.ubicacion != 8){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }

            printf(" Derrotaste al <%s>!\n", npc[i].esqueleto_entidad.nombre);
            printf(" Fue tan facil que dio pena...\n");

            pausa();

            printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);
            npc[i].esqueleto_entidad.estado = E_MUERTO;

            return 1;
        }

        //Pelea con el paladin Rudigier
        if (i == 9){
            printf(" Te enfrentas a un gran maestro de la espada con tu escasa habilidad.. no aciertas ni un golpe..\n");
            printf(" [%s]: Maldita basura.. te atreviste a enfrentarme con este nivel? Esto es lo que te mereces.\n\n", npc[9].esqueleto_entidad.nombre);

            pausa();

            printf(" De un solo ataque.. Rudigier te cercena la cabeza\n");

            mostrar_escena_muerte();
            resetear_juego();

            return 1;
        }

        //Pelea con el jefe final
        if (i == 11){
            //Final 1? digamos que terminamos el juego.. pero tambien morimos?
            if (!jugador.bendicion){
                printf(" Sientes lentamente como se enfria el ambiente...\n");
                printf(" El calor, abandona de a poco tu cuerpo.\n");

                pausa();

                printf(" Y ves.. ves descender poco a poco.. a un Ser Divino!\n");
                printf(" La sola mirada a un ser de tal complejidad, hace que pierdas, poco a poco, tu cabeza.. tu razocinio...\n");
                printf(" Ya en el piso, comprendes que un mortal jamas podria acercarse a semejante magnitud.\n");
                printf(" Solo.. con frio.. y en la presencia de la Encarnacion de la Luna...\n");

                pausa();

                printf(" Finalmente mueres.\n");

                pausa();
                final_malo();

                return 0;
            }
            printf(" Sientes lentamente como se enfria el ambiente...\n");
            printf(" El calor, abandona de a poco tu cuerpo.\n");

            pausa();

            printf(" Y ves.. ves descender poco a poco.. a un Ser Divino!\n");

            pausa();

            printf(" Pero, cuando pensabas que ya estaba todo perdido.. tu espada comienza a brillar...\n");
            printf(" Se trata de la bendicion de Samael.\n");
            printf(" Momento a momento, recuperas el calor perdido en tu frio cuerpo y logras reunir fuerzas...\n");

            pausa();

            printf(" Un solo golpe al viento! solo eso basto, para liberar todo el esplendor contenido en la espada y darle a la Encarnacion...\n");
            printf(" Despues de golpearlo, caes al suelo sin fuerzas.. Y los agudos gritos de desesperacion y dolor de aquel Ser Divino te aturden...\n");
            printf(" Al cabo de un rato, los gritos paran.. La Encarnacion, calcinada.. hecha cenizas y polvo...\n");

            pausa();

            printf(" Lloras.. pero no de dolor, sino de un gran alivio porque.. conseguiste escapar de El Culto!\n");

            npc[11].esqueleto_entidad.estado = E_MUERTO;

            pausa();
            final_bueno();

            return 0;
        }

    }

    // pelea si hay como parametro un nombre

    for (int i = 0; i < contar_npcs; i++){

         if (comparar_nombres_entidades(npc[4].esqueleto_entidad.nombre, par) && npc[4].esqueleto_entidad.estado == E_VIVO){

                if (!oportunidad_guardian){
                    printf(" Te lanzas al ataque en un intento desesperado!\n");

                    pausa();

                    printf(" Lamentableme el Guadian Mimir es demasiado fuerte y te atrapa con una mano...\n");
                    printf(" Sientes como lentamente te comprime la cabeza con su agarre hasta que..\n");

                    pausa();

                    printf(" Te estalla la cabeza!\n");
                    printf(" [%s]: Lo que faltaba! mas mugre a esta horrible sala!\n", npc[i].esqueleto_entidad.nombre);

                    pausa();

                    mostrar_escena_muerte();

                    limpiar_pantalla();

                    resetear_juego();

                    return 1;
                }

                printf(" Agarras al <%s> de espaldas y le atraviesas el corazon!\n", npc[4].esqueleto_entidad.nombre);
                npc[4].esqueleto_entidad.estado = E_MUERTO;
                printf(" [%s]: %s\n\n", npc[4].esqueleto_entidad.nombre, npc[4].frase_muerte);

                printf(" ===[NUEVA UBICACION: %s]===\n", mundo[5].nombre);

                pausa();

                printf(" Con una gran preocupacion fuera del camino, ahora puedes recorrer el calabozo libremente.\n");
                mundo[5].salidas[DIR_OESTE].bloqueada = ABIERTO;
                objetos[1].ubicacion = 5;
                printf(" Te das vuelta para darle una segunda mirada al cuerpo del Guardian. Ves como caen unas llaves de sus bolsillos!! \n");
                oportunidad_guardian = 0;

                jugador.esqueleto_entidad.ubicacion = mundo[5].id;

                pausa();
                limpiar_pantalla();
                cmd_mirar();

                return 1;
            }

        if (comparar_nombres_entidades(npc[i].esqueleto_entidad.nombre, par) && npc[i].esqueleto_entidad.estado == E_VIVO && npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion){

            if(npc[i].esqueleto_entidad.tipo == T_NPC){
                printf(" No puedes atacar a <%s>..\n", npc[i].esqueleto_entidad.nombre);

                return 1;
            }

            if (!posesion_objeto(ESPADA_DESGASTADA) && !posesion_objeto(ESPADA_BENDITA)){
                printf(" Necesitas un arma para poder pelear.\n");

                return 1;
            }

            // pelea xurxess
            if (i == 1){
                if (jugador.esqueleto_entidad.ubicacion != 1){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }
                printf(" Atacas a <%s>!\n", npc[i].esqueleto_entidad.nombre);
                printf(" Consigues derrotarlo con facilidad\n");
                npc[i].esqueleto_entidad.estado = E_MUERTO;
                printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);

                if (npc[1].esqueleto_entidad.estado == E_MUERTO && npc[2].esqueleto_entidad.estado == E_MUERTO && npc[3].esqueleto_entidad.estado == E_MUERTO){
                    mundo[1].salidas[DIR_OESTE].bloqueada = ABIERTO;

                    pausa();
                    limpiar_pantalla();
                    cmd_mirar();
                }

                return 1;
            }

            //pelea julien
            if (i == 2){
                if (jugador.esqueleto_entidad.ubicacion != 1){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }
                printf(" Atacas a <%s>!\n", npc[i].esqueleto_entidad.nombre);
                printf(" Consigues derrotarlo con facilidad\n");
                npc[i].esqueleto_entidad.estado = E_MUERTO;
                printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);

                if (npc[1].esqueleto_entidad.estado == E_MUERTO && npc[2].esqueleto_entidad.estado == E_MUERTO && npc[3].esqueleto_entidad.estado == E_MUERTO){
                    mundo[1].salidas[DIR_OESTE].bloqueada = ABIERTO;

                    pausa();
                    limpiar_pantalla();
                    cmd_mirar();
                }

                return 1;

            }

            //pelea oscar
            if (i == 3){
                if (jugador.esqueleto_entidad.ubicacion != 1){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }
                printf(" Atacas a <%s>!\n", npc[i].esqueleto_entidad.nombre);
                printf(" Consigues derrotarlo con facilidad.\n");
                npc[i].esqueleto_entidad.estado = E_MUERTO;
                printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);

                if (npc[1].esqueleto_entidad.estado == E_MUERTO && npc[2].esqueleto_entidad.estado == E_MUERTO && npc[3].esqueleto_entidad.estado == E_MUERTO){
                    mundo[1].salidas[DIR_OESTE].bloqueada = ABIERTO;

                    pausa();
                    limpiar_pantalla();
                    cmd_mirar();
                }

                return 1;

            }

            //Pelea con Soldado novato Ramos
            if (i == 6){

                if (jugador.esqueleto_entidad.ubicacion != 8){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }

                if (npc[7].esqueleto_entidad.estado == E_VIVO){
                    printf(" Intentas atacar al <%s>!\n", npc[i].esqueleto_entidad.nombre);

                    pausa();

                    printf(" Pero el <%s> interviene y te hace un agujero en el abdomen...\n", npc[7].esqueleto_entidad.nombre);

                    printf(" [%s]: Maldito idiota, no reconocer al mas fuerte de los dos antes de atacar..\n", npc[7].esqueleto_entidad.nombre);
                    printf(" [%s]: Esto es lo que te mereces JAJAJA!\n", npc[7].esqueleto_entidad.nombre);

                    pausa();

                    printf(" [%s]: A-Asi se dice teniente! jajaja! -temblando del miedo-\n", npc[i].esqueleto_entidad.nombre);
                    printf(" [%s]: C-Creiste que podrias m-matarme?! Toma!!\n", npc[i].esqueleto_entidad.nombre);

                    pausa();

                    printf(" Ambos soldados comienzan a patearte mientras te desangras en el suelo...\n");

                    pausa();

                    mostrar_escena_muerte();

                    resetear_juego();

                    return 1;
                }
                printf(" Derrotaste al <%s>!\n", npc[i].esqueleto_entidad.nombre);
                printf(" Fue tan facil que dio pena...\n");

                pausa();

                printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);
                npc[i].esqueleto_entidad.estado = E_MUERTO;


                return 1;
            }

            //Pelea con el Teniene Samuel
            if (i == 7){

                if (jugador.esqueleto_entidad.ubicacion != 8){
                    printf(" <%s> no se encuentra en esta sala.\n", npc[i].esqueleto_entidad.nombre);

                    return 1;
                }

                printf(" Atacas al <%s>!\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" Luego de un rato combatiendo, logras desarmarlo!\n");
                printf(" Ahora el teniente yace de rodillas frente a ti...\n");

                pausa();

                printf(" [%s]: Que esperas! hazl..! -sonido de objeto cayendo-\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: %s\n\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);
                npc[i].esqueleto_entidad.estado = E_MUERTO;
                printf(" Decapitaste al <%s>...\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" De la gabardina del Teniente caen dos objetos.\n");
                objetos[3].ubicacion = mundo[8].id;
                objetos[4].ubicacion = mundo[8].id;

                pausa();
                cmd_mirar();

                return 1;
            }

            //Pelea con Rudigier
            if (i == 9){

                if (jugador.esqueleto_entidad.ubicacion != 11){
                    printf(" <%s> no se encuentra en esta sala...\n", par);

                    return 1;
                }
                printf(" Te enfrentas a un gran maestro de la espada con tu escasa habilidad.. no aciertas ni un golpe..\n");
                printf(" [%s]: Maldita basura.. ¿te atreviste a enfrentarme con este nivel? Esto es lo que te mereces..\n\n", npc[9].esqueleto_entidad.nombre);

                pausa();

                printf(" De un solo ataque.. Rudigier te cercena la cabeza\n");

                FILE* arch;
                arch = fopen("Moriste.txt", "r");
                if(arch == NULL){
                    printf(" [No se pudo cargar la escena muerte]\n");

                    return 1;
                }
                printf("\n");
                char linea[TAM_LINEA];

                while(fgets(linea, sizeof(linea), arch) != NULL) printf("%s", linea);
                printf("\n");

                pausa();
                fclose(arch);
                limpiar_pantalla();
                resetear_juego();

                return 1;
            }

            //Pelea Jefe Final
            if (i == 11){

                if (jugador.esqueleto_entidad.ubicacion != 19){
                    printf(" <%s> no se encuentra en esta sala...\n", par);

                    return 1;
                }
                //Final 1? digamos que terminamos el juego.. pero tambien morimos?
                if (!jugador.bendicion){
                    printf(" Sientes lentamente como se enfria el ambiente...\n");
                    printf(" El calor, abandona de a poco tu cuerpo.\n");

                    pausa();

                    printf(" Y ves.. ves descender poco a poco, a un Ser Divino!\n");
                    printf(" La sola mirada a un ser de tal complejidad, hace que pierdas, poco a poco, tu cabeza.. tu razocinio...\n");
                    printf(" Ya en el piso, comprendes que un mortal jamas podria acercarse a semejante magnitud.\n");
                    printf(" Solo.. con frio.. y en la presencia de la Encarnacion de la Luna.\n");

                    pausa();

                    printf(" Finalmente mueres...\n");

                    pausa();

                    final_malo();

                    return 0;
                }
                printf(" Sientes lentamente como se enfria el ambiente...\n");
                printf(" El calor, abandona de a poco tu cuerpo.\n");

                pausa();

                printf(" Y ves.. ves descender poco a poco.. a un Ser Divino!\n");

                pausa();

                printf(" Pero, cuando pensabas que ya estaba todo perdido.. tu espada comienza a brillar...\n");
                printf(" Se trata de la bendicion de Samael.\n");
                printf(" Momento a momento, recuperas el calor perdido en tu frio cuerpo y logras reunir fuerzas...\n");

                pausa();

                printf(" Un solo golpe al viento! solo eso basto, para liberar todo el esplendor contenido en la espada y darle a la Encarnacion.\n");
                printf(" Despues de golpearlo, caes al suelo sin fuerzas.. Y los agudos gritos de desesperacion y dolor de aquel Ser Divino te aturden.\n");
                printf(" Al cabo de un rato, los gritos paran.. La Encarnacion, calcinada.. hecha cenizas y polvo...\n");

                pausa();

                printf(" Lloras.. pero no de dolor, sino de un gran alivio porque.. conseguiste escapar de El Culto!\n");

                npc[11].esqueleto_entidad.estado = E_MUERTO;

                pausa();
                final_bueno();

                return 0;
            }


            if (jugador.esqueleto_entidad.ubicacion != npc[i].esqueleto_entidad.ubicacion){
                printf(" <%s> no se encuentra en esta sala.\n", par);

                return 1;
            }
            if(!posesion_objeto(ESPADA_DESGASTADA) && !posesion_objeto(ESPADA_BENDITA)){
                printf(" Necesitas un arma para poder pelear.\n");

                return 1;
            }

            printf(" Atacas a <%s>!\n", npc[i].esqueleto_entidad.nombre);
            npc[i].esqueleto_entidad.estado = E_MUERTO;
            printf(" [%s]: %s\n", npc[i].esqueleto_entidad.nombre, npc[i].frase_muerte);


            return 1;
        }
    }

    printf(" Esa persona no existe..\n");

    return 1;
}

//Comando usar

int cmd_usar(const char *par){

    if (!par || strlen(par) == 0){
        printf(" Que quieres usar?\n");

        return 1;
    }

    //cuando se quiera usar el molotov
    if (strstr(par, "molotov")){

        if (!posesion_objeto(MOLOTOV)){
            printf(" No tienes una bomba molotov..\n");

            return 1;
        }
        if (!posesion_objeto(ENCENDEDOR)){
            printf(" Necesitas un encendedor para poder prender el molotov..\n");

            return 1;
        }
        if (jugador.esqueleto_entidad.ubicacion == 11 && npc[9].esqueleto_entidad.estado == E_VIVO){
            printf(" Sacas el encendedor del inventario y enciendes la mecha del molotov.\n");
            printf(" Le lanzas el molotov rapidamente a Rudigier yy!\n");

            pausa();

            printf(" Lo destroza de un espadazo...\n");

            pausa();

            printf(" Pero le explota directo en la cara!\n");
            printf(" Rudigier pierde esa sonrisa burlona que tenia.. su arrogancia lo llevo a su perdicion...\n");
            npc[9].esqueleto_entidad.estado = E_MUERTO;
            printf(" [%s]: %s\n", npc[9].esqueleto_entidad.nombre, npc[9].frase_muerte);
            printf(" ===[ASCENSOR: LIBERADO]===\n");
            remover_del_inventario(MOLOTOV);

            pausa();
            cmd_mirar();

            return 1;
        }
        else {
            printf(" Gastar el molotov aqui seria inutil, espera a enfrentarte a Rudigier..\n");

            return 1;
        }
    }

    //cuando se quiera usar la pocion
    if (strstr(par, "brebaje") && posesion_objeto(BREBAJE)){

        if (jugador.esqueleto_entidad.ubicacion == 13 && npc[10].esqueleto_entidad.estado == E_VIVO && npc[10].dialogo == 0){
            printf(" Le das el brebaje medicinal al minero herido..\n");
            printf(" Al ver que le cuesta tomarlo, lo ayudas dandoselo directamente para que pueda beberlo con faciliad..\n");
            printf(" Sus heridas comienzan a cerrarse, y su cara, recupera color.. quedas asombrado ante el poder del brebaje, pues casi parece una obra de magia..\n\n");

            pausa();

            printf(" [%s]: Gra-Gracias por salvarme..\n", npc[10].esqueleto_entidad.nombre);
            printf(" [%s]: Soy el unico que queda...\n\n", npc[10].esqueleto_entidad.nombre);
            printf(" Inmovil y sin poder ayudar mas, ves como Sebastian entra en llanto y decides darle un momento\n");

            pausa();

            printf(" Ahora con los ojos secos y rojos de furia, lo ves distinto.. lo ves decidido\n\n");
            printf(" [%s]: Tenemos que salir de aqui.. ayudarme, y yo te ayudare a ti..\n", npc[10].esqueleto_entidad.nombre);
            printf(" [%s]: Yo era el jefe de la sala de maquinas, dejame ponerlas en marcha para que puedas moverte libremente por el area...\n\n", npc[10].esqueleto_entidad.nombre);
            printf(" Pasan unos momentos desde que Sebastian se puso en marcha.. esperas a que ponga en orden el lugar\n");

            pausa();

            printf(" De pronto escuchas a Sebastian, desde unas bocinas de la sala de descanso.. parece que quiere informarte de la situacion.\n\n");
            printf(" [%s]: He logrado poner todo en marchar. Ahora te toca a ti.. buena suerte...\n\n", npc[10].esqueleto_entidad.nombre);
            printf(" ===[EL SISTEMA DE RAILES Y VAGONES AH SIDO ACTIVADO]===\n");
            jugador.railes_activos = 1;
            strcpy(npc[10].esqueleto_entidad.descripcion, "Unico superviviente de la masacre en las minas.");
            npc[10].dialogo = 1;
            npc[10].esqueleto_entidad.ubicacion = 14;
            remover_del_inventario(BREBAJE);

            pausa();
            limpiar_pantalla();
            cmd_mirar();


            return 1;
        }
        else if (jugador.esqueleto_entidad.ubicacion == 13 && npc[10].dialogo == 1){
            printf(" Ya usaste el brebaje y no lo tienes mas en el inventario...\n");

            return 1;
        }
        else {
            printf(" Aqui no hay nadie herido que necesite del brebaje...\n");

            return 1;
        }
    }

    for (int i = 0; i < jugador.esqueleto_entidad.cantidad_items; i++){
        int id_objeto = jugador.esqueleto_entidad.inventario[i];

        if (strstr(objetos[id_objeto].nombre, par)){

            if (!objetos[id_objeto].usable){
                printf(" No puedes usar eso\n");

                return 1;
            }
            if (objetos[id_objeto].tipo == T_CONSUMIBLE){

                if (id_objeto == 8){
                    printf(" No necesitas el brebaje, no estas herido\n");

                    return 1;
                }
                printf(" Usas <%s>\n", objetos[id_objeto].nombre);

                if (objetos[id_objeto].consumible){
                    remover_del_inventario(id_objeto);
                }

                return 1;
            }
            if (objetos[id_objeto].tipo == T_LLAVE){
                printf(" Debes usar esto en una puerta especifica\n");

                return 1;
            }
            printf(" Usas <%s>\n", objetos[id_objeto].nombre);

            return 1;
        }
    }

    printf(" No tienes ese objeto\n");

    return 1;
}

//Comando hablar
int cmd_hablar(const char *par){
    //primero, por si el jugador no escribe un destinatario (parametro),
    //que el juego detecte si hay aalguien en la sala, en caso de haber varios,
    //mostrar una lista de los npc presentes
    if (!par || strlen(par) == 0){
        int npcs_encontrados = 0;
        int npcs = -1;

        for (int i = 0; i < contar_npcs; i++){

            if (npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO){
                npcs_encontrados++;
                npcs = i;
            }
        }

        if (npcs_encontrados == 0){
            printf(" No hay nadie con quien hablar aqui\n");

            return 1;
        }
        if (npcs_encontrados > 1){
            printf(" Hay varias personas en la sala, elije con quien..\n");
            printf(" [NPC PRESENTES]: \n\n");

            for (int i = 0; i < contar_npcs; i++){

                if (npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO){
                    printf(" - %s\n", npc[i].esqueleto_entidad.nombre);
                }
            }

            printf(" [CONSEJO]: hablar [nombre del npc]\n");


            return 1;
        }
        int i;
        if (npcs_encontrados > 1) i = npcs_encontrados;
        else i = npcs;

        if (npc[i].esqueleto_entidad.tipo == T_ENEMIGO){
            printf(" <%s> no gastara palabras contigo\n", npc[i].esqueleto_entidad.nombre);

            return 1;
        }

        printf(" Hablas con <%s>: \n", npc[i].esqueleto_entidad.nombre);


        switch (i){
            case 0:
                printf(" [%s]: N-no pense que despertarias! que? no recuerdas quien eres?\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" [%s]: Yo tampoco recuerdo quien soy... solo se, que ha pasado mucho tiempo desde que estoy aqui.. encerrado entre mugre y sangre.\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Intente escapar varias veces pero.. este viejo cuerpo ya no es lo que era.\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: En el ultimo intento, me dejaron en este lamentable estado.. apenas puedo moverme!\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" [%s]: Yo, yo ya me he rendido.. pero tu.. tu aun tienes luz! esperanza en tu ojos!\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Ten, toma esto. Ahora es inutil para alguien como yo, que se ha rendido...\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Es algo vieja, y esta desgastada.. pero creo que te servira.\n\n", npc[i].esqueleto_entidad.nombre);

                if (npc[i].esqueleto_entidad.cantidad_items > 0){
                     printf(" ===[RECIBES: ESPADA DESGASTADA]===\n");
                    agregar_al_inventario(ESPADA_DESGASTADA);
                    objetos[0].ubicacion= -1;
                    npc[i].esqueleto_entidad.cantidad_items = 0;

                    pausa();
                    cmd_mirar();
                }

                break;

            case 8:
                printf(" [%s]: Necesitas algo?\n\n", npc[i].esqueleto_entidad.nombre);
                printf(" [OPCIONES]: \n");
                printf("   1. Pedir brebaje medicinal\n");
                printf("   2. Pedir que fabrique un molotov\n");
                printf("   3. Nada\n");
                int opcion;

                do{
                    printf(" [ELIJA UNA OPCION]: ");
                    scanf("%d", &opcion);
                    limpiar_buffer();
                    printf("\n");
                    if (opcion < 1 || opcion > 3) printf(" Opcion no valida..\n");
                }while(opcion < 1 || opcion > 3);

                switch(opcion){
                    case 1:
                        if (!posesion_objeto(BREBAJE)){
                            printf(" [%s]: Sinceramente no pense que un sacrificio llegaria tan lejos luego de aquel viejo..\n", npc[i].esqueleto_entidad.nombre);
                            printf(" [%s]: Supongo, que como recompensa podria darte esto\n\n", npc[i].esqueleto_entidad.nombre);
                            printf(" ===[HAS RECIBIDO: BREBAJE MEDICINAL]===\n\n");
                            agregar_al_inventario(BREBAJE);
                            objetos[7].ubicacion = -1;

                            pausa();

                            printf(" [%s]: Puede que te parezca inutil ya que no te quedo ni una marca hasta ahora, pero quien sabe...\n", npc[i].esqueleto_entidad.nombre);
                            printf(" [%s]: Quizas te resulte util!\n\n", npc[i].esqueleto_entidad.nombre);
                        }
                        else printf(" [%s]: No puedo darte otro Brebaje. Se acabaron.\n\n", npc[i].esqueleto_entidad.nombre);

                        pausa();
                        cmd_mirar();

                        break;

                    case 2:

                        printf(" [%s]: Guau! no paras de sorprenderme eh..\n", npc[i].esqueleto_entidad.nombre);
                        fabricar_molotov();

                        pausa();
                        cmd_mirar();

                        break;

                    case 3:
                        printf(" [%s]: Si vos decis..\n", npc[i].esqueleto_entidad.nombre);

                        break;
                }

                break;

            case 10:

                if (npc[10].dialogo == 0){
                    printf(" [%s]: P-por favor.. A-ayudame...\n\n", npc[i].esqueleto_entidad.nombre);
                    printf(" [CONSEJO]: podrias darle el brebaje que te dio el Boticario\n");

                }
                else{
                    printf(" [%s]: Nuevamente, te doy las gracias. Sin ti, habria muerto...\n", npc[i].esqueleto_entidad.nombre);
                    printf(" [%s]: El sistema de railes esta funcionando. Buena suerte.\n", npc[i].esqueleto_entidad.nombre);
                }

                break;

            default:
                printf(" ...\n");
        }
        return 1;
    }

    // ahora para que reciba un nombre como parametro y hable con ese npc
    //(es exactamente lo mismo, solo cambia el modo de reconocer a quien hablar)
    for (int i = 0; i < contar_npcs; i++){

        if (npc[i].esqueleto_entidad.ubicacion == jugador.esqueleto_entidad.ubicacion && npc[i].esqueleto_entidad.estado == E_VIVO && comparar_nombres_entidades(npc[i].esqueleto_entidad.nombre, par)){

            if (npc[i].esqueleto_entidad.tipo == T_ENEMIGO){
            printf(" <%s> no gastara palabras contigo\n", npc[i].esqueleto_entidad.nombre);

            return 1;
        }

        printf(" Hablas con %s: \n", npc[i].esqueleto_entidad.nombre);

        switch (i){
            case 0:
                printf(" [%s]: N-no pense que despertarias! que? no recuerdas quien eres?\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" [%s]: Yo tampoco recuerdo quien soy... solo se, que ha pasado mucho tiempo desde que estoy aqui.. encerrado entre mugre y sangre.\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Intente escapar varias veces pero.. este viejo cuerpo ya no es lo que era.\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: En el ultimo intento, me dejaron en este lamentable estado.. apenas puedo moverme!\n", npc[i].esqueleto_entidad.nombre);

                pausa();

                printf(" [%s]: Yo, yo ya me he rendido.. pero tu.. tu aun tienes luz! esperanza en tu ojos!\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Ten, toma esto. Ahora es inutil para alguien como yo, que se ha rendido...\n", npc[i].esqueleto_entidad.nombre);
                printf(" [%s]: Es algo vieja, y esta desgastada.. pero creo que te servira.\n\n", npc[i].esqueleto_entidad.nombre);

                if (npc[i].esqueleto_entidad.cantidad_items > 0){
                     printf(" ===[RECIBES: ESPADA DESGASTADA]===\n");
                    agregar_al_inventario(ESPADA_DESGASTADA);
                    objetos[0].ubicacion = -1;
                    npc[i].esqueleto_entidad.cantidad_items = 0;

                    pausa();
                    cmd_mirar();
                }

                break;

            case 8:
                printf(" [%s]: Necesitas algo?\n\n", npc[i].esqueleto_entidad.nombre);
                printf(" [OPCIONES]: \n");
                printf("   1. Pedir brebaje medicinal\n");
                printf("   2. Pedir que fabrique un molotov\n");
                printf("   3. Nada\n");
                int opcion;

                do{
                    printf(" [ELIJA UNA OPCION]: ");
                    scanf("%d", &opcion);
                    limpiar_buffer();
                    printf("\n");
                    if (opcion < 1 || opcion > 3) printf("Opcion no valida..\n");
                }while(opcion < 1 || opcion > 3);

                switch(opcion){
                    case 1:
                       if (!posesion_objeto(BREBAJE)){
                            printf(" [%s]: Sinceramente no pense que un sacrificio llegaria tan lejos luego de aquel viejo..\n", npc[i].esqueleto_entidad.nombre);
                            printf(" [%s]: Supongo, que como recompensa podria darte esto\n\n", npc[i].esqueleto_entidad.nombre);
                            printf(" ===[HAS RECIBIDO: BREBAJE MEDICINAL]===\n\n");
                            agregar_al_inventario(objetos[7].id);
                            objetos[7].ubicacion = -1;

                            pausa();

                            printf(" [%s]: Puede que te parezca inutil ya que no te quedo ni una marca hasta ahora, pero quien sabe...\n", npc[i].esqueleto_entidad.nombre);
                            printf(" [%s]: Quizas te resulte util!\n\n", npc[i].esqueleto_entidad.nombre);
                        }
                        else printf(" [%s]: No puedo darte otro Brebaje. Se acabaron.\n\n", npc[i].esqueleto_entidad.nombre);

                        pausa();
                        cmd_mirar();

                        break;

                    case 2:
                        printf(" [%s]: Guau! no paras de sorprenderme eh..\n", npc[i].esqueleto_entidad.nombre);
                        fabricar_molotov();

                        pausa();
                        cmd_mirar();

                        break;

                    case 3:
                        printf(" [%s]: Si vos decis..\n", npc[i].esqueleto_entidad.nombre);

                        break;
                }

                break;

            case 10:

                if (npc[10].dialogo == 0){
                    printf(" [%s]: P-por favor.. A-ayudame...\n\n", npc[i].esqueleto_entidad.nombre);
                    printf(" [CONSEJO]: podrias darle el brebaje que te dio el Boticario\n");

                }
                else{
                    printf(" [%s]: Nuevamente, te doy las gracias. Sin ti, habria muerto...\n", npc[i].esqueleto_entidad.nombre);
                    printf(" El sistema de railes esta funcionando. Buena suerte\n");
                }

                break;

            default:
                printf(" ...\n");
        }

        return 1;
        }
    }
    printf( "No hay nadie con quien hablar en esta sala..\n");

    return 1;
}

// Comando rezar. para la interaccion con el Arcangel Samael
int cmd_rezar(){

    //primera vez rezando, obtenemos bendicion, arcangel se va
    if (jugador.esqueleto_entidad.ubicacion == 6 && npc[5].esqueleto_entidad.estado == E_VIVO){
        printf(" Te decides, con determinacion, a arrodillarte en este sucio y aspero suelo, a ofrecer una humilde plegaria.\n");
        printf(" Cierras los ojos y rezas con fervor, mientras te sangran las rodillas...\n");

        pausa();

        printf(" Tu fe, no flaquea. Se fortalece!\n");
        printf(" Ya profundo en el rezo, una voz comienza a hablarte por dentro.\n");

        pausa();

        printf(" [%s]: Joven creacion, tu que has sido traido a este lugar frio y oscuro..\n", npc[5].esqueleto_entidad.nombre);
        printf(" [%s]: Tu que hace tiempo deberias de haber perdido todo rastro de fe..\n", npc[5].esqueleto_entidad.nombre);
        printf(" [%s]: Tu que fuiste elegido Sacrificio! que deberias de albergar  ya cero esperanzas...\n", npc[5].esqueleto_entidad.nombre);

        pausa();

        printf(" [%s]: Te has mantenido firme, con devocion y fe. Incluso mas que yo!\n", npc[5].esqueleto_entidad.nombre);
        printf(" [%s]: Gracias a tu humilde plegaria, tu voz ha llegado a el Creador.. quien concedio fuerzas\n", npc[5].esqueleto_entidad.nombre);
        printf(" [%s]: para que cumpla un ultimo deber aqui en la tierra.\n", npc[5].esqueleto_entidad.nombre);

        printf(" [%s]: %s\n", npc[5].esqueleto_entidad.nombre, npc[5].frase_muerte);

        pausa();

        printf(" ===[HAS RECIBIDO: BENDICION DE SAMAEL]===\n");
        printf(" ===[OBJETO MODIFICADO: ESPADA DESGASTADA --> ESPADA ACLAMADA POR EL SOL]===\n\n");

        pausa();

        printf(" Poco a poco, la forma de Samael se desvanece, partiendo de este plano.\n");
        printf(" Ahora, solo queda una estatua en su nombre. \n");

        jugador.bendicion = 1;
        npc[5].esqueleto_entidad.estado = E_MUERTO;
        objetos[0].ubicacion = -1;
        remover_del_inventario(ESPADA_DESGASTADA);
        objetos[8].ubicacion = 1;
        agregar_al_inventario(ESPADA_BENDITA);

        pausa();
        cmd_mirar();

        return 1;
    }
    //Si rezas en otro lugar:
    printf(" Por alguna razon, eliges rezar en silencio...\n");


    //si Samael ya nos dio su bendicion:
    if (jugador.bendicion) {

        if (jugador.esqueleto_entidad.ubicacion == 6) printf(" Le rezas a la estatua Samael, pero tu plegaria no llega a ningun lado...\n");
        else printf(" No hay vida que merezca una plagaria en este lugar\n");
    }

    return 1;
}

//Comando inventario
int cmd_inventario(){
   mostrar_inventario();

   pausa();
   cmd_mirar();

   return 1;
}

//Comando ayuda.
//muestra al jugador informacion acerca de los comandos dentro del juego (cuales son y como usarlos)
//tambien da consejos basico acerca de como funciona y como deberia jugar el juego
int cmd_ayuda(){
    printf(" ===[COMANDOS DISPOBILES]===\n\n");
    printf("   ir       [direccion]   - Moverte (norte, sur, este, oeste, arriba, abajo)\n");
    printf("   mirar                  - Ver la sala actual\n");
    printf("   examinar [objetivo]    - Examina algo en detalle\n");
    printf("   agarrar  [objeto]      - agarras el objeto\n");
    printf("   abrir    [objetivo]    - Abre (si los hay) puertas\n");
    printf("   usar     [objeto]      - Usa un objeto disponible en el inventario\n");
    printf("   atacar   [enemigo]     - Ataca al enemigo seleccionado(Sin parametro --> detecta a los enemigos en la sala)\n");
    printf("   hablar   [npc]         - Habla con el npc seleccionado (Sin parametro --> detecta a los npc en la sala)\n");
    printf("   rezar                  - Da una plegaria a un necesitado\n");
    printf("   inventario             - Ves tu inventario\n");
    printf("   ayuda                  - Ves este mensaje\n");
    printf("   informacion            - Muestra informacion relevante del jugador\n");
    printf("   salir                  - Salir del juego (sin guardar)\n\n");

    pausa();
    cmd_mirar();

    return 1;
}

//comando informacion
//muestra detalles del jugador  (nombre, ubicacion, cant objetos, si esta bendito o si tiene el sistemas de vagonetas activo)

int cmd_informacion(){
    printf(" ===[INFORMACION DEL JUGADOR]===\n\n");
    printf(" [NOMBRE]: %s\n", jugador.esqueleto_entidad.nombre);
    printf(" [UBICACION]: %s\n", mundo[jugador.esqueleto_entidad.ubicacion].nombre);
    printf(" [OBJETOS]: %d/%d\n", jugador.esqueleto_entidad.cantidad_items, TAM_INVENTARIO);

    if (jugador.bendicion) printf(" --- BENDICION DE SAMAEL PRESENTE ---\n");
    if (jugador.railes_activos) printf(" --- SISTEMA DE RAILES ACTIVOS ---\n");

    pausa();
    cmd_mirar();

    return 1;
}
