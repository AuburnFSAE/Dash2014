#include "wems_EcuComms_StreamScanners.h"


void (*wems_EcuComms_Action)(WemsEcuBufferBlock*) = &wems_EcuComms_VoidScan;

WemsEcuBufferBlock* wems_EcuComms_Init(si16 n_channel_count, si16 n_comm_pin){

	WemsEcuBufferBlock* ecu_block = 
	wems_EcuComms_GetEcuBlock( n_channel_count, n_comm_pin );
	ecu_block->scan_complete=false;

	wems_EcuComms_Action = &wems_EcuComms_HeaderScan;
	
return ecu_block;}


void wems_EcuComms_HeaderScan(WemsEcuBufferBlock* ecu_blk){
    ecu_blk->bitque >>= 1;
    ecu_blk->bitque |= ((ui32)digitalRead( ecu_blk->comm_pin )) << 31;

    if( (ecu_blk->bitque & HEADERMASK ) == ecu_blk->header ){
		ecu_blk->scan_complete=false;
		ecu_blk->channel_pos=0;
		ecu_blk->bit_pos=30;
		wems_EcuComms_Action = &wems_EcuComms_CountScan;
    }

return;}


void wems_EcuComms_CountScan(WemsEcuBufferBlock* ecu_blk){
    ecu_blk->bitque >>= 1;
    ecu_blk->bitque |= ((ui32)digitalRead( ecu_blk->comm_pin )) << 31;
    ecu_blk->bit_pos--;

    if( ecu_blk->bit_pos == 0 ){
		ecu_blk->channel_count = (0x00FFu & ecu_blk->bitque);

		ecu_blk->bit_pos=10;
		if(ecu_blk->channel_count==ecu_blk->ecu_stream.word_count)
			wems_EcuComms_Action = &wems_EcuComms_DataScan;
		else
			wems_EcuComms_Action=&wems_EcuComms_HeaderScan;
    }
return;}


void wems_EcuComms_DataScan(WemsEcuBufferBlock* ecu_blk){
    ecu_blk->bitque >>= 1;
    ecu_blk->bitque |= ((ui32)digitalRead( ecu_blk->comm_pin )) << 31;
    ecu_blk->bit_pos--;

    if( ecu_blk->bit_pos == 0 ){		
		ecu_blk->bit_pos=20;
	
		ecu_blk->ecu_stream.data[ecu_blk->channel_pos] = 
		(((ecu_blk->bitque >> 10)&0xFF)) | ((ecu_blk->bitque & 0xFF)<< 8);

		if( ++ecu_blk->channel_pos == ecu_blk->channel_count ){
			ecu_blk->bitque = 0;
			ecu_blk->scan_complete=true;
			wems_EcuComms_Action = &wems_EcuComms_ResetHeaderWait;
		}
    }
return;}


void wems_EcuComms_VoidScan(WemsEcuBufferBlock* ecu_blk){return;}


void wems_EcuComms_ResetHeaderWait(WemsEcuBufferBlock* ecu_blk){

	if( ecu_blk->scan_complete == false)
		wems_EcuComms_Action = &wems_EcuComms_HeaderScan;
return;} 
