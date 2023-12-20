#include <stdio.h>
#include <stdlib.h>


int main(){
    system("gcc menu_tigr.c -lGLU -lGL -lX11 -o menu");
    printf("menu_tigr.c compiled successfully\n");

    system("gcc fnaf.c -o fnaf");
    printf("fnaf.c compiled successfully\n");

    system("gcc final_screen.c -lGLU -lGL -lX11 -o final_screen");
    printf("final_screen.c compiled successfully\n");

    system("gcc camera.c -lGLU -lGL -lX11 -o camera");
    printf("camera.c compiled successfully\n");

    system("gcc security_cameras.c -lGLU -lGL -lX11 -o cameras");
    printf("security_cameras.c compiled successfully\n");

    system("gcc defeat.c -lGLU -lGL -lX11 -o defeat");
    printf("defeat.c compiled successfully\n");

    system("gcc nightmodules.c -lGLU -lGL -lX11 -o nightmodules");
    printf("nightmodules.c compiled successfully\n");

    system("gcc nightmodules2.c -lGLU -lGL -lX11 -o nightmodules2");
    printf("nightmodules2.c compiled successfully\n");

    system("gcc night.c -lGLU -lGL -lX11 -o night");
    printf("night.c compiled successfully\n");

    // system("gnome-terminal --working-directory=/Desktop");
    
    system("chmod +x start.sh");
    system("./start.sh");



    return 0;
}
