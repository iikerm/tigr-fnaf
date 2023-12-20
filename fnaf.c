#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char text_night[5];
    int current_night;
} TNight;

// DEFINICIÓN DE MÓDULOS
void howtoplay(), call(), call2(), call3(), call4(), newLine(), clearConsole(), dibujar_marco(), dibujar_parte_vacia(), dibujar_marco_camaras(), dibujar_interior_camaras(), dibujar_puertas_a(), dibujar_puertas_b(), dibujar_puertas_c(), dibujar_puertas(), dibujar_camaras(), map(), energia_tiempo(), warnings(), actionMenu(), menu_camaras_seguridad(), menu_luces(), menu_puertas();
int randomNumber(), readFile(const char *fname), randomNumber01(), drawConditionDoors();


// MÓDULO PRINCIPAL
int main() {

    // Variables que manejan las diferentes condiciones del juego y su valor al inicializarse
    int drawcondition = 0, difficulty, energy = 100, time = 150, condition, score = 0, punctuation = 0, dead = 0,
        light1 = 0, light2 = 0, door1 = 0, door2 = 0, animatronic = 0, kill = 5, randfermin, animatronic_position = 2, animatronic_left = 3;
    float power = 100.00;
    char selection = 'y';
    TNight t;
    strcpy(t.text_night, "Night");
    t.current_night = 1;

    // Se llama al módulo readFile para ver el contenido de difficulty.txt y asignar el valor de la dificultad
    difficulty = readFile("difficulty.txt");
    difficulty = difficulty + 1;    // Puesto que el valor guardado en el .txt está entre 0 y 2 le sumamos 1

    // Comprobamos que la dificultad sea válida
    if (difficulty != 1 && difficulty != 2 && difficulty != 3) {
        difficulty = 0; // En caso contrario, le asignamos el valor 0
    }

    if (difficulty == 1 || difficulty == 2 || difficulty == 3) {    // El código solo se ejecutará si la dificultad es válida, por ello el valor
                                                                    // 0 hará que el código pare de ejecutarse

        while (t.current_night <= 5 && selection == 'y') {  // La partida continúa siempre que no se haya superado la noche 5 y el jugador haya
                                                            // decidido seguir jugando
            selection = 'n';
            
            // Principio de ronda

            // El tiempo que se debe sobrevivir se asigna en función de la dificultad
            if (difficulty == 2 || difficulty == 3) {
                time = 120;
            } else {
                if (difficulty == 1) {
                    time = 150;
                }
            }

            call(t);    // Se muestra la pantalla de 12AM
            // Se resetean las variables de control
            condition = 0;
            punctuation = 0;
            light1 = 0;
            light2 = 0;
            door1 = 0;
            door2 = 0;
            animatronic = 0;
            kill = 5;
            dead = 0;
            animatronic_position = 2;
            animatronic_left = 3;
            power = 100.00;
            energy = 100;
            
            int pid = fork();
                while(pid == 0) {
                    system("mpg123 -q sound/MainAmbience.mp3"); // Música de fondo
                }
            // Ronda
            while (time != 0 && condition != 5 && dead == 0) {  // La ronda dura hasta que el tiempo se agota o el jugador abandona/muere
                
                // Se comprueba si queda energía en el sistema de seguridad y, en caso contrario, se apaga todo
                if (energy == 0) {
                    light1 = 0;
                    light2 = 0;
                    door1 = 0;
                    door2 = 0;
                }

                clearConsole();
                newLine();
                if (animatronic == 0) { // En caso de que no haya ningún animatrónico en movimiento
                    animatronic = randomNumber(difficulty); // Se genera un número aleatorio para decidir si un animatrónico se mueve o no
                    if (animatronic == 1) { // Si un animatrónico se mueve entonces se decide cuál es (para no dibujarlo en la cámara) 
                                            // y su posición (para saber en qué puerta está) mediante números aleatorios 
                        int random_number;
                        random_number = randomNumber01();
                        animatronic_position = random_number;
                        random_number = randomNumber01();
                        animatronic_left = random_number;
                    }
                }

                warnings(kill, energy); // Se envían las variables kill y energy al módulo warning
                newLine();

                // Se traducen todas las variables de luces, puertas y animatrónicos a una única variable que controla la condición de dibujo del mapa
                drawcondition = drawConditionDoors(light1, light2, door1, door2, animatronic, animatronic_position);

                map(drawcondition, animatronic_position, energy);   // Se dibuja el mapa
                newLine();
                energia_tiempo(energy, time);   // Se dibuja la barra de energía y se escribe el tiempo restante
                newLine();
                newLine();
                actionMenu(energy, time, &condition);   // Se envían las variables energy, time y condition al módulo actionMenu, el cual le ofrece
                                                        // al jugador las acciones disponibles y recoge su elección
		if (energy <= 0){
                    dead = 1;
                }
                // Se abre el menú correspondiente en función de lo que haya elegido hacer el jugador
                if (condition == 1) {
                    menu_camaras_seguridad(animatronic, animatronic_left);
                }
                if (condition == 2) {
                    menu_luces(&light1, &light2);
                }
                if (condition == 3) {
                    menu_puertas(&door1, &door2);
                }
                if (condition == 5) {
                    dead = 1;
                    system("pkill -TSTP mpg123");
                }

                // Al acabar la ronda la energía se gasta de forma proporcional a lo que haya activado y a la dificultad
                for (int i = 0; i <= difficulty; i++) { // Realizamos el proceso de descuento de energía las veces que indique la dificultad (1-3)
                    power = power - 0.1;   // Siempre se gasta un poco de energía en cada turno
                    // Se gasta un poco más de energía por cada elemento activado
                    if (light1 == 1) {
                        power = power - 0.15;
                    }
                    if (light2 == 1) {
                        power = power - 0.15;
                    }
                    if (door1 == 1) {
                        power = power - 0.15;
                    }
                    if (door2 == 1) {
                        power = power - 0.15;
                    }
                }

                // Si hay un animatrónico en movimiento se le resta 1 al contador de muerte
                if (animatronic == 1) {
                    kill = kill - 1;
                }

                // Si el contador de muerte llega a 0, entonces
                if (kill == 0) {
                    clearConsole();
                    // Si el jugador ha cerrado la puerta correcta se reproduce un sonido y las variables de animatronic se resetean
                    if (animatronic_position == 0 && door1 == 1) {
                        char selection;
                        animatronic = 0;
                        kill = 5;
                        animatronic_position = 2;
                        animatronic_left = 3;
                        int pid = fork();
		            if(pid==0){
		                randfermin = rand() % 9;
		                if(randfermin == 1){
		            	    system("mpg123 -q sound/fermin1.mp3 2>/dev/null");
		            	} else { 
		            	    system("mpg123 -q sound/garble1.mp3 2>/dev/null");
		            	}
		            	exit(0);
		            }
                    }
                    if (animatronic_position == 1 && door2 == 1) {
                        char selection;
                        animatronic = 0;
                        kill = 5;
                        animatronic_position = 2;
                        animatronic_left = 3;
                        int pid = fork();//proceso hijo que reproduce sonido se va el animatronico
		        if(pid==0){
		            randfermin = rand() % 9;
		            if(randfermin == 1){
		            	system("mpg123 -q sound/fermin1.mp3 2>/dev/null");
		            } else { 
		            	system("mpg123 -q sound/garble1.mp3 2>/dev/null");
		            }
		            exit(0);
		        }
                    }
                    // Si el jugador no ha cerrado la puerta corriente se activan las variables de muerte y fin de partida
                    if (animatronic_position == 0 && door1 == 0) {
                        t.current_night = 6;
                        dead = 1;
                    }
                    if (animatronic_position == 1 && door2 == 0) {
                        t.current_night = 6;
                        dead = 1;
                    }
                }

                // Al final de cada ronda se actualiza energía, tiempo y puntuación de la ronda
                energy = (int)power;
                time = time - 1;
                punctuation = punctuation + 1;
            }
            // Al acabar la ronda se actualiza la puntuación global de la partida con la obtenida en la ronda
            score = score + punctuation;
            clearConsole();
            // Si el jugador no ha muerto ni decidido salir del juego y las noches no han terminado
            if (dead == 0 && t.current_night <=5 && condition != 5) {
                system("pkill -TSTP mpg123");   // Se para la música
                call2(t);   // Pantalla de 6AM
                printf("You won %d points in this round. Total score is %d\n", punctuation, score); // Puntos que se ha obtenido en esa ronda
                if (t.current_night != 5) { // Si las noches no han terminado se le pregunta al jugador si quiere seguir
                    do{
		        printf("Continue? (y/n): ");
		        scanf(" %c", &selection);
		    }while (selection != 'y' && selection != 'n');
                }
                if (selection == 'y') { // Si el jugador decide continuar se pasa a la siguiente noche
                    t.current_night = t.current_night + 1;
                } else {    // Si no, se le asigna a la noche un valor que se sale de los aceptados (1-5) y que, por tanto, rompe el bucle while de la partida
                    t.current_night = 6;
                }
            // En caso contrario
            } else {
            	if(condition != 5){
		        call4(score);   // Se muestra la pantalla de muerte
		        printf("You won %d points in this round. Total score is %d\n", punctuation, score);
		}
            }
        }
        if(condition!=5){call3(score);}   // Se muestra la pantalla final con las estrellas

    // Si ha habido un problema al principio del código se le habrá asignado valor 0 a la dificultad, cosa que
    // hará que se muestre un mensaje de error
    } else {
        printf("Critical error: The difficulty is not valid!\n");
    }
    remove("difficulty.txt");
    system("pkill -TSTP mpg123");
    return 0;
}


// MÓDULOS SECUNDARIOS

// Módulo que recoge el valor de la noche actual y llama a la pantalla de 12AM
void call (TNight t) {
    FILE *animFile = fopen("night.txt", "w");

    putc(t.current_night, animFile);
    fclose(animFile);

    system("chmod +x nightscreen.sh");
    system("./nightscreen.sh");
}

// Módulo que recoge el valor de la noche actual y llama a la pantalla de 6AM
void call2 (TNight t) {
    FILE *animFile = fopen("night.txt", "w");

    putc(t.current_night, animFile);
    fclose(animFile);

    system("chmod +x day.sh");
    system("./day.sh");
}

// Módulo que recoge el valor de la puntuación y llama a la pantalla final
void call3 (int score) {
    FILE *animFile = fopen("finish.txt", "w");

    putc(score, animFile);
    fclose(animFile);

    system("chmod +x finish.sh");
    system("./finish.sh");
}

// Módulo que recoge el valor de la puntuación y llama a la pantalla de muerte
void call4 (int score) {
    system("chmod +x defeat.sh");
    system("./defeat.sh");
}

// Módulo que traduce las condiciones de luces, puertas y animatrónicos a una única variable que controla el dibujo del mapa
int drawConditionDoors (int light1, int light2, int door1, int door2, int animatronic, int animatronic_position) {
    int drawcondition;
    // Normal
    if (light1 == 0 && light2 == 0 && door1 == 0 && door2 == 0) {   
        drawcondition = 1;
    // Puerta 2 cerrada
    } else if (light1 == 0 && light2 == 0 && door1 == 0 && door2 == 1) {    
        drawcondition = 2;
    // Puerta 1 cerrada
    } else if (light1 == 0 && light2 == 0 && door1 == 1 && door2 == 0) {
        drawcondition = 3;
    // Puertas cerradas
    } else if (light1 == 0 && light2 == 0 && door1 == 1 && door2 == 1) {
        drawcondition = 4;
    // Luz 2 encendida
    } else if (light1 == 0 && light2 == 1 && door1 == 0 && door2 == 0) {
        drawcondition = 5;
    // Luz 2 encendida y puerta 2 cerrada
    } else if (light1 == 0 && light2 == 1 && door1 == 0 && door2 == 1) {
        drawcondition = 6;
    // Luz 2 encendida y puerta 1 cerrada
    } else if (light1 == 0 && light2 == 1 && door1 == 1 && door2 == 0) {
        drawcondition = 7;
    // Luz 2 encendida y puertas cerradas
    } else if (light1 == 0 && light2 == 1 && door1 == 1 && door2 == 1) {
        drawcondition = 8;
    // Luz 1 encendida
    } else if (light1 == 1 && light2 == 0 && door1 == 0 && door2 == 0) {
        drawcondition = 9;
    // Luz 1 encendida y puerta 2 cerrada
    } else if (light1 == 1 && light2 == 0 && door1 == 0 && door2 == 1) {
        drawcondition = 10;
    // Luz 1 encendida y puerta 1 cerrada
    } else if (light1 == 1 && light2 == 0 && door1 == 1 && door2 == 0) {
        drawcondition = 11;
    // Luz 1 encendida y puertas cerradas
    } else if (light1 == 1 && light2 == 0 && door1 == 1 && door2 == 1) {
        drawcondition = 12;
    // Luces encendidas
    } else if (light1 == 1 && light2 == 1 && door1 == 0 && door2 == 0) {
        drawcondition = 13;
    // Luces encendidas y puerta 2 cerrada
    } else if (light1 == 1 && light2 == 1 && door1 == 0 && door2 == 1) {
        drawcondition = 14;
    // Luces encendidas y puerta 1 cerrada
    } else if (light1 == 1 && light2 == 1 && door1 == 1 && door2 == 0) {
        drawcondition = 15;
    // Luces encendidas y puertas cerradas
    } else if (light1 == 1 && light2 == 1 && door1 == 1 && door2 == 1) {    
        drawcondition = 16;
    }

    // Se dibuja luz en la puerta 1 en los casos 9, 10, 13, 14
    // Se dibuja luz en la puerta 2 en los casos 5, 7, 13, 15
    // Se dibuja puerta 1 cerrada en los casos 3, 4, 7, 8, 11, 12, 15, 16
    // Se dibuja puerta 2 cerrada en los casos 2, 4, 6, 8, 10, 12, 14, 16

    return drawcondition;
}

// Módulo que hace saltos de línea
void newLine () {
    printf("\n");
}

// Módulo que limpia la consola
void clearConsole () {
    system("clear");
}

// Módulo que genera un número aleatorio entre 0 y 1
int randomNumber01() {
    int random;
    srand(time(NULL));
    random = rand() % 2 + 0;
    return random;
}

// Módulo que genera el número aleatorio del animatronic en función de la dificultad (más difícil = menos margen aleatorio)
int randomNumber(int difficulty) {
    int random;
    srand(time(NULL));
    if (difficulty == 1) {
        random = rand() % 5 + 0;
        if (random == 1) {
            random = 1;
        } else {
            random = 0;
        }
    } else if (difficulty == 2) {
        random = rand() % 3 + 0;
        if (random == 1) {
            random = 1;
        } else {
            random = 0;
        }
    } else {
        random = rand() % 2 + 0;
        if (random == 1) {
            random = 1;
        } else {
            random = 0;
        }
    }
    return random;
}

// Módulo que abre la pantalla de Tigr de las cámaras de seguridad
void menu_camaras_seguridad (int animatronic, int animatronic_left) {
    char selection;
    FILE *animFile = fopen("anim.txt", "w");

    putc(animatronic_left, animFile);
    fclose(animFile);

    system("chmod +x cam.sh");
    system("./cam.sh");
    
    printf("Press any key to continue: ");
    scanf(" %c", &selection);
}

// Menú que muestra el estado de las luces, pregunta al jugador con cuál quiere interactuar y actualiza las variables correspondientes
void menu_luces (int *light1, int *light2) {
    int interaction, l1 = *light1, l2 = *light2;
    if (l1 == 0 && l2 == 0) {
        printf("At this moment, both lights are OFF.\n");
    } else if (l1 == 1 && l2 == 0) {
        printf("At this moment, light 1 is ON and light 2 is OFF.\n");
    } else if (l2 == 1 && l1 == 0) {
        printf("At this moment, light 1 if OFF and light 2 is ON.\n");
    } else {
        printf("At this moment, both lights are ON.\n");
    }
    printf("What light do you want to interact with? (1/2): ");
    scanf("%d", &interaction);
    while (interaction != 1 && interaction != 2) {
        printf("What light do you want to interact with? (1/2): ");
        scanf("%d", &interaction);
    }
    if (interaction == 1) {
        if (*light1 == 0) {
            *light1 = 1;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/switch.mp3");
                sleep(1);
                exit(0);
            }
        } else {
            *light1 = 0;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/switch.mp3");
                sleep(1);
                exit(0);
            }
        }
    } else {
        if (*light2 == 0) {
            *light2 = 1;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/switch.mp3");
                sleep(1);
                exit(0);
            }
        } else {
            *light2 = 0;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/switch.mp3");
                sleep(1);
                exit(0);
            }
        }
    }
}

// Menú que muestra el estado de las puertas, pregunta al jugador con cuál quiere interactuar y actualiza las variables correspondientes
void menu_puertas (int *door1, int *door2) {
    int interaction, l1 = *door1, l2 = *door2;
    if (l1 == 0 && l2 == 0) {
        printf("At this moment, both doors are OPENED.\n");
    } else if (l1 == 1 && l2 == 0) {
        printf("At this moment, door 1 is CLOSED and door 2 is OPENED.\n");
    } else if (l2 == 1 && l1 == 0) {
        printf("At this moment, door 1 is OPENED and door 2 is CLOSED.\n");
    } else {
        printf("At this moment, both doors are CLOSED.\n");
    }
    printf("What door do you want to interact with? (1/2): ");
    scanf("%d", &interaction);
    while (interaction != 1 && interaction != 2) {
        printf("What door do you want to interact with? (1/2): ");
        scanf("%d", &interaction);
    }
    if (interaction == 1) {
        if (*door1 == 0) {
            *door1 = 1;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/Door.mp3");
                sleep(1);
                exit(0);
            }
        } else {
            *door1 = 0;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/Door.mp3");
                sleep(1);
                exit(0);
            }
        }
    } else {
        if (*door2 == 0) {
            *door2 = 1;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/Door.mp3");
                sleep(1);
                exit(0);
            }
        } else {
            *door2 = 0;
            int pid = fork();
            if(pid==0){ 
            	system("mpg123 -q sound/Door.mp3");
                sleep(1);
                exit(0);
            }
        }
    }
}

// Módulo que gestiona el menú de acciones y recoge la elección del jugador
void actionMenu (int energy, int time, int *condition) {
    char selection;
    int action;
    // Se comprueba si hay energía
    if (energy > 0) {
        printf("Available actions\n");
        printf("1. Check cameras\n");
        printf("2. Turn on/off lights\n");
        printf("3. Open/close doors\n");
        printf("4. Do nothing\n");
        printf("5. Return to main menu\n");
        do{
            printf("Select an action: ");
            scanf("%d", &action);
            if(action >= 1 && action <= 5){
            }else{ 
                while (getchar() != '\n');  // Descartar caracteres restantes en la línea
            }
        }while (action < 1 && action > 5);
        *condition = action;
    // Si no hay energía el jugador únicamente puede elegir si sigue o no jugando
    } else {
        printf("You can not use the security system!\n");
        printf("You must survive %d more time to win\n", time);
    }
}

// Módulo que dibuja la barra de energía y el tiempo
void energia_tiempo (int energy, int time) {
    if (energy > 0) {
        printf("Remaining energy: %d%%  ", energy);
        for (int i = 1; i <= energy; i++) {
            printf("▆");
        }
    } else {
        printf("OUT OF ENERGY!");
        system("pkill -TSTP mpg123");
        int pid = fork();
        if(pid==0){ 
            system("mpg123 -q sound/powerdown.mp3 2>/dev/null");
            exit(0);
        }
        sleep(5);
        int pid2 = fork();
        if(pid2==0){ 
            system("mpg123 -q sound/musicbox.mp3 2>/dev/null");
            exit(0);
        }
        sleep(5);
    }
    newLine();
    printf("Remaining time: %d", time);
}

// Módulo que muestra avisos
void warnings (int kill, int energy) {
    int randrecio;
    if (kill == 4 && energy > 0) {  // Si el contador de kill está iniciado, se muestra un aviso al alcanzar éste el valor 4
        for (int i = 0; i <= 60; i++) {
            printf(" ");
        }
        int pid = fork();
	if(pid==0){
	    randrecio = rand() % 5;
	    if(randrecio == 1){
	        system("mpg123 -q sound/recio.mp3 2>/dev/null");
	        exit(0);
	    } else { 
		system("mpg123 -q sound/FreddyLaugh3.mp3 2>/dev/null | tr -d ' \t\n\r\f'");
		exit(0);
	    }

	}
        printf("Have you heard something?");
    }
}

// Módulo que se ocupa de dibujar el mapa empleando otros módulos secundarios, todos ellos definidos más abajo, y basándose en la condición establecida
void map (int drawcondition, int animatronic_position, int energy) {
    int condition = drawcondition, position = animatronic_position, energia = energy;
    dibujar_marco();
    for(int i = 0; i <= 9; i++) {
        dibujar_parte_vacia();
    }
    dibujar_camaras(energia);
    for(int i = 0; i <= 2; i++) {
        dibujar_parte_vacia();
    }
    dibujar_puertas(condition, position);
    dibujar_marco();
}

// Módulos de dibujo de mapa

// Módulo que dibuja el marco superior e inferior del mapa
void dibujar_marco () {
    for (int i = 0; i <= 150; i++) {
        printf("_");
    }
    newLine();
}

// Módulo que dibuja la zona vacía del mapa
void dibujar_parte_vacia () {
    printf("|");    // Marcos de la pantalla
    for (int i = 0; i <= 148; i++) {
        printf(" ");
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja el marco de las cámaras de seguridad
void dibujar_marco_camaras () {
    printf("|");    // Marcos de la pantalla
    for (int i = 0; i <= 66; i++) {
        printf(" ");
    }
    for (int i = 0; i <= 18; i++) {
        printf("_");
    }
    for (int i = 0; i <= 62; i++) {
        printf(" ");
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja el interior de las cámaras de seguridad
void dibujar_interior_camaras (int a) {
    for (int i = 0; i <= 3; i++) {
        printf("|");    // Marcos de la pantalla
        for (int i = 0; i <= 66; i++) {
            printf(" ");
        }
        printf("|");
        for (int i = 0; i <= 16; i++) {
            if (a <= 0) {
                printf(" ");
            } else {
                printf("+");
            }
        }
        printf("|");
        for (int i = 0; i <= 62; i++) {
            printf(" ");
        }
        printf("|");    // Marco de la pantalla
        newLine();
    }
}

// Módulo que dibuja la parte de arriba de las puertas
void dibujar_puertas_a () {
    // Imprimir las líneas superiores de las puertas
    printf("|");    // Marco de la pantalla
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre el marco y la primera puerta
    }
    for (int i = 0; i <= 12; i++) {
        printf("_");    // Parte de arriba de la primera puerta
    }
    for (int i = 0; i <= 80; i++) {
        printf(" ");    // Espacio entre las puertas
    }
    for (int i = 0; i <= 12; i++) {
        printf("_");    // Parte de arriba de la segunda puerta
    }
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre la segunda puerta y el marco
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja los marcos de las puertas
void dibujar_puertas_b (int a, int b) {
    
    if (((a == 9 || a == 10 || a == 13 || a == 14 || a == 17) && b == 0) || ((a == 5 || a == 7 || a == 13 || a == 15 || a == 18) && b==1)){
    	int pid = fork();
        if(pid==0){ 
            system("mpg123 -q sound/windowscare.mp3 2>/dev/null");
            exit(0);
        }
    }
    // Imprimir la primera parte de las puertas
    printf("|");    // Marco de la pantalla
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre el marco y la primera puerta
    }
    printf("|");    // Marco de la primera puerta
    for (int i = 0; i <= 10; i++) {
        // Interior de la primera puerta
        if (a == 3 || a == 4 || a == 7 || a == 8 || a == 11 || a == 12 || a == 15 || a == 16) {
            printf("▆");
        } else if (a == 9 || a == 10 || a == 13 || a == 14 || a == 17) {
            if (b == 0) {
                if (i < 2 || i > 8) {
                    printf(" ");
                }
                else {
                    printf("-");
                }
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la primera puerta
    for (int i = 0; i <= 80; i++) {
        printf(" ");    // Espacio entre las puertas
    }
    printf("|");    // Marco de la segunda puerta
    for (int i = 0; i <= 10; i++) {
        // Interior de la segunda puerta
        if (a == 2 || a == 4 || a == 6 || a == 8 || a == 10 || a == 12 || a == 14 || a == 16) {
            printf("▆");
        } else if (a == 5 || a == 7 || a == 13 || a == 15 || a == 18) {
            if (b == 1) {
                if (i < 2 || i > 8) {
                    printf(" ");
                }
                else {
                    printf("-");
                }
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la segunda puerta
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre la segunda puerta y el marco
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja los marcos de las puertas
void dibujar_puertas_d (int a, int b) {
    // Imprimir la primera parte de las puertas
    printf("|");    // Marco de la pantalla
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre el marco y la primera puerta
    }
    printf("|");    // Marco de la primera puerta
    for (int i = 0; i <= 10; i++) {
        // Interior de la primera puerta
        if (a == 3 || a == 4 || a == 7 || a == 8 || a == 11 || a == 12 || a == 15 || a == 16) {
            printf("▆");
        } else if (a == 9 || a == 10 || a == 13 || a == 14 || a == 17) {
            if (b == 0) {
                printf(" ");
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la primera puerta
    for (int i = 0; i <= 80; i++) {
        printf(" ");    // Espacio entre las puertas
    }
    printf("|");    // Marco de la segunda puerta
    for (int i = 0; i <= 10; i++) {
        // Interior de la segunda puerta
        if (a == 2 || a == 4 || a == 6 || a == 8 || a == 10 || a == 12 || a == 14 || a == 16) {
            printf("▆");
        } else if (a == 5 || a == 7 || a == 13 || a == 15 || a == 18) {
            if (b == 1) {
                printf(" ");
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la segunda puerta
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre la segunda puerta y el marco
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja el botón de las puertas
void dibujar_puertas_c (int a, int b) {
    // Imprimir la parte del botón de las puertas
    printf("|");    // Marco de la pantalla
    for (int i = 0; i <= 18; i++) {
        printf(" ");    // Espacio entre el marco y el botón de la puerta
    }
    printf("❑");   // Botón de la puerta
    printf(" ");   // Espacio entre el botón y el marco de la puerta
    printf("|");   // Marco de la puerta
    for (int i = 0; i <= 10; i++) {
        if (a == 3 || a == 4 || a == 7 || a == 8 || a == 11 || a == 12 || a == 15 || a == 16) {
            printf("▆");
        } else if (a == 9 || a == 10 || a == 13 || a == 14 || a == 17) {
            if (b == 0) {
                if (i < 2 || i > 8) {
                    printf(" ");
                }
                else {
                    printf("-");
                }
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la puerta
    for (int i = 0; i <= 78; i++) {
        printf(" ");    // Espacio entre las puertas
    }
    printf("❑");   // Botón de la puerta
    printf(" ");   // Espacio entre el botón y el marco de la puerta
    printf("|");   // Marco de la puerta
    for (int i = 0; i <= 10; i++) {
        if (a == 2 || a == 4 || a == 6 || a == 8 || a == 10 || a == 12 || a == 14 || a == 16) {
            printf("▆");
        } else if (a == 5 || a == 7 || a == 13 || a == 15 || a == 18) {
            if (b == 1) {
                if (i < 2 || i > 8) {
                    printf(" ");
                }
                else {
                    printf("-");
                }
            } else {
                printf("+");
            }
        } else {
            printf(" ");
        }
    }
    printf("|");    // Marco de la segunda puerta
    for (int i = 0; i <= 20; i++) {
        printf(" ");    // Espacio entre la segunda puerta y el marco
    }
    printf("|");    // Marco de la pantalla
    newLine();
}

// Módulo que dibuja la pantalla de las cámaras de seguridad a partir de reunir las acciones de módulos anteriores
void dibujar_camaras (int energia) {
    int a = energia;
    dibujar_marco_camaras();
    dibujar_interior_camaras(a);
    dibujar_marco_camaras();
}

// Módulo que dibuja las puertas a partir de llamar en orden a los módulos anteriores
void dibujar_puertas (int condition, int position) {
    int a = condition, b = position;
    dibujar_puertas_a();
    for(int i = 0; i <= 2; i++) {
        dibujar_puertas_d(a, b);
    }
    for(int i = 0; i <= 2; i++) {
        dibujar_puertas_b(a, b);
    }
    dibujar_puertas_c(a, b);
    for(int i = 0; i <= 4; i++) {
        dibujar_puertas_b(a, b);
    }
}

// Módulo que lee el archivo de dificultad y almacena ese valor en la variable correspondiente
int readFile(const char *fname){
    FILE *dFile = fopen(fname, "r");
    int difficulty=0;

    rewind(dFile);
    difficulty = getc(dFile);
    fclose(dFile);

    return difficulty;
}



