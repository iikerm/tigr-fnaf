#include "tigr.h"
#include "tigr.c"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define lightGrey tigrRGBA(0xcc, 0xcc, 0xcc, 0xff)      // color normal del texto
#define clearWhite tigrRGBA(0xdd, 0xdd, 0xdd, 0xdd)     // color de drawStatic
#define clearGrey tigrRGBA(0x77, 0x77, 0x77, 0xdd)      // color de drawStatic
#define darkGrey tigrRGBA(0x44, 0x44, 0x44, 0xff)       // color de los bordes de la cámara

#define exittext "Press ESC to close cameras"

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

int readFile(const char *fname){
    FILE *animFile = fopen(fname, "r");
    int animatronic_left=0;

    rewind(animFile);
    animatronic_left = getc(animFile);
    fclose(animFile);

    return animatronic_left;
}

void tigrCamera(){
    int count=0, ancho, alto, ventanaAlto, ventanaAncho, randomAnim;
    int animatronic_left = readFile("anim.txt");
    Tigr *screen = tigrWindow(300, 200, "Cameras", TIGR_FULLSCREEN);
    Tigr *image = tigrLoadImage("assets/alfredo.png");
    Tigr *image2 = tigrLoadImage("assets/alfredo_dorado.png");
    Tigr *image3 = tigrLoadImage("assets/alfredo_plateado.png");
    Tigr *fondo = tigrLoadImage("assets/fondofnaf3.png");

    if (!image || !image2 || !image3 || !fondo){
        tigrError(screen, "Some images not found, make sure you have downloaded the 'assets' folder");
    }
    int mx=0, my=0, left=0, right=0, center=0, buttons=0;

    if (animatronic_left!=3){
        ancho = image->w;
        alto = image->h;

        ventanaAlto = (screen->h)/2 - alto/2;
        ventanaAncho = (screen->w)/2 - ancho/2;

        while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)){
            count++;

            tigrBlitAlpha(screen, fondo, 0, 0, 0, 0, 300, 200, 100.0);
            switch (animatronic_left){
                case 0:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
                case 1:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    break;
                case 2:
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
                default:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
            }

            tigrFillRect(screen, -1, -1, screen->w+20, screen->h+20, tigrRGBA(0, 0, 0, 150));
            drawStatic(screen);
            if (count <= 20){
                tigrFillCircle(screen, (screen->w)-tigrTextWidth(tfont, "REC")-7, 5, 5, tigrRGB(255, 0, 0));
                tigrPrint(screen, tfont, ((screen->w)-tigrTextWidth(tfont, "REC")), 1, lightGrey, "REC");
            }else{
                if (count>40) {
                    count =0;
                }
            }
            
            tigrPrint(screen, tfont, 10, (screen->h)-10, lightGrey, exittext);
            tigrFillRect(screen, 2, 2, (screen->w)-4-(tigrTextWidth(tfont, "REC")+12), 5, darkGrey);  // Linea horizontal superior
            tigrFillRect(screen, (tigrTextWidth(tfont, exittext))+11, (screen->h)-7, (screen->w)-4-(tigrTextWidth(tfont, exittext)+11), 5, darkGrey);  // Línea horizontal inferior
            tigrFillRect(screen, 2, 5, 5, ((screen->h)-7), darkGrey);
            tigrFillRect(screen, (screen->w)-7, 2+(tigrTextHeight(tfont, "REC")), 5, ((screen->h)-7)-9, darkGrey);
            tigrUpdate(screen);
            
            
        }
    } else {
        randomAnim = ((float)rand() / (float)RAND_MAX) * (int)8;        // says what animatronic is not there
        ancho = image->w;
        alto = image->h;

        ventanaAlto = (screen->h)/2 - alto/2;
        ventanaAncho = (screen->w)/2 - ancho/2;

        while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)){
            count++;

            tigrBlitAlpha(screen, fondo, 0, 0, 0, 0, 300, 200, 100.0);
            switch (randomAnim){
                case 0:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
                case 1:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    break;
                case 2:
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
                default:
                    tigrBlitAlpha(screen, image3, ventanaAncho - 75, ventanaAlto - 20, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image, ventanaAncho, ventanaAlto, 0, 0, ancho, alto, 100.0);
                    tigrBlitAlpha(screen, image2, ventanaAncho + 75, ventanaAlto + 20, 0, 0, ancho, alto, 100.0);
                    break;
            }

            tigrFillRect(screen, -1, -1, screen->w+20, screen->h+20, tigrRGBA(0, 0, 0, 150));
            drawStatic(screen);
            if (count <= 20){
                tigrFillCircle(screen, (screen->w)-tigrTextWidth(tfont, "REC")-7, 5, 5, tigrRGB(255, 0, 0));
                tigrPrint(screen, tfont, ((screen->w)-tigrTextWidth(tfont, "REC")), 1, lightGrey, "REC");
            }else{
                if (count>40) {
                    count =0;
                }
            }
            
            tigrPrint(screen, tfont, 10, (screen->h)-10, lightGrey, exittext);
            tigrFillRect(screen, 2, 2, (screen->w)-4-(tigrTextWidth(tfont, "REC")+12), 5, darkGrey);  // Linea horizontal superior
            tigrFillRect(screen, (tigrTextWidth(tfont, exittext))+11, (screen->h)-7, (screen->w)-4-(tigrTextWidth(tfont, exittext)+11), 5, darkGrey);  // Línea horizontal inferior
            tigrFillRect(screen, 2, 5, 5, ((screen->h)-7), darkGrey);
            tigrFillRect(screen, (screen->w)-7, 2+(tigrTextHeight(tfont, "REC")), 5, ((screen->h)-7)-9, darkGrey);
            tigrUpdate(screen);
            
            
        }
    }

    tigrFree(image);
    tigrFree(image2);
    tigrFree(image3);
    tigrFree(fondo);
    tigrFree(screen);
}

int main () {
    tigrCamera();
    return 0;
}