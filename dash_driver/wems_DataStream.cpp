#include "wems_DataStream.h"

DataStreamSI16 wems_DataStream_GetSI16(si16 n_word_count){

	DataStreamSI16 dstream;
	dstream.pU8 = (ui8*)calloc(n_word_count+1, sizeof(si16));
	dstream.word_count=n_word_count;
	dstream.sync_word = 0xFFFF;
	dstream.word_size = 2;
	dstream.stream_length = (n_word_count+1)*2;
	dstream.pS16[0] = dstream.sync_word;
	dstream.data = dstream.pS16 + 1;

return dstream;}
