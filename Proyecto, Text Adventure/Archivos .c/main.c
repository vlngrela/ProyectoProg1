#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "mundo.h"
#include "entidades.h"
#include "objetos.h"
#include "comandos.h"
#include "utilidades.h"

#define LIM_MIN_MENU 0
#define LIM_MAX_MENU 2
#define BUCLE -1
#define SALIR_BUCLE 0
#define PARTIDA_EN_CURSO 1
#define SALIR_JUEGO 0

//declaracion de funciones
void mostrar_bienvenida();
void mostrar_introduccion();
void inicio();
int menu(int, int);
void mostrar_creditos();



int main() {
    mostrar_bienvenida();

    limpiar_pantalla();

    int bucle = BUCLE;
    int partida = PARTIDA_EN_CURSO;
    int opc;

    do{
        opc = menu(LIM_MIN_MENU, LIM_MAX_MENU);

        switch (opc){

            case JUGAR:
                limpiar_pantalla();

                // los clavo aca asi no repito codigo al pedo y por si el jugador salio de la partida y vuelve a jugar se reinicia todo (distinto a resetear_juego)
                inicializar_mundo();
                inicializar_jugador();
                inicializar_npcs();
                inicializar_objetos();

                partida = PARTIDA_EN_CURSO;

                comandos_t cmd;
                char confirmar_salida;

                mostrar_introduccion();
                limpiar_pantalla();
                inicio();
                cmd_mirar();

                while(partida){
                    leer_comandos(&cmd);
                    printf("\n");

                    if (strcmp(cmd.verbo, "salir") == 0){
                        printf(" Estas seguro de querer salir? [s/n]: ");
                        scanf(" %c", &confirmar_salida);
                        limpiar_buffer();
                        printf("\n");

                        if (confirmar_salida == 's' || confirmar_salida == 'S'){
                            printf(" ---[VOLVIENDO AL MENU PRINCIPAL]---\n");
                            printf("         Gracias por jugar!\n");

                            pausa();

                            partida = SALIR_JUEGO;

                        }
                        else printf(" ---[CONTINUANDO LA PARTIDA]---\n");

                        continue;
                    }

                    int accion = ejecutar_comando(&cmd);

                    if (accion == 0){
                        pausa();

                        partida = SALIR_JUEGO;
                    }
                }

                break;

            case CREDITOS:
                limpiar_pantalla();

                mostrar_creditos();

                break;

            case SALIR:
                limpiar_pantalla();

                printf(" =================================================================\n");
                printf("                         GRACIAS POR JUGAR!\n");
                printf(" =================================================================\n");
                printf("\n");

                bucle = SALIR_BUCLE;

                break;
        }
    }while(bucle != SALIR_BUCLE);

    return 0;
}

//para dar bienvenida al jugador y explicar de donde proviene el genero
//del juego que estan por jugar
void mostrar_bienvenida(){
    limpiar_pantalla();
    FILE* arch;
    arch = fopen("intro2.txt", "r");
     if(arch == NULL){
        printf(" [No se pudo cargar la intro\n]");
        return;
    }
    printf("\n");
    char linea[TAM_LINEA];

    while(fgets(linea, sizeof(linea), arch) != NULL) printf("%s", linea);
    printf("\n");

    pausa();

    fclose(arch);

    limpiar_pantalla();

    printf(" =================================================================\n");
    printf("        BIENVENIDOS AL TEXT ADVENTURE: ESCAPE FROM THE CULT\n");
    printf(" =================================================================\n");
    printf("\n");

    printf(" El origen de los Text Adventure (aventura conversacional) se remonta a la decada \n");
    printf(" de los 70. Practicamente en los inicios de los Videojuegos como forma de entretenimiento para las masas.\n");
    printf(" Este genero consta de un modo de juego muy basico, donde el jugador debe usar comandos de texto\n");
    printf(" para controlar al personaje e interactuar con un entorno simulado (mediante graficos o texto).\n\n");

    printf(" En este proyecto habra imagenes de los objetos que hay esparcidos por el mundo, ya que\n");
    printf(" es un requisito del mismo incluir archivos en el proyecto.\n");
    printf(" Todo lo demas, que tenga descripciones detalladas, es para que lo puedan visualizar con su imaginacion!!\n");
    printf(" Y eso es lo bueno de este genero, cada persona tendra una experiencia y vision distinta de cada escenario\n");
    printf(" y de cada personaje que se encontraran en el juego.\n");
    printf(" -----------------------------------------------------------------\n\n");

   pausa();
}

void mostrar_introduccion(){

    printf(" =================================================================\n");
    printf("                           P R O L O G O\n");
    printf(" =================================================================\n");
    printf("\n");

    printf(" Fuiste secuetrado por un culto que adora a la luna. Traido para\n");
    printf(" ser usado como un sacrificio mas. De tu sangre y de la de los demas sacrificios,\n");
    printf(" los cultistas podran darle acceso a todo su poder a su ser adorado.\n");
    printf(" Ya sea por suerte o por arrogancia de los herejes, tus ataduras se sueltan.\n");

    pausa();
    limpiar_pantalla();

    printf(" =================================================================\n");
    printf("                       CONSEJOS/ADVERTENCIAS\n");
    printf(" =================================================================\n");
    printf("\n");

    printf(" [MORIS DE UN SOLO GOLPE]: tu cuerpo es muuy debil, cualquier herida y adios.\n\n");
    printf(" [NO HAY GUARDADO]: si moris, comenzas desde el principio\n\n");
    printf(" [CUIDADO AL PELEAR]: no todo se resuelve con la espada, a veces hay condiciones para la victoria.\n\n");
    printf(" [EXPLORA Y EXAMINA]: a veces, los detalles dan pistas para avanzar.\n\n");
    printf(" [AYUDA]: si tenes problemas, escribi <ayuda> para ver la lista de comandos y sus funciones.\n\n");
    printf(" [QUEJAS]: hablen con Valentin Grela.\n\n");

    pausa();
}

//para dar un poco de ambientacion
void inicio(){

    printf(" Despiertas en la oscuridad. El olor a humedad y cuerpos podridos te\n");
    printf(" sacude la cabeza completamente. Tus manos, atadas. Por suerte para ti\n");
    printf(" ni se molestaron en atarte adecuadamente.\n");

    pausa();

    printf(" Al poco tiempo, tus ojos se acostumbran a la falta de luz.\n");
    printf(" Te encuentras en una caverna. A tu alrededor, cuerpos.\n");
    printf(" Decenas de cuerpos, pudriendose. Parecen haber sido sacrificados.\n");

    pausa();

    printf(" Mientras mas observas la sala, te encuentras con un sacrificio diferente.\n");
    printf(" Te encuentras con un sacrificio aun con vida. Si con vida puede llamarse\n");
    printf(" a alguien con su condicion.\n");
    printf(" Sus ojos, se encuentran con los tuyos. Todo en esta oscuridad.\n\n");

    pausa();

    printf(" [%s]: Tu.. tu eres nuevo... -susurra con una voz aspera y quebrada-\n", npc[0].esqueleto_entidad.nombre);
    printf(" [%s]: Bienvenido al infierno mismo! Sacrificio... -te dice, mientras rie siniestramente-\n", npc[0].esqueleto_entidad.nombre);

    pausa();
    limpiar_pantalla();
}

//muestra el menu y tambien devuelve la opcion elegida
int menu(int limite_minimo, int limite_maximo){
    limpiar_pantalla();
    int opcion;

    printf(" *==========================================*\n");
    printf(" |                                          |\n");
    printf(" |           ESCAPE FROM THE CULT           |\n");
    printf(" |                                          |\n");
    printf(" |              MENU PRINCIPAL              |\n");
    printf(" |                                          |\n");
    printf(" *==========================================*\n");
    printf("\n");
    printf("  %d. Jugar\n", JUGAR);
    printf("  %d. Creditos\n", CREDITOS);
    printf("  %d. Salir\n", SALIR);
    printf("\n");
    printf(" *==========================================*\n");

    do{
        printf(" [ELIJE UNA OPCION]: ");
        scanf("%d", &opcion);
        limpiar_buffer();
        if(opcion < limite_minimo || opcion > limite_maximo) printf(" Opcion invalida, intente nuevamente..\n");
    }while(opcion < limite_minimo || opcion > limite_maximo);

    return opcion;
}

//seria como la "presentacion" de un trabajo practico normal
void mostrar_creditos(){

    printf(" ================================================================\n");
    printf("                             CREDITOS\n");
    printf(" ================================================================\n");
    printf("\n");

    printf(" [PROYECTO FINAL]: Programacion\n");
    printf(" [TEMA PROYECTO]: Text Adventure: Escape From The Cult\n\n");

    printf(" [ALUMNO]: Grela Gutierrez Valentin\n");
    printf(" [FACULTAD]: UTN-INSPT\n");
    printf(" [CICLO]: 2025\n\n");

    printf("               ===[ESPECIFICACIONES DEL PROYECTO]===\n\n");

    printf("  - [LENGUAJE]: C\n");
    printf("  - [MODELADO DEL MAPA]: Grafos y matriz de adyacencia\n");
    printf("  - [ESTRUCTURAS DE DATOS]: salas y pisos, entidades y objetos\n");
    printf("  - [ARCHIVOS]: Ascii art en .txt\n");
    printf("  - [JUGABILIDAD]: Parser con comandos simples\n\n");

    printf(" ================================================================\n");

    pausa();
}

