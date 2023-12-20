pkill -TSTP mpg123
sleep 1
mpg123 -q sound/6am.mp3 2>/dev/null &
./nightmodules2

pkill -9 nightmodules2
