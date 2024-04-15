#include "track_energy.h"

uint8_t track_on[3];
bool update_track_energy_io;

void init_track_energy_gpio(void)
{
    memset(track_on,0,3);
    return;
}

void track_energy_switch(void)
{
    return;
}