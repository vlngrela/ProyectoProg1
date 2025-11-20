#ifndef PARSER_H
#define PARSER_H

#define TAM_ENTRADA 100
#define TAM_VERBO 32
#define TAM_PARAMETRO 32

typedef struct{
    char verbo[TAM_VERBO];
    char parametro[TAM_PARAMETRO];
}comandos_t;

void leer_comandos(comandos_t *cmd);
int ejecutar_comando(const comandos_t *cmd);

#endif //PARSER_H
