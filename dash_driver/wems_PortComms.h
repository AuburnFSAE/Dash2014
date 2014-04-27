#ifndef PortComms
#define PortComms
#include "wems.h"
#include "wems_DataStream.h"

struct PacketDescriptor{
	ui8 word_size;
	ui8 packet_width;
};


PacketDescriptor wems_PortComms_GetDescriptor(ui8* bhead);
#endif
