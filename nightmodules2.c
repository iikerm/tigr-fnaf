// Para compilar se usa el texto de debajo:
// gcc menu_tigr.c -lGLU -lGL -lX11 -o menu_tigr

#include "tigr.h"
#include "tigr.c"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define lightGrey tigrRGBA(0xcc, 0xcc, 0xcc, 0xff)  // color normal del texto
#define clearWhite tigrRGBA(0xdd, 0xdd, 0xdd, 0xdd)     // color de drawStatic
#define clearGrey tigrRGBA(0x77, 0x77, 0x77, 0xdd)      // color de drawStatic

#define darkGrey tigrRGBA(0x44, 0x44, 0x44, 0xff)       // color de los bordes de la cámara

#define text "6 AM"
#define text2 "5:59 AM"

int readFile(const char *fname){
    FILE *animFile = fopen(fname, "r");
    int current_night=0;

    rewind(animFile);
    current_night = getc(animFile);
    fclose(animFile);

    return current_night;
}

void tigrNight(int night){
    int counter=0, ancho, alto, ventanaAlto, ventanaAncho;
    
    Tigr *screen = tigrWindow(300, 200, "Cameras", TIGR_FULLSCREEN);

    ventanaAlto = (screen->h)/2 - alto/2;
    ventanaAncho = (screen->w)/2 - ancho/2;

    while (!tigrClosed(screen) && counter < 100){
        counter++;
        tigrPrint(screen, tfont, ((screen->w)/2)-20, (screen->h)/2, lightGrey, text2);
        tigrUpdate(screen);
    }
    tigrClear(screen, tigrRGB(0, 0, 0));
    counter = 0;
    while (!tigrClosed(screen) && counter < 200){
        counter++;
        tigrPrint(screen, tfont, ((screen->w)/2)-8, (screen->h)/2, lightGrey, text);
        tigrUpdate(screen);
    }
    /*
    counter = 0;
    tigrFillRect(screen, -1, -1, screen->w+20, screen->h+20, tigrRGBA(0, 0, 0, 255));
    while (!tigrClosed(screen) && counter < 50){
        counter++;
        
        
        tigrUpdate(screen);
        
        
    }
    
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)){
        counter++;
        tigrPrint(screen, tfont, 10, (screen->h)-15, lightGrey, text);
        
        tigrUpdate(screen);
        
        
    }
    */

    tigrFree(screen);
}

int main(){
    int current_night = readFile("night.txt");
    tigrNight(current_night);
    return 0;
}
