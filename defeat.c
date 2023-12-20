#include "tigr.h"
#include "tigr.c"
#include <unistd.h>

#define black tigrRGBA(0, 0, 0, 0xff)
#define white tigrRGBA(0xff, 0xff, 0xff, 0xff)
#define lightGrey tigrRGBA(0xcc, 0xcc, 0xcc, 0xff)      // color normal del texto
#define darkGrey tigrRGBA(0x44, 0x44, 0x44, 0xff)       // color de los bordes de la cámara


void drawDefeat();

int main(){
    int pid= fork();
    if(pid==0){ 
    		system("mpg123 -q sound/Dead.mp3");
                sleep(1);
                exit(0);
    }
    drawDefeat();
    return 0;
}

void drawDefeat(){
    Tigr *screen = tigrWindow(320, 240, "YOU DIED!", TIGR_FULLSCREEN);
    Tigr *overlay = tigrLoadImage("assets/screen_defeat.png");
    int count=0, close = 0;
    int eyeWidth=20, sep=80, xPos=(screen->w/2)-eyeWidth-(sep/4);
    int y= 50, eyeCount=0;

    while (!tigrClosed(screen) && close == 0){
        if (count<212){
            count++;
        }
        tigrClear(screen, tigrRGB(0x00, 0x00, 0x00));

        if(count==212){
            if (eyeCount<=255){
                eyeCount++;
            }
            if (eyeCount<=255){
                tigrFillCircle(screen, xPos, y+eyeWidth, eyeWidth, tigrRGBA(0xcc, 0xcc, 0x66, eyeCount));          //upper eye 1
                tigrFillCircle(screen, xPos+sep, y+eyeWidth, eyeWidth, tigrRGBA(0xcc, 0xcc, 0x66, eyeCount));      // upper eye 2

                tigrFillCircle(screen, xPos, y+(int)(1.5*eyeWidth), eyeWidth, black);       // lower eye 1
                tigrFillCircle(screen, xPos+sep, y+(int)(1.5*eyeWidth), eyeWidth, black);   // lower eye 2
            } else {
                close = 1;
            }
        }
        
        
        tigrPrint(screen, tfont, 10, (screen->h)-10, lightGrey, "Press ESC to close cameras");
        
        tigrFillCircle(screen, (screen->w)-tigrTextWidth(tfont, "REC")-7, 5, 5, lightGrey);
        tigrPrint(screen, tfont, ((screen->w)-tigrTextWidth(tfont, "REC")), 1, lightGrey, "REC");

        tigrFillRect(screen, 2, 2, (screen->w)-4-(tigrTextWidth(tfont, "REC")+12), 5, darkGrey);  // Linea horizontal superior
        tigrFillRect(screen, (tigrTextWidth(tfont, "Press ESC to close cameras"))+11, (screen->h)-7, (screen->w)-4-(tigrTextWidth(tfont, "Press ESC to close cameras")+11), 5, darkGrey);  // Línea horizontal inferior
        tigrFillRect(screen, 2, 5, 5, ((screen->h)-7), darkGrey);
        tigrFillRect(screen, (screen->w)-7, 2+(tigrTextHeight(tfont, "REC")), 5, ((screen->h)-7)-9, darkGrey);
        
        tigrPrint(screen, tfont, (screen->w/2)-27, (screen->h)-count-12, white, "YOU DIED!");

        tigrBlitAlpha(screen, overlay, 0, 0, 0, 0, overlay->w, overlay->h, 255);
        
        tigrUpdate(screen);
    }
    tigrFree(overlay);
    tigrFree(screen);
}
