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

/*
typedef struct {
    char text_night[5];
    int current_night;
} TNight;


void drawStatic(Tigr *screen){
    int i=0, width=screen->w, height=screen->h, rX=0, rY=0, rDim=0;
    
    srand(time(0));

    for(i=0; i<200; i++){
        rX = ((int)width*((float)rand())/((float)(RAND_MAX)));
        rY = ((int)height*((float)rand())/((float)(RAND_MAX)));
        rDim = (int)(((float)rand())/((float)(RAND_MAX))*40);
        if(i%2==0){
            tigrFillRect(screen, rX, rY, rDim, 3, clearGrey);
        }
        else{
            tigrFillRect(screen, rX, rY, rDim, 3, clearWhite);
        }
    }
}


void tigrNight(int night){
    int counter=0, ancho, alto, ventanaAlto, ventanaAncho;
    
    Tigr *screen = tigrWindow(300, 200, "Cameras", TIGR_FULLSCREEN);

    ventanaAlto = (screen->h)/2 - alto/2;
    ventanaAncho = (screen->w)/2 - ancho/2;

    while (!tigrClosed(screen) && counter < 200){
        counter++;
        tigrPrint(screen, tfont, ((screen->w)/2)-13, (screen->h)/2, lightGrey, text);
        tigrUpdate(screen);
    }
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
    

    tigrFree(screen);
}
*/

int readFile(const char *fname){
    FILE *animFile = fopen(fname, "r");
    int current_night=0;

    rewind(animFile);
    current_night = getc(animFile);
    fclose(animFile);

    return current_night;
}


void tigr12AM(int current_night){
    int counter=0, ancho, alto, ventanaAlto, ventanaAncho;
    
    Tigr *screen = tigrWindow(300, 200, "Starting...", TIGR_FULLSCREEN);

    ventanaAlto = (screen->h)/2 - alto/2;
    ventanaAncho = (screen->w)/2 - ancho/2;

    while (!tigrClosed(screen) && counter < 150){
        counter++;
        tigrPrint(screen, tfont, ((screen->w)/2)-22, (screen->h)/2, lightGrey, "Night %d", current_night);
        tigrPrint(screen, tfont, ((screen->w)/2)-18, ((screen->h)/2)+20, lightGrey, ("12 AM"));
        tigrUpdate(screen);
    }

    tigrFree(screen);
}

int main(){
    int current_night = readFile("night.txt");
    tigr12AM(current_night);
    return 0;
}