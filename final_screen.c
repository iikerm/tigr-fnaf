// Para compilar se usa el texto de debajo:
// gcc menu_tigr.c -lGLU -lGL -lX11 -o menu_tigr

#include "tigr.h"
#include "tigr.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define lightGrey tigrRGBA(0xcc, 0xcc, 0xcc, 0xff)  // color normal del texto
#define clearWhite tigrRGBA(0xdd, 0xdd, 0xdd, 0xdd)     // color de drawStatic
#define clearGrey tigrRGBA(0x77, 0x77, 0x77, 0xdd)      // color de drawStatic

#define darkGrey tigrRGBA(0x44, 0x44, 0x44, 0xff)       // color de los bordes de la cámara

// Definición de los diferentes textos que se utilizarán en la pantalla
#define exittext "Press ESC to exit game"
#define text0 "BRUH..."
#define text1 "NOT BAD!"
#define text2 "VERY GOOD!"
#define text3 "EXCELLENT!!"

// Módulo que lee el valor de la puntuación
int readFile(const char *fname){
    FILE *animFile = fopen(fname, "r");
    int points=0;

    rewind(animFile);
    points = getc(animFile);
    fclose(animFile);

    return points;
}

// Módulo que dibuja la pantalla final
void tigrCamera(int punctuation){
    int ancho, alto, ventanaAlto, ventanaAncho;
    Tigr *screen = tigrWindow(300, 200, "Cameras", TIGR_FULLSCREEN);
    Tigr *image = tigrLoadImage("assets/star.png");

    if (!image){    // Si no se encuentra la imagen se muestra un mensaje de error
        tigrError(screen, "Some images not found, make sure you have downloaded the 'assets' folder");
    }

    ancho = image->w;
    alto = image->h;

    ventanaAlto = (screen->h)/2 - alto/2;
    ventanaAncho = (screen->w)/2 - ancho/2;

    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)){
        
        // Se dibujan las tres estrellas
        tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
        tigrBlitAlpha(screen, image, ventanaAncho - 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
        tigrBlitAlpha(screen, image, ventanaAncho + 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
        
        // Se les da una capa negra para que se vean oscuras
        tigrFillRect(screen, -1, -1, screen->w+20, screen->h+20, tigrRGBA(0, 0, 0, 200));
        
        // Se comprueba el valor de la puntuación
        if (punctuation < 100) {    // Si la puntuación es menor que 100 no se dibujan estrellas y el texto es 0
            tigrPrint(screen, tfont, 134, (screen->h)-160, lightGrey, text0);
        } else if (punctuation >= 100 && punctuation < 350) {   // Si 100 > puntuación > 350 se dibuja una estrella y el texto es 1
            tigrPrint(screen, tfont, 125, (screen->h)-160, lightGrey, text1);
            tigrBlitAlpha(screen, image, ventanaAncho - 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
        } else if (punctuation >= 350 && punctuation < 750) {   // Si 350 > puntuación > 750 se dibujan dos estrellas y el texto es 2
            tigrPrint(screen, tfont, 118, (screen->h)-160, lightGrey, text2);
            tigrBlitAlpha(screen, image, ventanaAncho - 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
            tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
        } else {    // Si puntuación > 750 se dibujan tres estrellas y el texto es 3
            tigrPrint(screen, tfont, 116, (screen->h)-160, lightGrey, text3);
            tigrBlitAlpha(screen, image, ventanaAncho - 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
            tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
            tigrBlitAlpha(screen, image, ventanaAncho + 85, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
        }

        tigrPrint(screen, tfont, 10, (screen->h)-15, lightGrey, exittext);
        
        tigrUpdate(screen);
        
        
    }

    tigrFree(image);
    tigrFree(screen);
}

int main(){
    int punctuation; 
    punctuation = readFile("finish.txt");
    tigrCamera(punctuation); 
    return 0;
}