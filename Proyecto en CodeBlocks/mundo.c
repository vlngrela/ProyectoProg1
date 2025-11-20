#include "mundo.h"
#include <string.h>

salas_t mundo[MAX_SALAS];
int contador_salas = 0;


void inicializar_mundo(){

/* ===[ESTRUCTURA]===

    MUNDO[i] = (SALAS_T){
        ID,
        NOMBRE,
        DESCRIPCION,
        PISO,
        <CONEXIONES[i]>
        {
            DESCRIPCION/NOMBRE,
            SALA BUSCADA, (seria como un id)
            BLOQUEADA/ABIERTA
        },
    };
*/
    // piso "1" (0)
    mundo[0] = (salas_t){
        0,
        "Almacen de Sacrificios",
        "Un lugar humedo, maloliente y triste.\n No se molestan en limpiar las docenas de cuerpos que yacen aqui tirados.",
        0,
            {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"camino hacia el Altar", 1, ABIERTO},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {"puerta a sala comun",2, CERRADO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[1] = (salas_t){
        1,
        "Altar del Culto",
        "Lugar de adoracion de los cultistas.\n Las paredes y pisos estan cubiertas de rojo, parece que hubo actvidad recientemente.",
         0,
         {
            [DIR_NORTE] = {"camino al almacen", 0, ABIERTO},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {"pasillo a la sala comun", 2, CERRADO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[2] = (salas_t){
        2,
        "Sala Comun",
        "Conecta a varios lugares.\n Incomoda lo bien decorado que esta el lugar, como si no pasara nada en las salas contiguas.",
         0,
         {
            [DIR_NORTE] = {"puerta al almacen", 0, ABIERTO},
            [DIR_SUR] = {"pasillo al fondo del altar", 1, ABIERTO},
            [DIR_ESTE] = {"sendero de la audiencia", 3, CERRADO},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={"escaleras de la Orden", 4, ABIERTO},
            [DIR_ABAJO]={0}
        }
    };
    mundo[3] = (salas_t){
        3,
        "Recamara del Santisimo",
        "Habitacion de descanso de la figura de maxima exponencia en el culto.",
        0,
        {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {"sendero de la audiencia", 2, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };

    // piso "2" (1)
     mundo[4] = (salas_t){
        4,
        "Escaleras",
        "Conducen a los mas profundo",
         1,
         {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {"puerta Este del calabozo", 5, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={"escaleras a las cavernas", 2, ABIERTO}
        }
    };
    mundo[5] = (salas_t){
        5,
        "Calabozo",
        "Carcel construida por la orden.\n El lugar se cae a los pedazos, ratas y otros bichos rondan por el suelo como si nada.",
        1,
        {
            [DIR_NORTE] = {"puerta a las celdas", 6, CERRADO},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"puerta Este del calabozo", 4, CERRADO},
            [DIR_OESTE] = {"camino al centro de la orden", 7, CERRADO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[6] = (salas_t){
        6,
        "Celdas",
        "El segundo peor lugar despues del almacen.\n Se escuchan gritos por todas partes y la sangre fluye por el piso a montones.\n Los encerrados aqui ya no presentan luz en sus ojos, parecen.. cascaras.",
        1,
        {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"puerta al calabozo", 5, ABIERTO},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[7] = (salas_t){
        7,
        "Salon de la Convergencia",
        "Una maravilla arquitectonica.\n El lugar se encuentra en el fondo de un crater en el centro de una montania inaccesible.\n Por el centro se vez como la luz lunar irraiga el lugar e ilumina las paredes, en las cuales\n hay numerosas construcciones.",
        1,
        {
            [DIR_NORTE] = {"pasillo norte", 8, ABIERTO},
            [DIR_SUR] = {"pasillo Sur", 9, ABIERTO},
            [DIR_ESTE] = {"puerta Oeste del calabozo", 5, ABIERTO},
            [DIR_OESTE] = {"salon de ascensores", 11, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[8] = (salas_t){
        8,
        "Barracas de la Orden",
        "Lugar de descanso de los herejes.\n Un pasillo largo con varias habitaciones por lado.\n Se ve basnte normal para ser algo hecho por ellos...",
        1,
        {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"pasillo Norte", 7, ABIERTO},
            [DIR_OESTE] = {"camino del pensamiento", 9, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[9] = (salas_t){
        9,
        "Jardin Interior",
        "Esta lleno de color y vida. Por todas partes, hay variedad de flores y plantas nunca antes vistas, siendo cultivadas con esmero.\n Duro contraste con el resto de lugares.",
        1,
        {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"entrada a la botica", 10, ABIERTO},
            [DIR_ESTE] = {"pasillo Sur", 7, ABIERTO},
            [DIR_OESTE] = {"camino del pensamiento", 8, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[10] = (salas_t){
        10,
        "Botica del Bastion",
        "Esta habitacion presenta un agradable olor a madera y hierbas.\n Todos los articulos estan enfermamente bien organizados...\n Al fonde parece haber alguien.",
        1,
        {
            [DIR_NORTE] = {"salida al jardin interior", 9, ABIERTO},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[11] = (salas_t){
        11,
        "Gran Elevador",
        "Salida del refugio del culto, parece resguardado por alguien",
        1,
        {
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"salon de la Convergencia", 7, ABIERTO},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={"ascenso al area minera", 12, ABIERTO},
            [DIR_ABAJO]={0}
        }
    };

    // piso "3" (2)
    mundo[12] = (salas_t){
        12,
        "Gran Elevador, Area minera",
        "Tunel escarvado por mineros desafortunados, pues al derrumbarse una pared de la \n sala de descanso, encontraron el camino a su perdicion.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"tunel a la sala de descanso",13,ABIERTO},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={"descenso a la guarida de la Orden",11, ABIERTO}
        }
    };
    mundo[13] = (salas_t){
        13,
        "Sala de Descanso",
        "Sala nexo del area minera, se encuentra muy bien organizada y establecida.\n Esta repleta de los cuerpos de los mineros masacrados. Ni siquiera pudieron defenderse.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {"railes al tunel de joyas",15,ABIERTO},
            [DIR_SUR] = {"entrada a la sala de maquinas",14,CERRADO},
            [DIR_ESTE] = {"pasillo a la sala de control",16,ABIERTO},
            [DIR_OESTE] = {"tunel a los ascensores",12, CERRADO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[14] = (salas_t){
        14,
        "Sala de Maquinas",
        "Lugar de almacenamiento de todas las herramientas de mineria y metalurgia.\n Tambien es la sala en la que el jefe de los maquinistas gestionaba el uso de los rieles.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {"puerta a la sala de descanso",13,ABIERTO},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"pasillo a la sala de control",16,ABIERTO},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[15] = (salas_t){
        15,
        "Tunel de Joyas",
        "Es la razon principal por la cual comenzo una excavacion en un area tan remota.\n Esta repleta de joyas que cubre todo el espectro de colores.\n Seria una vista hermosa si no fuera por la excavacion excesiva.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"railes a la sala de descanso",13,ABIERTO},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[16] = (salas_t){
        16,
        "Sala de Control",
        "Cuarto de monitoreo a cargo del lider de la excavacion.\n Por aqui pasaban todos los trabajadores para corroborar su condicion...\n Y verificar que no se hayan robado nada.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {"pasillo a la sala de descanso",13,ABIERTO},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"railes al salon de vagonetas",17,ABIERTO},
            [DIR_OESTE] = {"pasillo a la sala de maquinas",14, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };
    mundo[17] = (salas_t){
        17,
        "Salon de Vagonetas",
        "Entrada y salida del area minera.\n Presenta un increible sistema de movimiento ultra eficiente, capaz de mover a los mineros, los recursos, y a\n todas las maquinarias de un sitio a otro de forma rapida y sencilla.",
        2,
        {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {"railes a la sala de control",16,ABIERTO},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={"railes a la entrada", 18,ABIERTO},
            [DIR_ABAJO]={0}
        }
    };

    // piso "4" (3)
    mundo[18] = (salas_t){
        18,
        "Afueras del area minera",
        "Aire frio y puro, se ve como la luna ocupa todo el cielo.",
        3,
        {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {"sendero de tierra",19, ABIERTO},
            [DIR_OESTE] = {0},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={"entrada a la mina",17,ABIERTO}
        }
    };
     mundo[19] = (salas_t){
         19,
         "El Valle",
         "Por alguna razon, el ambiente se hizo muy frio de repente.\n Se puede sentir como con cada respiracion de congelan los pulmones.",
         3,
         {
            //conexiones
            [DIR_NORTE] = {0},
            [DIR_SUR] = {0},
            [DIR_ESTE] = {0},
            [DIR_OESTE] = {"sendero de tierra",18, ABIERTO},
            [DIR_ARRIBA]={0},
            [DIR_ABAJO]={0}
        }
    };

    contador_salas = 20;
}
