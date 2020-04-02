# Introduction
This is my build of DWM. I tried to keep it as simple as possible, so there is no need to have a .Xresources or .Xdefaults file in your home directory to get things to work. As long as you can compile this, dwm should work. 

For the most part, the key bindings are the same. I use GruvBox theme as my theme. There are also some differences such as MOD+Space opens rofi, MOD+q quits the focused program, etc. You can look through the file and find the rest. Also, I changed the centered master patch to have gaps using the fullgaps patch.

Be sure to checkout my build of [dwmblocks](https://github.com/alexb7711/dwmblocks) as well if you like this :).

# "Dependencies"
Just some of the programs that I call, and if you want them to work you should have them installed.

* rofi [https://github.com/adi1090x/rofi](https://github.com/adi1090x/rofi)
* playerctl
* pulseaudio (pactl)
* lightdm
* Inconsolata Nerd Font [https://github.com/ryanoasis/nerd-fonts](https://github.com/ryanoasis/nerd-fonts)

# Patches
* FullGaps
* CenteredMaster (with gaps)
* MoveStack
* ActualFullScreen
* FakeFullScreen

# What is DWM?
As described by suckless:

>dwm - dynamic window manager
>============================
>dwm is an extremely fast, small, and dynamic window manager for X.
>
>
>Requirements
>------------
>In order to build dwm you need the Xlib header files.
>
>
>Installation
>------------
>Edit config.mk to match your local setup (dwm is installed into
>the /usr/local namespace by default).
>
>Afterwards enter the following command to build and install dwm (if
>necessary as root):
>
>    make clean install
>
>
>Running dwm
>-----------
>Add the following line to your .xinitrc to start dwm using startx:
>
>    exec dwm
>
>In order to connect dwm to a specific display, make sure that
>the DISPLAY environment variable is set correctly, e.g.:
>
>    DISPLAY=foo.bar:1 exec dwm
>
>(This will start dwm on display :1 of the host foo.bar.)
>
>In order to display status info in the bar, you can do something
>like this in your .xinitrc:
>
>    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
>    do
>    	sleep 1
>    done &
>    exec dwm
>
>
>Configuration
>-------------
>The configuration of dwm is done by creating a custom config.h
>and (re)compiling the source code.
