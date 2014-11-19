 #include "dash_driver.h"

void do_nothing_write(void*, si16){
	return;
}


void genie_write(void* n_ecu_blk, si16 numgauges){

	WemsEcuBufferBlock* blk=(WemsEcuBufferBlock*)n_ecu_blk;
	Gauge* gauges = (Gauge*)blk->object_data;
	Gauge* gauge;

	for(int ig=0; ig<numgauges; ig++){
		gauge=gauges+ig;//pointer arithmateic look it up

		genieWriteObject(GENIE_OBJ_LED_DIGITS,
			gauge->id,
			blk->ecu_stream.data[gauge->ecu_channel]/gauge->scale);

		if(gauge->number!=-1){
			if( gauge->is_alert && !gauge->is_active){
				genieWriteObject(GENIE_OBJ_GAUGE,
					gauge->number,
					gauge->alert_on);

				gauge->is_alert=false;
				gauge->is_active=true;

			}else if( !gauge->is_alert && gauge->is_active) {
				genieWriteObject(GENIE_OBJ_GAUGE,
					gauge->number,
					gauge->alert_off);	
				gauge->is_active=false;		
			}
		}

	}
return;}


void portforward_write(void* n_ecu_blk, si16 numgauges){

	WemsEcuBufferBlock* blk=(WemsEcuBufferBlock*)n_ecu_blk;
	Gauge* gauges = (Gauge*)blk->object_data;

	if( blk->scan_complete || 1 ){	
		ui8 channel=0;
		ui16 flag_package=0;
		packet[0]=0xFF;
		packet[1]=0xFF;

		do{	

			//write gauge and check for flag
				if( channel < blk->channel_count ){
					packet[2]=(blk->ecu_stream.data[channel]&0xFF00)>>8;
					packet[3]= blk->ecu_stream.data[channel]&0x00FF;

					for(int ig=0; ig<numgauges; ig++){
						if(gauges[ig].ecu_channel==channel && gauges[ig].is_alert)
							flag_package|= 1<<channel;
					}

				}else{
					//packet[0]=SOH;
					//packet[1]=channel;
					packet[2]=(flag_package&0xFF00)>>8;
					packet[3]=flag_package&0x00FF;
				}

				TERM.write(packet, PacketLength);
				
				TERM.write(channel);
				delay(10);
				channel++;
				
		}while( channel < blk->channel_count+1);


		for(int ig=0; ig<numgauges; ig++){
			gauges[ig].is_alert=false;
		}	
	}	
return;}
