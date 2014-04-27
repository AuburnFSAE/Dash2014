#ifndef DATASTREAM
#define DATASTREAM

#include "wems_typedef.h"
#include <stdlib.h>

struct DataStreamSI16{
	union{
		ui8*  pU8;
		si16* pS16;
	};
	si16* data;
	si16 word_count;
	si16 sync_word;
	si16 word_size;
	si16 stream_length;
};


DataStreamSI16 wems_DataStream_GetSI16(si16 n_word_count);

#endif
