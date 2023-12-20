#!/bin/bash

# chmod +x start.sh
# ./start.sh

gcc menu_tigr.c -lGLU -lGL -lX11 -o menu
echo "menu_tigr.c compiled successfully"

gcc fnaf.c -o fnaf
echo "fnaf.c compiled successfully"

gcc final_screen.c -lGLU -lGL -lX11 -o final_screen
echo "final_screen.c compiled successfully"

gcc camera.c -lGLU -lGL -lX11 -o camera
echo "camera.c compiled successfully"

gcc security_cameras.c -lGLU -lGL -lX11 -o cameras
echo "security_cameras.c compiled successfully"

gcc defeat.c -lGLU -lGL -lX11 -o defeat
echo "defeat.c compiled successfully"

gcc nightmodules.c -lGLU -lGL -lX11 -o nightmodules
echo "nightmodules.c compiled successfully"

gcc nightmodules2.c -lGLU -lGL -lX11 -o nightmodules2
echo "nightmodules2.c compiled successfully"





reproducir_en_bucle_menu() {
while true; do
    mpg123 -q sound/menu.mp3 2>/dev/null
done
}

touch output.txt

continue=1

while [[ $continue == 1 ]];do


    reproducir_en_bucle_menu &
    ./menu > output.txt 2>&1

    pkill -9 menu
    last_result=$(tail -n 1 output.txt)

    pkill -TSTP mpg123
    sleep 0.1
    pkill -TERM mpg123

    if [[ $last_result == 13 ]]; then
        continue=0
	
    else
        ./fnaf
        pkill -9 fnaf
    
    fi 
done

rm -r output.txt
