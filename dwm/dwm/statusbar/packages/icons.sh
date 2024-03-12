#! /bin/bash
# ICONS 部分特殊的标记图标 这里是我自己用的，你用不上的话去掉就行

tempfile=$(cd $(dirname $0);cd ..;pwd)/temp

this=_icons
icon_color="^c#FFFFFF^^b#1111110x88^"
text_color="^c#FFFFFF^^b#1111110x88^"
signal=$(echo "^s$this^" | sed 's/_//')

with_v2raya() {
    [ "$(ps aux | grep -v grep | grep 'v2raya')" ] && icons=(${icons[@]} "")
}

with_bluetooth() {
    [ ! "$(command -v bluetoothctl)" ] && echo command not found: bluetoothctl && return
    [ "$(bluetoothctl info 14:3F:A6:47:9A:80 | grep 'Connected: yes')" ] && icons=(${icons[@]} " ")
}

update() {
    text=("|")
    icons=("")
    #with_v2raya
    with_bluetooth

    icon="${icons[@]}"
    text="${text}"

    sed -i '/^export '$this'=.*$/d' $tempfile
    printf "export %s='%s%s%s%s%s'\n" $this "$signal" "$icon_color" "| $icon " "$text" >> $tempfile
}

notify() {
    texts=""
    #[ "$(ps aux | grep -v grep | grep 'v2raya')" ] && texts="$texts\n v2raya 已启动"
    [ "$(bluetoothctl info 14:3F:A6:47:9A:80 | grep 'Connected: yes')" ] && texts="$texts\n WH-1000XM3 已链接"
    [ "$texts" != "" ] && notify-send " Info" "$texts" -r 9527
}

call_menu() {
    ~/.config/rofi/powermenu/type-3/powermenu.sh
	# case $(echo -e ' 关机\n 重启\n 休眠\n 锁定' | rofi -dmenu -window-title power -theme ~/scripts/config/rofi.rasi) in
    #     " 关机") poweroff ;;
    #     " 重启") reboot ;;
    #     " 休眠") systemctl hibernate ;;
    #     " 锁定") ~/scripts/blurlock.sh ;;
    # esac
}

click() {
    case "$1" in
        L) feh --randomize --bg-fill ~/Pictures/wallpaper/* ;;
        R) call_menu ;;
    esac
}

case "$1" in
    click) click $2 ;;
    notify) notify ;;
    *) update ;;
esac
