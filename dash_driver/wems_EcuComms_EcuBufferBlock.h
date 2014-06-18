/** Author Sherrod James 2014 */

#ifndef ECU_COMMS_BLOCK
#define ECU_COMMS_BLOCK
#include "wems_typedef.h"
#include "wems_DataStream.h"

/** @page EcuBuffer EcuBufferBlock Api.
	wems_EcuComms_EcuBufferBlock.h defines the WemsEcuBufferBlock api.
	This part of the api defines the functions that control and read an 
	array of data that represents a set of channels from the ecu. It also provides
	functions for scanning and enumerating the data from a stream.
*/

struct WemsEcuBufferBlock{
	volatile ui8  scan_complete;
	ui32 header;
	ui32 bitque;
	ui16 channel_pos;
	ui16 bit_pos;
	si16 channel_count;
	si16 comm_pin;
	void* object_data;
	si16 reset_cnt;
	DataStreamSI16 ecu_stream;
};

const ui32 HEADERMASK = 0x3FFFFFFFu;

ui32 wems_EcuComms_GetHeader(void);

WemsEcuBufferBlock* wems_EcuComms_GetEcuBlock(si16 n_num_channels, si16 n_comm_pin);


#endif

