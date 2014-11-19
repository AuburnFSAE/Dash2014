
#include <Arduino.h>
#include <stdlib.h>
#include "wems_typedef.h"

#ifndef WEMS_RPM_ALLOC_H
#define WEMS_RPM_ALLOC_H

/** @page RPM RPM API.
	wems_rpm_alloc.h defines the wems_rpm_block api.
	This part of the api encapsulates a sets of pins that are used to
	control led lights through the wems_rpm_block and rpm functions.
*/


/**
@bug 
   There seems to be some problem with the rpm struct and indirect pointers. for instance
   if inside a function, the call _rpm_struct->pins[0] is made it crashes the controller. 
   The problem does not seem to affect the ecu_block structure, which has 1 fewer pointer 
   than rpm_blk. For now rpm_blk is set so that all memory is allocated statically, which is 
   inconvient for and end user but seems to be stable.
*/

/** @struct wems_rpm_block
	@brief encapsulates rpm lights that are defined by the pins member.
	
	@var wems_rpm_block::pins
	The array pins that the controler uses to control

	@var wems_rpm_block::count
	The number of led pins controled.
	
	@var wems_rpm_block::active_level 
   	Member 'active_level' defines whether the pins should be activated high or low.
   	For Arduino source it should be set by wems_rpm_block.active_level = HIGH
   	By default wems_rpm_block are create to be active HIGH

   	@var wems_rpm_block::table
   	A table(ie array) that holds the rpm values that are mapped to the rpm leds. For instance 
   	setting table to [7000, 10500, 12000] will trigger the rpm lights at those rpm values.
   	Below is an example of allocating an rpm struct with certian rpm values
   	@code
		wems_rpm_block rpm;
		const si16 pin_count=3;
		si16 pins[pin_count]={2,3,4};
		si16 rpm_table[pin_count]={7000, 10500, 12000};

		wems_rpm_Init(rpm, pins, rpm_table, pin_count);
   	@endcode
 */
struct wems_rpm_block{
	si16* pins;
	si16 count;
	si16 min_rpm;
	si16 max_rpm;
	si16 rpm_range;
	si16 active_level;
	si16* table;
	si16  table_size;
};



/** Initializes a wems_rpm_block*.
	Initializes a rpm struct and returns the reference to it. The values of its rpm table and pins will be copied from
	rpm_table and pins respectivly. rpm_table and pins are expected to be the same length. 
	The min and max values are set according to the first and last element of the rpm_table.
*/
void wems_rpm_Init(wems_rpm_block* blk, si16* pins, si16* rpm_table, si16 pin_count);


// wems_rpm_alloc_ligthstable allocates blk->table to the values in rpm_values.
void wems_rpm_alloc_ligthstable(wems_rpm_block* blk, const si16* rpm_values, si16 num_rpm_values);

void wems_rpm_set_lights(const wems_rpm_block* blk, const si16 rpm);

void wems_rpm_alloff(const wems_rpm_block* blk);

void wems_rpm_pulsepin(const wems_rpm_block* blk,  si16 pin, si16 pulse_count);

#endif

