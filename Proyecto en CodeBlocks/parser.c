#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "comandos.h"

static void pasar_minuscula(char *string){
    for(int i = 0; string[i]; i++){
        string[i] = tolower((unsigned char)string[i]);
    }
}


//lee la entrada del jugador, pasa todo a minuscula y luego separa en verbo y parametro
void leer_comandos(comandos_t *cmd){
    char entrada[TAM_ENTRADA];
    printf("\n  > ");

    if(!fgets(entrada, sizeof(entrada),stdin)){
        cmd ->verbo[0] = '\0';
        return;
    }

    entrada[strcspn(entrada, "\n")] = '\0';
    pasar_minuscula(entrada);

    char *verb = strtok(entrada, " ");
    if(verb) strcpy(cmd->verbo, verb);
    else cmd->verbo[0] = '\0';

    char *par = strtok(NULL, " ");
    if(par) strcpy(cmd->parametro, par);
    else cmd->parametro[0] = '\0';
}

// despues de leer la entrada del usuario y verificar que este en la lista de comandos validos
// devuelve el comando a usar
int ejecutar_comando(const comandos_t *cmd){
    if(strlen(cmd->verbo) == 0) return 1;

    if(strcmp(cmd->verbo, "ir") == 0) return cmd_ir(cmd->parametro);
    if(strcmp(cmd->verbo, "mirar") == 0) return cmd_mirar();
    if(strcmp(cmd->verbo, "examinar") == 0) return cmd_examinar(cmd->parametro);
    if(strcmp(cmd->verbo, "agarrar") == 0) return cmd_agarrar(cmd->parametro);
    if(strcmp(cmd->verbo, "abrir") == 0) return cmd_abrir(cmd->parametro);
    if(strcmp(cmd->verbo, "atacar") == 0) return cmd_atacar(cmd->parametro);
    if(strcmp(cmd->verbo, "usar") == 0) return cmd_usar(cmd->parametro);
    if(strcmp(cmd->verbo, "hablar") == 0) return cmd_hablar(cmd->parametro);
    if(strcmp(cmd->verbo, "rezar") == 0) return cmd_rezar();
    if(strcmp(cmd->verbo, "inventario") == 0) return cmd_inventario();
    if(strcmp(cmd->verbo, "ayuda") == 0) return cmd_ayuda();
    if(strcmp(cmd->verbo, "informacion") == 0) return cmd_informacion();

    if (strcmp(cmd->verbo, "salir") == 0) return 1;

    printf(" <%s> no es un comando valido.\n", cmd->verbo);

    return 1;
}
