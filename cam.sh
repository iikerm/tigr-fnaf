mpg123 -q sound/blip3.mp3 2>/dev/null &

reproducir_en_bucle_camera() {
  while true; do
    mpg123 -q --title "sound/camera.mp3" 2>/dev/null
    wait $!
  done
}

reproducir_en_bucle_camera 2>/dev/null &


./camera


pkill -9 camera

pkill -STOP -f "sound/camera.mp3"
mpg123 -q sound/put_down.mp3 2>/dev/null &





