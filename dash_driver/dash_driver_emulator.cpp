#include "dash_driver.h"
bool temp_tggle=0;

void dash_driver_emulator(){
	//pause interrupt to enable high speed serial
	//pause_interrupts();
	
	digitalWrite(ITCPin, temp_tggle = !temp_tggle);

	while(_ecu_blk->scan_complete==0){
		//Terminal code goes here
		while(TERM.available() < 4);
		
		TERM.readBytes((si8*)packet, PacketLength);
		
		//Analyze packet	
		if(packet[0]==ACK || packet[0]==ENQ || packet[0]==SOH){

			if(packet[0]==SOH){

				_ecu_blk->ecu_stream.data[packet[1]] = 
				((packet[2] & 0x00FF)<<8) | ((packet[3] & 0x00FF));
				
				//once all data sent switch scan_complete to true
				if(packet[1]==_ecu_blk->channel_count-1){
					_ecu_blk->scan_complete=1;
					time_ref=millis();
				}
			}
		}

		packet[2]=packet[0];
		packet[0]=ENQ;
		packet[1]=ENQ;
		packet[3]=0;
		
		TERM.write(packet, PacketLength);
	}

  
	dash_driver_core();
	
	if((millis()-time_ref)>reset_mills)
		_ecu_blk->scan_complete=0;
return;}
