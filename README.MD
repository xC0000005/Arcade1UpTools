This is a set of tools for interacting with the Arcade1up Hardware.
The GPIO is all using the SUNXI GPIO example code and is made to interface between a menu launcher like the stock one and whatever the script wants to do (aka, you could have holding a button change game load behavior)

gcc -o a1upgpio main.c gpio_lib.c -std=c99 -static

Video is all (only), apparently, Framebuffer direct, with a BGRAAA format at 640x480. I haven't been able to get the EGL library on the board to work, and the code that intereacts with it cats pre-prepared images to the framebuffer (the game menu/launcher) or memcopy/fwrites render buffers straight in.


