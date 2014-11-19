

#ifndef ALERT_H
#define ALERT_H 
	#include "wems.h"

    struct Alert{
        ui8 pattern;
        ui8 pin_count;
        si16 cycle_count;
        bool is_active;
        bool triggered;
    };

    inline Alert wems_alert_Create(ui8 n_pattern, ui8 n_pin_count){
    	Alert alert;

		alert.pattern=n_pattern;
		alert.pin_count=n_pin_count;
		alert.cycle_count=0;
		alert.is_active=false;
		alert.triggered=false;
    return alert;}

    inline Alert wems_alert_Process(Alert alert, const wems_rpm_block* blk, const Gauge* guage_list, si16 numgauges){

            alert.is_active=false;
            for(si16 ig=0; ig<numgauges && !alert.is_active; ig++){
                alert.is_active |= gauge_list[ig].is_alert;
            }

            if(alert.is_active){
                alert.triggered=true;
                //toggle the alertpattern an even number of times so that its state is not changed
                for(si16 n=0; n<2; n++){
                for(si16 pin=0; pin<alert.pin_count; pin++){
                    digitalWrite(blk->pins[pin], alert.pattern & (1<<pin));
                }
                    if(alert.cycle_count%10==0)                
                    alert.pattern= ~alert.pattern;

                    if(alert.cycle_count++==1000){
                        alert.cycle_count=0;
                    }

                }
            }else if(alert.triggered){
                wems_rpm_set_lights(&rpm_blk, 0);
                alert.triggered=false;
            }
    return alert;}

#endif

