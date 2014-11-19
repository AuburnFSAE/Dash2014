#include "wems_rpm_alloc.h"


void wems_rpm_Init(wems_rpm_block* blk, si16* rpm_pins, si16* rpm_table, si16 pin_count){

	blk->pins=rpm_pins;
	blk->table=rpm_table;
	blk->count=pin_count;

	blk->active_level = HIGH;
	
	blk->table_size=pin_count;

	blk->min_rpm=rpm_table[0];
	blk->max_rpm=rpm_table[pin_count-1];
	for( si16 i = 0; i < blk->count; i++){
		pinMode(blk->pins[i], OUTPUT);
		digitalWrite(blk->pins[i], LOW);
	}
return;}




void wems_rpm_set_lights(const wems_rpm_block* blk, const si16 rx){
	for(si16 ilow=0; ilow<blk->table_size; ilow++){
		digitalWrite(blk->pins[ilow], LOW); 
	}

	for(si16 ihgh=0; ihgh<blk->table_size; ihgh++){
		if(rx>blk->table[ihgh])digitalWrite(blk->pins[ihgh], HIGH);
	}
return;}


void wems_rpm_alloff(const wems_rpm_block* blk){
	for(si16 ilow=0; ilow<blk->table_size; ilow++){
		digitalWrite(blk->pins[ilow], LOW); 
	}
return;}


void wems_rpm_pulsepin(const wems_rpm_block* blk, si16 pin, si16 pulse_count){
	//multiply by 2 to get even on and offs with mod 2
	for(int i=0; i<pulse_count; i++){
		//pulses the shift pin 
		digitalWrite(blk->pins[pin],  HIGH );
		delay(3);
		digitalWrite(blk->pins[pin], LOW);
		delay(3);
	}
}
