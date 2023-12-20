
#include <stdio.h>
#include <stdlib.h>

int main() {
    char nombre_imagen[] = "howtoplay.jpg";
    char respuesta;

    printf("¿Deseas abrir la imagen %s? (s/n): ", nombre_imagen);
    scanf(" %c", &respuesta);

    if (respuesta == 's' || respuesta == 'S') {
        printf("Abriendo la imagen...\n");
        system("xdg-open imagen.jpeg");
    } else {
        printf("No se abrirá la imagen.\n");
    }

    return 0;
}


/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#endif

void open_image(char* nombre_imagen) {
    #ifdef _WIN32
        ShellExecute(NULL, "open", nombre_imagen, NULL, NULL, SW_SHOWNORMAL);
    #else
        // Cambia 'gnome-open' a 'kde-open' si estás utilizando KDE.
        system("gnome-open ", *nombre_imagen);
    #endif
}

int main() {
    char nombre_imagen[] = "howtoplay.jpg";
    char respuesta;

    printf("¿Deseas abrir la imagen %s? (s/n): ", nombre_imagen);
    scanf(" %c", &respuesta);

    if (respuesta == 's' || respuesta == 'S') {
        printf("Abriendo la imagen...\n");
        open_image(nombre_imagen);
    } else {
        printf("No se abrirá la imagen.\n");
    }

    return 0;
}




#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *image;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    image = gtk_image_new_from_file("imagen.jpg");

    gtk_container_add(GTK_CONTAINER(window), image);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

*/
