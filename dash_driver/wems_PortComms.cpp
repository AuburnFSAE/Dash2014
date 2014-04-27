#include "wems_PortComms.h"


PacketDescriptor wems_PortComms_GetDescriptor(ui8* header){
	PacketDescriptor descriptor;

	descriptor.word_size = header[0];

	descriptor.packet_width = header[1];

return descriptor;}


