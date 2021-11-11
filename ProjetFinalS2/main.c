#include <allegro.h>
#include <stdio.h>
#include "proto.h"


int main()
{
    allegro_init();
    install_keyboard();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    install_timer();
    set_window_title("ECE-TYPE");

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    menuPrincipal();
    return 0;
}
END_OF_MAIN();


