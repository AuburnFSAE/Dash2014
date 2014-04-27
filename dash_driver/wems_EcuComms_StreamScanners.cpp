#include "wems_EcuComms_StreamScanners.h"

void (*wems_EcuComms_Action)(WemsEcuBufferBlock*) = &wems_EcuComms_VoidScan;

WemsEcuBufferBlock* wems_EcuComms_Init(si16 n_channel_count, si16 n_comm_pin){

	WemsEcuBufferBlock* ecu_block = 
	wems_EcuComms_GetEcuBlock( n_channel_count, n_comm_pin );
	ecu_block->scan_complete = 0;

	wems_EcuComms_Action = &wems_EcuComms_HeaderScan;
return ecu_block;}

void wems_EcuComms_HeaderScan(WemsEcuBufferBlock* comm_blk){
    comm_blk->bitque >>= 1;
    comm_blk->bitque |= ((ui32)digitalRead( comm_blk->comm_pin )) << 31;

    
    if( (comm_blk->bitque & HEADERMASK ) == comm_blk->header ){
		comm_blk->scan_complete=0;
		comm_blk->channel_pos=0;
		comm_blk->bit_pos=30;
		wems_EcuComms_Action = &wems_EcuComms_CountScan;
    }

return;}

void wems_EcuComms_CountScan(WemsEcuBufferBlock* comm_blk){
    comm_blk->bitque >>= 1;
    comm_blk->bitque |= ((ui32)digitalRead( comm_blk->comm_pin )) << 31;
    comm_blk->bit_pos--;

    if( comm_blk->bit_pos == 0 ){
		comm_blk->ecu_channel_count = (0x00FFu & comm_blk->bitque);
		comm_blk->bit_pos=10;
		
		wems_EcuComms_Action = &wems_EcuComms_DataScan;
    }
return;}

void wems_EcuComms_DataScan(WemsEcuBufferBlock* comm_blk){
    comm_blk->bitque >>= 1;
    comm_blk->bitque |= ((ui32)digitalRead( comm_blk->comm_pin )) << 31;
    comm_blk->bit_pos--;

    if( comm_blk->bit_pos == 0 ){		
		comm_blk->bit_pos=20;
	
		comm_blk->ecu_stream.data[comm_blk->channel_pos] = 
		(((comm_blk->bitque >> 10)&0xFF)) | ((comm_blk->bitque & 0xFF)<< 8);

		if( ++comm_blk->channel_pos == comm_blk->num_channels ){
			comm_blk->bitque = 0;
			comm_blk->scan_complete=1;
			wems_EcuComms_Action = &wems_EcuComms_ResetHeaderWait;
		}
    }
return;}

void wems_EcuComms_VoidScan(WemsEcuBufferBlock* comm_blk){return;}

void wems_EcuComms_ResetHeaderWait(WemsEcuBufferBlock* comm_blk){

	if( comm_blk->scan_complete == 0)
		wems_EcuComms_Action = &wems_EcuComms_HeaderScan;

return;} 
