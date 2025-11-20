#include <stdio.h>
#include <stdlib.h>
#include "utilidades.h"


const char* direccion_a_string(direcciones_t dir){
    switch(dir){
        case DIR_NORTE: return "norte";
        case DIR_SUR: return "sur";
        case DIR_ESTE: return "este";
        case DIR_OESTE: return "oeste";
        case DIR_ARRIBA: return "arriba";
        case DIR_ABAJO: return "abajo";
        default: return "ninguno";
    }
}

void mostrar_escena_muerte(){
    FILE* arch = fopen("Moriste.txt", "r");

    if(arch == NULL){
        printf("===[NO SE PUDO CARGAR LA ESCENA]===\n");
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

void final_malo(){

    limpiar_pantalla();
    printf(" =================================================================\n");
    printf("                   ALMA PRIVADA DE REENCARNACION\n");
    printf(" =================================================================\n");
    printf("\n");

    pausa();

    printf(" Felicitaciones?? Moriste!! pero por la mano de una deidad..\n");
    printf(" Tu alma fue marcada, por lo tanto.. no puedes regresar!!\n");
    printf(" Estas condenado a transitar eternamente en el purgatorio, y con el pasar de los siglos\n");
    printf(" perderas tu cordura. Y quien sabe! quizas hasta veas el final de los tiempos!\n");

    pausa();

    printf(" No me malentiendas.. terminaste el juego! de la peor manera jeje..\n\n");
    printf(" ===[FINAL MALO DESBLOQUEADO: ALMA PRIVADA DE REENCARNACION]===\n");
}

void final_bueno(){

    limpiar_pantalla();
    printf(" =================================================================\n");
    printf("                   LIBERACION DEL ALMA CONDENADA\n");
    printf(" =================================================================\n");
    printf("\n");

    pausa();

    printf(" Felicitaciones!! conseguiste escapar de ese lugar horrible!\n");
    printf(" Pero ahora, queda una incognita...\n");
    printf(" Quien eres? de donde vienes? quizas nunca lo sepas..\n");
    printf(" Pero eso queda en ti! de lo que te propongas.\n");


    pausa();

    printf(" Sinceramente no crei que fueras a escapar.. No me malentiendas!\n");
    printf(" Es que nadie ha logrado escapar de ese lugar condenado! pero tu lo hiciste!\n");
    printf(" Por lo tanto, tienes mi respeto.\n\n");
    printf(" ===[FINAL BUENO DESBLOQUEADO: LIBERACION DEL ALMA CONDENADA]===\n");
}

void limpiar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);

}

void limpiar_pantalla(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif // _WIN32
}

void pausa(){
    printf("\n");
    printf(" -- presiona ENTER para continuar --\n");
    getchar();
}
