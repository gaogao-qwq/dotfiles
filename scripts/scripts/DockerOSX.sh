docker run -it \
    --device /dev/kvm \
	--device /dev/snd \
    -p 50922:10022 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e "DISPLAY=${DISPLAY:-:0.0}" \
    -e AUDIO_DRIVER=pa,server=unix:/tmp/pulseaudio.socket \
	-e EXTRA='-smp 16,sockets=8,cores=2' \
	-e RAM=8 \
    -v "/run/user/$(id -u)/pulse/native:/tmp/pulseaudio.socket" \
    -e GENERATE_UNIQUE=true \
    -e MASTER_PLIST_URL='https://raw.githubusercontent.com/sickcodes/osx-serial-generator/master/config-custom.plist' \
    sickcodes/docker-osx:latest
