/** Author Sherrod James 2014 */

#ifndef ECU_COMMS_BLOCK
#define ECU_COMMS_BLOCK
#include "wems_typedef.h"
#include "wems_DataStream.h"


/**
\file
*/

/**
	This header is one file the descirbes the ecu-comms namespace.
	wems_ecu-comms_ecubuffer-block defines the ecubuffer-block datatype.
	This datatype and the functions are defined in functional procedural
	manner similar to the gsl library. This is done for the reasons of
	-> Reduce overhead of objects calls in embedded enviroment.
	-> Reduce cohesion of object functions(seperated responsibility to functions etc.)
	►
*/


struct WemsEcuBufferBlock{
	ui8  scan_complete;
	ui32 header;
	ui32 bitque;
	ui16 channel_pos;
	ui16 bit_pos;
	si16 num_channels;
	si16 ecu_channel_count;
	si16 comm_pin;
	void* object_data;
	si16 reset_cnt;
	DataStreamSI16 ecu_stream;
};

const ui32 HEADERMASK = 0x3FFFFFFFu;

ui32 wems_EcuComms_GetHeader(void);

WemsEcuBufferBlock* wems_EcuComms_GetEcuBlock(si16 n_num_channels, si16 n_comm_pin);


#endif

