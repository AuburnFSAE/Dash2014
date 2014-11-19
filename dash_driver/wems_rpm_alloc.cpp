#include "wems_rpm_alloc.h"

void wems_rpm_alloc( wems_rpm_block* blk, si16* rpm_pins, si16 n_count){
	blk->pins = rpm_pins;

	blk->count=n_count;
	blk->min_rpm=6000;
	blk->max_rpm=13000;

	blk->rpm_range = blk->max_rpm - blk->min_rpm;
	blk->active_level = HIGH;
	for( si16 i = 0; i < blk->count; pinMode(rpm_pins[i++], OUTPUT));	

	for( si16 i = 0; i < blk->count; digitalWrite(rpm_pins[i++], LOW));
return;}


void wems_rpm_minmax_alloc(wems_rpm_block* blk, si16* rpm_pins, si16 count, si16 min_rpm, si16 max_rpm){
	wems_rpm_alloc(blk, rpm_pins, count);

	blk->min_rpm=min_rpm;
	blk->max_rpm=max_rpm;
	blk->rpm_range = blk->max_rpm - blk->min_rpm;
return;}


void wems_rpm_set_shiftpoint(wems_rpm_block* blk, si16 shift_rpm, ui8 shift_pin){
	blk->shift_pin = shift_pin;
	blk->shift_rpm = shift_rpm;

	digitalWrite( blk->shift_pin, LEVELFLIP(blk->shift_level) );
return;}


void wems_rpm_set_lights479(const wems_rpm_block* rpm_blk, si16 rpm_val, ui8 direction){	
	si16* leds = rpm_blk->pins;

	si16 _x_ = ((si32)rpm_blk->count * (rpm_val - rpm_blk->min_rpm))/rpm_blk->rpm_range;
	sflt eq_p = (rpm_blk->count/8.999);

	if( direction ){
		  si16 n=0;
		 if( _x_ > (n=4*eq_p) ){
		    for( si16 i=0; i<n; digitalWrite(leds[i++], rpm_blk->active_level) ); 
		}if( _x_ > (n=7*eq_p, n) ){
		    for( si16 i=4*eq_p; i < n; digitalWrite(leds[i++], rpm_blk->active_level) );

		}if( _x_ == (n=9*eq_p)){
		    for( si16 i=7*eq_p; i < n; digitalWrite(leds[i++], rpm_blk->active_level) );
		}
	}else{

	  if( _x_ < 8*eq_p )
	    for( si16 i=rpm_blk->count-1; i > 6*eq_p; digitalWrite(leds[i--], !(rpm_blk->active_level)) );

	  if( _x_ < 5*eq_p)
	    for( si16 i=6*eq_p; i > 3*eq_p; digitalWrite(leds[i--], !(rpm_blk->active_level)) );

	  if( _x_ < 1*eq_p )
	    for( si16 i=3*eq_p; i>-1; digitalWrite(leds[i--], !(rpm_blk->active_level)) );
	}

	wems_rpm_toggle_shift2(rpm_blk, rpm_val, 4);
return;}


void wems_rpm_set_lightsmap(const wems_rpm_block* blk, const si16 rx){
	for(si16 ilow=0; ilow<blk->map_size; ilow++){
		digitalWrite(blk->pins[ilow], LOW); 
	}

	for(si16 ihgh=0; ihgh<blk->map_size; ihgh++){
		if(rx>blk->map[ihgh])digitalWrite(blk->pins[ihgh], HIGH);
	}

return;}

void wems_rpm_reset(const wems_rpm_block* blk){
	for(si16 ilow=0; ilow<blk->map_size; ilow++){
		digitalWrite(blk->pins[ilow], LOW); 
	}
return;}

void wems_rpm_toggle_shift2(const wems_rpm_block* blk, si16 rpm_val, si16 ms_delay ){
	ui8 tggl = blk->shift_level;
	if( rpm_val >= blk->shift_rpm )
	for( int i =0; i < 4; i++){ digitalWrite(blk->shift_pin, (delay(ms_delay),tggl=!tggl, tggl) );}
}
