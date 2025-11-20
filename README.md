En  la carpeta proyecto dejo todos los archivos en 3 carpetas distintas para los 3 tipos de archivos que hay (.c, .h, .txt), en la carpeta juego solo esta el .exe y los .txt para jugar de una vez.

El png grafos muestra la distribucios de los pisos y las salas, y un intento (horrible) por mostrar como se establece una matriz de adyacencia.

Si va a compilar todo para ir probandolo le recomiendo tener los .txt en la misma carpeta que el .txt, de otra forma tendra que cambiar el campo correspondiente al nombre los archivos en objetos.c a la carpeta donde los guarde.

[PARA COMPILAR]: gcc main.c mundo.c comandos.c objetos.c entidades.c parser.c utilidades.c -o escapeFromTheCult.exe
