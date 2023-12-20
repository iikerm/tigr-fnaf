// Text below must be used to compile this code:
// gcc menu_tigr.c -lGLU -lGL -lX11 -o menu_tigr

#include "tigr.h"
#include "tigr.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


// Main menu options:
#define menuText "What do you want to do?:\n"
#define option1 "-> Start game\n"
#define option2 "-> How to play\n"
#define option3 "-> Authors\n"
#define option4 "Press ESC to exit...\n"

// tigr RGBA colors:
#define lightGrey tigrRGBA(0xcc, 0xcc, 0xcc, 0xff)  // Normal text color
#define lightYellow tigrRGBA(242, 231, 126, 0xff)   // Selected text color
#define white tigrRGBA(0xff, 0xff, 0xff, 0xff)      // Mr incredible's sprite border color
#define black tigrRGB(0x00, 0x00, 0x00)             // Background color for the tigr window

#define clearWhite tigrRGBA(0xdd, 0xdd, 0xdd, 0xdd) // color from drawStatic()
#define clearGrey tigrRGBA(0x77, 0x77, 0x77, 0xdd)  // color from drawStatic()


//  HEADER PART

/*Starts the whole tigr part of this program. This was made compactly so that only one 
function needs to be called in order to start it. 
This module generates the tigr window that the program will run on and writes all the 
data necessary onto the file so the rest of the progams in the game can read it */
void startMenu();

/*This function acts as a shorthand for the program to execute the 'clear' command
on the linux terminal*/
void clearTerminal();

/*This function is in charge of drawing the 'nieve' present on almost all of the menu
in order to give it a glitched-like appearence*/
void drawStatic(Tigr *screen);

/*This function swaps randomly alfredo's normal sprite for the one where he is saying hello*/
void animate(Tigr *screen, Tigr *image, Tigr *animation, int forceAnimation);

/*This function draws most of the tigr elements in the main menu part*/
void reDraw(Tigr *sc, int selected);

/*This function takes as an argument the tigr window and clears everything from the main 
menu so that it can draw on it the difficulty selector part*/
int selectDifficulty(Tigr *diffScreen);

/*This function takes as an argument the tigr window and clears everything from the main 
menu so that it can draw on it the how to play part*/
int showHTP(Tigr *htpScreen);

/*This function is a shorthand to draw a triangle needed for the speech bubble in the 
how to play part*/
void drawTriangle(Tigr *screen, int x, int y, int h, TPixel color, TPixel bg_color);

/*This function takes as an argument the tigr window created in startMenu() and draws
everything needed for the main menu. It returns the difficulty chosen by the user in 
selectDifficulty() so that it can be written to the difficulty.txt file*/
int mainMenu(Tigr *screen);

/*This function takes as an argument the tigr window and clears everything from the main 
menu so that it can draw on it the authors part. It returns whether or not the program 
should be closed completely, or only go back to the main menu.*/
int showAuthors(Tigr *screen);

/*This function takes as an argument the tigr window, and the two images needed to make
the animation in the background of the authors part*/
void animateAuthorsBg(Tigr *screen, Tigr *img1, Tigr *img2, int count);


// START OF PROGRAM

int main(){
    startMenu();    // all the menu part starts with this function
    return 0;
}


void startMenu(){
    int difficulty=0, count=0;
    FILE *dFile = fopen("difficulty.txt", "w");

    Tigr *screen = tigrWindow(320, 240, "Five Night's at Alfredo's", TIGR_FULLSCREEN);    // creates the window that will display the menu
    difficulty = mainMenu(screen);       // starts the menu module

    if (difficulty==14){
        difficulty = 13;
    }
    
    putc(difficulty, dFile);
    fclose(dFile);
    printf("%d", difficulty);
}


void clearTerminal(){
    system("clear");
}

void drawStatic(Tigr *screen){
    int i=0, width=screen->w, height=screen->h, rX=0, rY=0, rDim=0;
    // printf("drawStatic\n");
    
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

void animate(Tigr *screen, Tigr *image, Tigr *animation, int forceAnimation){
    int randomNumber = 0;               // variable to store a random number between 0-4. It will indicate if the sprite says hello (0), or not(anything else)
    int coords[2] = {(screen->w)-(image->w), (screen->h)-(image->h)};

    randomNumber = ((float)rand() / (float)RAND_MAX) * (int)5;   
    // creates a random decimal and multiplies it by an integer 5. This will make a random number that is between 4 and 0 both inclusive

    if (randomNumber == 0 || forceAnimation == 1){
            tigrBlit(screen, animation, coords[0], coords[1], 0, 0, animation->w, animation->h);
            // if randomNumber is 0, it displays the animation
        }else{
            tigrBlit(screen, image, coords[0], coords[1], 0, 0, image->w, image->h);
            // if randomNumber is anything different from 0, it displays the normal image
        }
        
}

void reDraw(Tigr *screen, int selected){
    // int coords[2] = {(screen->w)-(image->w), (screen->h)-(image->h)}; 
    // coords[0] --> X coordinate, coords[1] --> Y coordinate
    int i=0;

    // tigrBlit(screen, image, coords[0], coords[1], 0, 0, image->w, image->h);
    // drawStatic(screen);

    tigrPrint(screen, tfont, 10, 0, white, "FIVE NIGHTS AT ALFREDO'S");
    tigrLine(screen, 10, tigrTextHeight(tfont, "F"), 160, tigrTextHeight(tfont, "F"), white);

    tigrPrint(screen, tfont, 10, 22, lightGrey, menuText);

    switch (selected){      // the one with lightYellow is the one selected in each case
        case 0:
            tigrPrint(screen, tfont, 12, 42, lightYellow, option1);
            tigrPrint(screen, tfont, 10, 54, lightGrey, option2);
            tigrPrint(screen, tfont, 10, 66, lightGrey, option3);
            break;
        
        case 1:
            tigrPrint(screen, tfont, 10, 42, lightGrey, option1);
            tigrPrint(screen, tfont, 12, 54, lightYellow, option2);
            tigrPrint(screen, tfont, 10, 66, lightGrey, option3);
            break;
        
        case 2:
            tigrPrint(screen, tfont, 10, 42, lightGrey, option1);
            tigrPrint(screen, tfont, 10, 54, lightGrey, option2);
            tigrPrint(screen, tfont, 12, 66, lightYellow, option3);
            break;
        
        default:
            break;
    }

    tigrPrint(screen, tfont, 10, (screen->h)-10, lightGrey, option4); 
    // Exit option cannot be selected, because you exit with ESC key, so it is outside the switch statement
}

int selectDifficulty(Tigr *diffScreen){
    Tigr *sprite;
    int spriteXpos, spriteYpos;
    int sel=0, count=0;

    while(!tigrClosed(diffScreen) && (sel!=13) && (sel!=14)){ 
        // this screen has a do while instead of a while to prevent the program from detecting the 'enter' key input
        // from the last window, as it only detects it once the first iteration has happened
        count++;

        tigrClear(diffScreen, tigrRGBA(0x00, 0x00, 0x00, 0x00));
        drawStatic(diffScreen);

        spriteXpos = ((diffScreen->w)-(sprite->w +8));      // X axis position of sprite on screen
        spriteYpos = ((diffScreen->h)-(sprite->h +10));     // Y axis position of sprite on screen

        tigrPrint(diffScreen, tfont, 10, 10, lightGrey, "How difficult do you want the game to be?\n");

        tigrPrint(diffScreen, tfont, 10, 30, lightGrey, "Easy\nMedium\nHard");

        if (tigrKeyDown(diffScreen, TK_DOWN)){  // code to switch between options using the down arrow
            sel += 1;
            if (sel > 2){
                sel = 0;
            }
        }

        if (tigrKeyDown(diffScreen, TK_UP)){    // code to switch between options using the up arrow
            sel -= 1;
            if (sel < 0){
                sel = 2;
            }
        }

        if (tigrKeyDown(diffScreen, TK_BACKSPACE)){
            sel = 14;       
            // 14 is an invalid value that will make the program go back to main menu
        }

        if (tigrKeyDown(diffScreen, TK_ESCAPE)){
            sel = 13;
            // 13 is another invalid value that will make the program exit without calling anything else
        }
        tigrPrint(diffScreen, tfont, 10, (diffScreen->h)-22, lightGrey, "Press BACKSPACE to go back");
        tigrPrint(diffScreen, tfont, 10, (diffScreen->h)-10, lightGrey, option4); 

        // the switch loads different sprites (mr incredible images) depending on the difficulty
        switch (sel){
            case 0:
                tigrPrint(diffScreen, tfont, 10, 30, lightYellow, "Easy (For begginers)"); // do things to change colour and load sprite here
                sprite = tigrLoadImage("assets/sprite_easy.png");
                break;
            case 1:
                tigrPrint(diffScreen, tfont, 10, 42, lightYellow, "Medium (You wont even last 10 turns)"); // do things to change colour and load sprite here
                sprite = tigrLoadImage("assets/sprite_mid.png");
                break;
            case 2:
                tigrPrint(diffScreen, tfont, 10, 54, lightYellow, "Hard (Very hard)"); // do things to change colour and load sprite here
                sprite = tigrLoadImage("assets/sprite_hard.png");
                break;       
        }
        if (!sprite){
            tigrError(0, "Cannot load mr incredible, make sure to have downloaded 'assets' folder"); // displays error if it doesnt find sprite image
        }
        
        tigrFillRect(diffScreen, (spriteXpos-3), (spriteYpos-3), (sprite->w +6), (sprite->h +6), white);
        // draws border for sprite

        tigrBlitAlpha(diffScreen, sprite, spriteXpos, spriteYpos, 0, 0, sprite->w, sprite->h, 0x77);
        // displays sprite

        if (tigrKeyDown(diffScreen, TK_RETURN) && count>10){
            break;      
            // This break cannot be removed because if this is implemented in any other way, the selection system won't work,
            // more about this is explained in the difficulty selector part of the memory document
        }

        tigrUpdate(diffScreen);
    }
    tigrFree(sprite);
    return sel;     // returns difficulty as a number from 0-2 (0=easy, 1=mid, 2=hard)
    
}

void drawTriangle(Tigr *screen, int x, int y, int h, TPixel color, TPixel bg_color){

    tigrLine(screen, x, y, x+h+2, y, color);      // horizontal line
    tigrLine(screen, x, y+h, x+h, y, color);    // diagonal 1
    tigrLine(screen, x, y+h+1, x+h+1, y, color);    // diagonal 2
    tigrLine(screen, x, y+1, x, y+h, bg_color);     // vertical line
}

int showHTP(Tigr *htpScreen){   // HTP stands for How To Play
    Tigr *mascot = tigrLoadImage("assets/alfredo.png");
    Tigr *anim = tigrLoadImage("assets/ff_sprite_anim_menu.png");
    if (!mascot){
        tigrError(0, "Cannot load alfredo, make sure to have downloaded 'assets' folder"); // displays error if it doesnt find sprite image
    }
    if (!anim){
        tigrError(0, "Cannot load alfredo's animation, make sure to have downloaded 'assets' folder"); // displays error if it doesnt find sprite image
    }
    
    
    int coords[] = {((htpScreen->w)-(mascot->w)), ((htpScreen->h)-(mascot->h))};    // Stores the coordinates where the mascot or anim images will have to be
    int htpSelected = 0, count = 0;     // variables to control option selected, and nº iterations of the while loop
    int mouseX, mouseY, leftButton;     // variables to control mouse input
    int forceAnimation = 0;             // variable that forces the display of the sprite's animation
    int forcedExit = 0;                 // variable that stores if the program has been forced to exit and in what way (e.g. 1=user pressed return) 

    char cam[24] = "Check the cameras often";
    char lights[52] = "Turn on the lights to check\nfor any unwanted visits";
    char doors[41] = "Close the doors to improve\nyour security";

    
    while(!tigrClosed(htpScreen) && forcedExit==0){
        
        count++;
        tigrClear(htpScreen, tigrRGB(0x00, 0x00, 0x00));

        tigrMouse(htpScreen, &mouseX, &mouseY, &leftButton);

        animate(htpScreen, mascot, anim, forceAnimation);

        drawStatic(htpScreen);

        tigrPrint(htpScreen, tfont, 10, 0, white, "HOW TO PLAY: ");
        tigrLine(htpScreen, 10, (tigrTextHeight(tfont, "H")), 80, (tigrTextHeight(tfont, "H")), white);
        
        

        if (tigrKeyDown(htpScreen, TK_DOWN)){
            htpSelected += 1;
            if (htpSelected > 2){
                htpSelected = 0;
            }
        }
        if (tigrKeyDown(htpScreen, TK_UP)){
            htpSelected -= 1;
            if (htpSelected < 0){
                htpSelected = 2;
            }
        }

        tigrRect(htpScreen, 10, 135, 200, 100, white);  // speech bubble
        drawTriangle(htpScreen, 209, 135, 10, white, black);
        

        switch (htpSelected){
            case 0:
                tigrPrint(htpScreen, tfont, 10, 24, lightYellow, cam);
                tigrPrint(htpScreen, tfont, 10, 48, lightGrey, lights);
                tigrPrint(htpScreen, tfont, 10, 84, lightGrey, doors);
                if ((!leftButton) || !((mouseX > coords[0]) && (mouseY > coords[1]))){
                    tigrPrint(htpScreen, tfont, 12, 137, white, "Check the monitor in the\nmiddle of the screen\nto see any visits");
                }
                break;
            case 1:
                tigrPrint(htpScreen, tfont, 10, 24, lightGrey, cam);
                tigrPrint(htpScreen, tfont, 10, 48, lightYellow, lights);
                tigrPrint(htpScreen, tfont, 10, 84, lightGrey, doors);
                if ((!leftButton) || !((mouseX > coords[0]) && (mouseY > coords[1]))){
                    tigrPrint(htpScreen, tfont, 12, 137, white, "If you turn the lights on,\nyou can see if the\nvisits have arrived yet");
                }
                break;
            case 2:
                tigrPrint(htpScreen, tfont, 10, 24, lightGrey, cam);
                tigrPrint(htpScreen, tfont, 10, 48, lightGrey, lights);
                tigrPrint(htpScreen, tfont, 10, 84, lightYellow, doors);
                if ((!leftButton) || !((mouseX > coords[0]) && (mouseY > coords[1]))){
                    tigrPrint(htpScreen, tfont, 12, 137, white, "If you close the doors,\nthe visits will go away\n(this is good btw)");
                }
                break;
        }
        if (leftButton){
            if ((mouseX > coords[0]) && (mouseY > coords[1])){
                forceAnimation = 1;
                if (htpSelected < 1){
                    tigrPrint(htpScreen, tfont, 12, 137, white, "PRO TIP FROM ALFREDO:\n\nDon't die! :)");
                }else{
                    if (htpSelected==1){
                        tigrPrint(htpScreen, tfont, 12, 137, white, "PRO TIP FROM ALFREDO:\n\nIf you see a weird warning,\nyou should be worried!");
                    }else{
                        tigrPrint(htpScreen, tfont, 12, 137, white, "PRO TIP FROM ALFREDO:\n\nIf you run out of energy\nyou can only pray to survive\nso don't waste it! ");
                    }
                }
            }
        }else{
            forceAnimation = 0;
        }


        if (tigrKeyDown(htpScreen, TK_BACKSPACE)){
            forcedExit = 1;       // stores 1 in forcedExit so that the program knows that it should continue in main menu
        }
        if (tigrKeyDown(htpScreen, TK_ESCAPE)){
            forcedExit = 2;       // stores 2 in forcedExit so that the program knows that it should exit everything in main menu
        }

        tigrUpdate(htpScreen);                          // updates changes made to the screen
    }
    tigrFree(mascot);                                   // frees the memory that stored the 'mascot' image
    tigrFree(anim);                                     // frees the memory that stored the 'anim' image
    return forcedExit;           
    // returns forcedExit so that the program knows if it should exit the program when esc is pressed or the window is closed
}


int mainMenu(Tigr *screen){
    int sel=0, difficulty=0, count=0, exitMenu=0;
    Tigr *fred = tigrLoadImage("assets/alfredo.png");    // loads alfredo's sprite
    Tigr *anim = tigrLoadImage("assets/ff_sprite_anim_menu.png");   // loads animation from alfredo's sprite
    Tigr *sign = tigrLoadImage("assets/sign.png");              // loads alfredo's name sign
    if (!fred){
        tigrError(0, "Cannot load alfredo, make sure to have downloaded 'assets' folder"); // displays error if it doesnt find sprite image
    }
    if (!anim){
        tigrError(0, "Cannot load alfredo's animation, make sure to have downloaded 'assets' folder"); // displays error if it doesnt find sprite image
    }
    if (!sign){
        tigrError(0, "Cannot load sign for alfredo, make sure to have downloaded 'assets' folder");
    }
    // && (difficulty!=13) 
    while(!tigrClosed(screen) && (sel!=13)){             // while this loop is running, the window will be open
        tigrClear(screen, black);
        tigrBlit(screen, sign, (screen->w - fred->w), (screen->h - fred->h - sign->h), 0, 0, sign->w, sign->h);
        animate(screen, fred, anim, 0);
        
        drawStatic(screen);
        
        reDraw(screen, sel);
        count++;

        if (tigrKeyDown(screen, TK_DOWN)){  // code to switch between options using the down arrow
            sel += 1;
            if (sel > 2){
                sel = 0;
            }
        }

        if (tigrKeyDown(screen, TK_UP)){    // code to switch between options using the up arrow
            sel -= 1;
            if (sel < 0){
                sel = 2;
            }
        }

        if (tigrKeyDown(screen, TK_ESCAPE)){
            difficulty = 13; // sets difficulty to 13 so that program exits without calling the next one
            sel = 13;        // sets sel to 13 so that the while loop ends
        }

        if (tigrKeyDown(screen, TK_RETURN)){     
            switch (sel){
                case 0:
                    clearTerminal();
                    difficulty = selectDifficulty(screen);
                    if (difficulty!=14){    
                        // 13 and 14 are the invalid values for difficulty, but it is only meant
                        // to exit if difficulty is 13, if it is 14, it should only go back to main menu
                        sel = 13;           // sets sel to 13 so that the while loop ends
                    }
                    break;
                case 1:
                    clearTerminal();
                    exitMenu = showHTP(screen);
                    if (exitMenu==0 || exitMenu==2){
                        difficulty = 13;    // this will make the program end without calling anything else after it
                        sel = 13;           // this forces the program to exit the while loop
                    }
                    break;
                case 2:
                    clearTerminal();
                    exitMenu = showAuthors(screen);
                    if (exitMenu==0 || exitMenu==2){
                        difficulty = 13;    // this will make the program end without calling anything else after it
                        sel = 13;           // this forces the program to exit the while loop
                    }
                    break;
            }
        }

        

        tigrUpdate(screen);     // needed to 'save' changes made to the window and display them
    } 

    tigrFree(anim);     // makes the memory available again
    tigrFree(fred);     // makes the memory available again
    tigrFree(screen);   // makes the memory available again
    return difficulty;
}

int showAuthors(Tigr *screen){
    Tigr *background1 = tigrLoadImage("assets/gigachad_high.png");
    Tigr *background2 = tigrLoadImage("assets/gigachad_wide.png");
    int count=0, forcedExit=0, sel=1, selmusic=0;
    int animationActive=0;

    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE) && sel!=13){
        tigrClear(screen, black);

        if (animationActive){
            if (count>=400){
                count=0;
            }   // This resets the animation
            count++;
            tigrClear(screen, tigrRGBA(0x39, 0x39, 0x39, 0xff));
            animateAuthorsBg(screen, background1, background2, count);
            tigrPrint(screen, tfont, 5, screen->h-12, lightGrey, "Authors IRL while making this game :");
        }else{
            tigrPrint(screen, tfont, 5, screen->h-12, lightGrey, "Press ENTER :)");
        }
        
        tigrPrint(screen, tfont, 5, 5, white, "PROJECT MADE BY: ");
        tigrLine(screen, 5, 17, 5+tigrTextWidth(tfont, "PROJECT MADE BY: "), 17, white);

        switch (sel){
        case 1:
            tigrPrint(screen, tfont, 10, 24, lightYellow, "Iker Marroquín  (Tigr designer)");
            tigrPrint(screen, tfont, 10, 36, lightGrey, "Javier Villanueva");
            tigrPrint(screen, tfont, 10, 48, lightGrey, "Marcos Márquez");
            break;
        case 2:
            tigrPrint(screen, tfont, 10, 24, lightGrey, "Iker Marroquín");
            tigrPrint(screen, tfont, 10, 36, lightYellow, "Javier Villanueva (Main game designer)");
            tigrPrint(screen, tfont, 10, 48, lightGrey, "Marcos Márquez");
            break;
        case 3:
            tigrPrint(screen, tfont, 10, 24, lightGrey, "Iker Marroquín");
            tigrPrint(screen, tfont, 10, 36, lightGrey, "Javier Villanueva");
            tigrPrint(screen, tfont, 10, 48, lightYellow, "Marcos Márquez (Sound effects designer)");
            break;        
        default:
            break;
        }

        if (tigrKeyDown(screen, TK_DOWN)){
            sel++;
        }
        if (tigrKeyDown(screen, TK_UP)){
            sel--;
        }
        if (sel>3){
            sel=1;
        }
        if (sel<1){
            sel=3;
        }

        if (tigrKeyDown(screen, TK_RETURN) && count>=1){
            animationActive = !animationActive;
            if (selmusic==0){
                selmusic=1;
            	int pid = fork();
		if(pid==0){ 
		    system("mpg123 -q sound/giga-chad-theme.mp3");
		    sleep(1);
		    exit(0);
		}
            } else
            {
                system("pkill -f \"sound/giga-chad-theme.mp3\"");
            	selmusic=0;
            }
            

        }   // toggles animation on / off

        if (count<1){
            count += 1;
        }   // needed so that the animation doesn't start when window is opened

        if (tigrKeyDown(screen, TK_BACKSPACE)){
            forcedExit = 1;       // stores 1 in forcedExit so that the program knows that it should continue in main menu
            sel = 13;

        }
        if (tigrKeyDown(screen, TK_ESCAPE)){
            forcedExit = 2;       // stores 2 in forcedExit so that the program knows that it should exit everything in main menu
            sel = 13;
        }
        
        tigrUpdate(screen);
    }
    tigrFree(background1);
    tigrFree(background2);
    return forcedExit;
}

void animateAuthorsBg(Tigr *screen, Tigr *img1, Tigr *img2, int count){
    if (count<=150){
        tigrBlit(screen, img1, 0, -count, 0, 0, img1->w, img1->h);
    }
    if (count>150){
        tigrBlitAlpha(screen, img1, 0, -150, 0, 0, img1->w, img1->h, 0);
        tigrBlitAlpha(screen, img2, count-250, 0, 0, 80, img2->w, img2->h, 255);
        // tigrPrint(screen, tfont, 0, 0, tigrRGB(255, 255, 255), "transp is: %d", (((float)count-150)/(float)20)*(int)255);
    }
    
}

