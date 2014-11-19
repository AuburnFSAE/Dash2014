#include <Arduino.h>
#include <stdlib.h>
#include "wems_typedef.h"

#ifndef WEMS_RPM_ALLOC_H
#define WEMS_RPM_ALLOC_H

/** wems_rpm_alloc defines the rpm block and shiftlights api.
This part of the api encapsulates a sets of pins that are used to
control led lights through the wems_rpm_block data type.
*/


/** @struct wems_rpm_block
 *  @brief This structure encapsulates rpm lights that are defined by the pins member
 *  @var wems_rpm_block::active_level 
 *  	Member 'active_level' defines whether the pins should be activated high or low.
 *  	For Arduino source it should be set by wems_rpm_block.active_level = HIGH
 *  	By default wems_rpm_block are create to be active HIGH
 */
struct wems_rpm_block{
	si16* pins;
	si16 count;
	si16 min_rpm;
	si16 max_rpm;
	si16 rpm_range;
	si16 active_level;
	si16 shift_pin;
	si16 shift_rpm;
	si16 shift_level;
	si16* map;
	si16  map_size;
};


#define LEVELFLIP(x) x==HIGH ? LOW : HIGH

void wems_rpm_alloc( wems_rpm_block* blk, si16* rpm_pins, si16 n_count);

void wems_rpm_minmax_alloc(	wems_rpm_block* blk,
		si16* rpm_pins,
		si16 num_pins,
		si16 min,
		si16 max);

void wems_rpm_set_shiftpoint(wems_rpm_block* blk, si16 shift_rpm, ui8 shift_pin);

/** wems_rpm_set_ligths479 partitions the rpm band into three blocks.
	The blocks are such that the lights are ratio by (4/9) then (7/9) and (9/9). 
	By this pattern light 1-4 will light 	when entering the band, 
	then 5-7 for the middle band and finally 8-9 for the final band.
*/
void wems_rpm_set_ligths479(const wems_rpm_block* blk, si16 rpm_val, ui8 direction);

void wems_rpm_toggle_shift2(const wems_rpm_block* blk, si16 rpm_val, si16 ms_delay );

/** wems_rpm_set_ligthsmap directly partitions rpm pins to individual values.
	wems_rpm_set_lightsmap is useful when nonlinear ranges are desired.
*/
void wems_rpm_set_lightsmap(const wems_rpm_block* blk, const si16 rx);

void wems_rpm_reset(const wems_rpm_block* blk);

#endif

