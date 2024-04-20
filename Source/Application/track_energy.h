#ifndef TRACK_ENERGY
#define TRACK_ENERGY
#include <stdint.h>
#include <stdbool.h>
extern uint8_t track_on[3];
extern bool update_track_energy_io;

/*
init PF2(track1), PF3(track2), PF4(track3) as push-pull output, and immediately switch off
low - switch track power off
hi  - switch track power on
*/
void init_track_energy_gpio(void);
void track_energy_switch(void);
/*
init PF8(track1), PF9(track2), PF10(track3) as ADC input, and start 
monitoring current driver analog_watchdogs_IT
*/
void init_track_analog_watchdog(void);
void analog_watchdogs_IT(void);
void change_analog_watchdog_channel(void);

#endif // !1